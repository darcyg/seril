#include "sqliteserializationcontext.hpp"
#include "serializationexception.hpp"
#include "sqliteserialized.hpp"
#include <sstream>

namespace seril {

   SQLiteSerializationContext::SQLiteSerializationContext(sqlite3* db, const std::string& name, const IDataContract::Schema& schema)
      : _map(), _stmt(nullptr)
   {
      if (schema.empty())
         throw SchemaIsEmptyException(name);

      std::stringstream sql;

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
         const Slot s = { (int)std::distance(it, std::begin(schema)), (*it)->is(IDataColumn::Identifier), false };

         _map.insert(std::make_pair((*it)->name(), s));

         if (it != std::begin(schema))
            sql << ", ";

         sql << '?';
      }

      sql << ')';

      const std::string replace(sql.str());

      check_errors(sqlite3_prepare_v2(db, replace.data(), (int)replace.size(), &_stmt, nullptr));
   }

   SQLiteSerializationContext::~SQLiteSerializationContext() {
      if (_stmt != nullptr)
         sqlite3_finalize(_stmt);
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

      return new SQLiteSerialized(std::move(_pk_binds));
   }

   void SQLiteSerializationContext::sint(const std::string& name, const int& value) {
      auto &slot = bind(name);

      check_errors(sqlite3_bind_int(_stmt, slot.position, value));

      if (slot.is_pk)
         _pk_binds.insert(std::make_pair(name, [value](sqlite3_stmt* stmt, int position) {
            return sqlite3_bind_int(stmt, position, value);
         }));
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
