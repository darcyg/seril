#include "sqliteserializationcontext.hpp"
#include "serializationexception.hpp"
#include "transaction.hpp"
#include <sstream>
#include <thread>

namespace seril {

   SQLiteSerializationContext::SQLiteSerializationContext(sqlite3* db, const std::string& name, const IDataContract::Schema& schema, Transaction& transaction)
      : _transaction(transaction), _map(), _stmt(nullptr), _db(db), _pk_query(), _in_transaction(false)
   {
      if (schema.empty())
         throw SchemaIsEmptyException(name);

      try {
         if (_transaction.join())
            check_errors(sqlite3_exec(_db, "BEGIN TRANSACTION", nullptr, nullptr, nullptr));
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

      const std::string replace(sql.str());

      check_errors(sqlite3_prepare_v2(_db, replace.data(), (int)replace.size(), &_stmt, nullptr));
   }

   SQLiteSerializationContext::~SQLiteSerializationContext() {
      if (_stmt != nullptr)
         sqlite3_finalize(_stmt);

      if (_in_transaction && _transaction.leave())
         sqlite3_exec(_db, "ROLLBACK TRANSACTION", nullptr, nullptr, nullptr);
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
            rc = sqlite3_exec(_db, "COMMIT TRANSACTION", nullptr, nullptr, nullptr);

            if (rc != SQLITE_BUSY)
               break;

            if (max_spins == 0) {
               sqlite3_exec(_db, "ROLLBACK TRANSACTION", nullptr, nullptr, nullptr);

               throw Exception("Cannot commit the transaction, underlying provider is busy");
            }

            std::this_thread::sleep_for(std::chrono::milliseconds(1));
            --max_spins;
         }
      }

      return _pk_query.apply();
   }

   void SQLiteSerializationContext::integer(const std::string& name, const int& value) {
      auto &slot = bind(name);

      check_errors(sqlite3_bind_int(_stmt, slot.position, value));

      if (slot.is_pk)
         _pk_query.integer(name, value);
   }

   void SQLiteSerializationContext::number(const std::string& name, const double& value) {
      auto &slot = bind(name);

      check_errors(sqlite3_bind_double(_stmt, slot.position, (double)value));

      if (slot.is_pk)
         _pk_query.number(name, value);
   }

   void SQLiteSerializationContext::string(const std::string& name, const std::string& value) {
      auto &slot = bind(name);

      check_errors(sqlite3_bind_text(_stmt, slot.position, value.data(), (int)value.size(), SQLITE_TRANSIENT));

      if (slot.is_pk)
         _pk_query.string(name, value);
   }

   void SQLiteSerializationContext::wstring(const std::string& name, const std::wstring& value) {
      static_assert(sizeof(std::wstring::value_type) != 16, "System's wchar_t is other than 16 bits long which is not supported");

      auto &slot = bind(name);

      check_errors(sqlite3_bind_text16(_stmt, slot.position, value.data(), (int)(value.length() * sizeof(std::wstring::value_type)), SQLITE_TRANSIENT));

      if (slot.is_pk)
         _pk_query.wstring(name, value);
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
