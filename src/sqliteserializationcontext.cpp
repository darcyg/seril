#include "sqliteserializationcontext.hpp"
#include "serializationexception.hpp"
#include "transaction.hpp"
#include <sstream>
#include <thread>

namespace seril {

   SQLiteSerializationContext::SQLiteSerializationContext(SQLiteConnection&& connection, const std::string& name, const IDataContract::Schema& schema, Transaction& transaction)
      : _connection(std::move(connection)), _transaction(transaction), _map(), _stmt(nullptr), _pk_query(), _in_transaction(false)
   {
      if (schema.empty())
         throw SchemaIsEmptyException(name);

      try {
         if (_transaction.join())
            check_errors(sqlite3_exec(*_connection, "BEGIN TRANSACTION", nullptr, nullptr, nullptr));
      }
      catch (...) {
         _transaction.leave();
         throw;
      }

      _in_transaction = true;

      std::stringstream sql;
      int position = 0;

      sql << "INSERT OR REPLACE INTO ";
      sql << name;
      sql << '(';

      for (auto it = std::begin(schema); it != std::end(schema); ++it) {
         if (it != std::begin(schema))
            sql << ", ";

         sql << (*it)->name();
      }

      sql << ") VALUES(";

      for (auto it = std::begin(schema); it != std::end(schema); ++it) {
         const Slot s = { ++position, (*it)->is(IDataColumn::Identifier), false };

         _map.insert(std::make_pair((*it)->name(), s));

         if (it != std::begin(schema))
            sql << ", ";

         sql << '?';
      }

      sql << ')';

      check_errors(sqlite3_prepare_v2(*_connection, sql.str().data(), (int)sql.str().size(), &_stmt, nullptr));
   }

   SQLiteSerializationContext::~SQLiteSerializationContext() {
      if (_stmt != nullptr)
         sqlite3_finalize(_stmt);

      if (_in_transaction && _transaction.leave())
         sqlite3_exec(*_connection, "ROLLBACK TRANSACTION", nullptr, nullptr, nullptr);
   }

   ISerialized* SQLiteSerializationContext::apply() {
      for (auto &kv : _map) {
         if (!kv.second.binded)
            sqlite3_bind_null(_stmt, kv.second.position);

         kv.second.binded = false;
      }

      auto rc = sqlite3_step(_stmt);

      if (rc != SQLITE_DONE)
         check_errors(rc);

      sqlite3_reset(_stmt);

      if (_in_transaction && _transaction.complete()) {
         size_t max_spins = 1000;

         _in_transaction = false;

         for (int rc;;) {
            rc = sqlite3_exec(*_connection, "COMMIT TRANSACTION", nullptr, nullptr, nullptr);

            if (rc != SQLITE_BUSY)
               break;

            if (max_spins == 0) {
               sqlite3_exec(*_connection, "ROLLBACK TRANSACTION", nullptr, nullptr, nullptr);

               throw Exception("Cannot commit the transaction, underlying provider is busy");
            }

            std::this_thread::sleep_for(std::chrono::milliseconds(1));
            --max_spins;
         }
      }

      return _pk_query.apply();
   }

   void SQLiteSerializationContext::sint(const std::string& name, const int& value) {
      auto &slot = bind(name);

      check_errors(sqlite3_bind_int(_stmt, slot.position, value));

      if (slot.is_pk)
         _pk_query.sint(name, value);
   }

   void SQLiteSerializationContext::uint(const std::string& name, const unsigned int& value) {
      auto &slot = bind(name);

      check_errors(sqlite3_bind_int(_stmt, slot.position, (int)value));

      if (slot.is_pk)
         _pk_query.uint(name, value);
   }

   void SQLiteSerializationContext::fpoint(const std::string& name, const float& value) {
      auto &slot = bind(name);

      check_errors(sqlite3_bind_double(_stmt, slot.position, (double)value));

      if (slot.is_pk)
         _pk_query.fpoint(name, value);
   }

   void SQLiteSerializationContext::dpoint(const std::string& name, const double& value) {
      auto &slot = bind(name);

      check_errors(sqlite3_bind_double(_stmt, slot.position, value));

      if (slot.is_pk)
         _pk_query.dpoint(name, value);
   }

   void SQLiteSerializationContext::utf8(const std::string& name, const std::string& value) {
      auto &slot = bind(name);

      check_errors(sqlite3_bind_text(_stmt, slot.position, value.data(), (int)value.length(), SQLITE_TRANSIENT));

      if (slot.is_pk)
         _pk_query.utf8(name, value);
   }

   void SQLiteSerializationContext::utf16(const std::string& name, const std::u16string& value) {
      auto &slot = bind(name);

      check_errors(sqlite3_bind_text16(_stmt, slot.position, value.data(), (int)(value.length() * sizeof(std::u16string::value_type)), SQLITE_TRANSIENT));

      if (slot.is_pk)
         _pk_query.utf16(name, value);
   }

   void SQLiteSerializationContext::binary(const std::string& name, const std::vector<unsigned char>& value) {
      auto &slot = bind(name);

      check_errors(sqlite3_bind_blob(_stmt, slot.position, value.data(), (int)value.size(), SQLITE_STATIC));
   }

   const SQLiteSerializationContext::Slot& SQLiteSerializationContext::bind(const std::string& name) {
      auto it = _map.find(name);

      if (it == std::end(_map))
         throw UnknownColumnException(name);

      it->second.binded = true;

      return it->second;
   }

   void SQLiteSerializationContext::check_errors(int rc) const {
      if (rc != SQLITE_OK)
         throw Exception(std::string("SQLite error: ") + sqlite3_errstr(rc));
   }

}
