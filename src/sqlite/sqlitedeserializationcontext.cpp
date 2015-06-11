#include "sqlitedeserializationcontext.hpp"
#include "serializationexception.hpp"
#include "sqliteserialized.hpp"
#include <sqlite/sqlite3.h>
#include <sstream>

namespace seril {

   SQLiteDeserializationContext::SQLiteDeserializationContext(sqlite3* db, const std::string& name, const IDataContract::Schema& schema, const ISerialized* serialized)
      : _map(), _stmt(nullptr)
   {
      if (schema.empty())
         throw SchemaIsEmptyException(name);

      std::stringstream sql;
      int position = 0;

      sql << "SELECT ";

      for (auto it = std::begin(schema); it != std::end(schema); ++it) {
         _map.insert(std::make_pair((*it)->name(), position++));

         if (it != std::begin(schema))
            sql << ", ";

         sql << (*it)->name();
      }

      sql << " FROM ";
      sql << name;

      auto sql_serialized = static_cast<const SQLiteSerialized*>(serialized);

      if (sql_serialized != nullptr && !sql_serialized->binds().empty()) {
         sql << " WHERE ";

         for (auto it = std::begin(sql_serialized->binds()); it != std::end(sql_serialized->binds()); ++it) {
            if (it != std::begin(sql_serialized->binds()))
               sql << " AND ";

            sql << it->first;
            sql << " = ?";
         }
      }

      const std::string select(sql.str());

      check_errors(sqlite3_prepare_v2(db, select.data(), (int)select.size(), &_stmt, nullptr));

      if (sql_serialized != nullptr) {
         position = 0;

         for (auto &bind : sql_serialized->binds())
            bind.second(_stmt, ++position);
      }
   }

   SQLiteDeserializationContext::~SQLiteDeserializationContext() {
      if (_stmt != nullptr)
         sqlite3_finalize(_stmt);
   }

   bool SQLiteDeserializationContext::next() {
      auto rc = sqlite3_step(_stmt);

      if (rc == SQLITE_ROW)
         return true;

      if (rc != SQLITE_DONE)
         check_errors(rc);

      return false;
   }

   int SQLiteDeserializationContext::sint(const std::string& name) {
      return sqlite3_column_int(_stmt, get_column(name));
   }

   unsigned int SQLiteDeserializationContext::uint(const std::string& name) {
      return (unsigned int)sqlite3_column_int(_stmt, get_column(name));
   }

   float SQLiteDeserializationContext::fnumber(const std::string& name) {
      return (float)sqlite3_column_double(_stmt, get_column(name));
   }

   double SQLiteDeserializationContext::dnumber(const std::string& name) {
      return sqlite3_column_double(_stmt, get_column(name));
   }

   std::string SQLiteDeserializationContext::string(const std::string& name) {
      auto pos = get_column(name);

      auto text = reinterpret_cast<const char*>(sqlite3_column_text(_stmt, pos));
      auto size = sqlite3_column_bytes(_stmt, pos);

      return std::move(std::string(text, text + size));
   }

   std::wstring SQLiteDeserializationContext::wstring(const std::string& name) {
      static_assert(sizeof(std::wstring::value_type) != 16, "System's wchar_t is other than 16 bits long which is not supported");

      auto pos = get_column(name);

      auto text = reinterpret_cast<const wchar_t*>(sqlite3_column_text16(_stmt, pos));
      auto size = sqlite3_column_bytes(_stmt, pos);

      return std::move(std::wstring(text, text + size));
   }

   std::vector<unsigned char> SQLiteDeserializationContext::binary(const std::string& name) {
      auto pos = get_column(name);

      auto blob = reinterpret_cast<const unsigned char*>(sqlite3_column_blob(_stmt, pos));
      auto size = sqlite3_column_bytes(_stmt, pos);

      return std::move(std::vector<unsigned char>(blob, blob + size));
   }

   void SQLiteDeserializationContext::check_errors(int rc) const {
      if (rc != SQLITE_OK)
         throw Exception(std::string("SQLite error: ") + sqlite3_errstr(rc));
   }

   int SQLiteDeserializationContext::get_column(const std::string& name) const {
      auto it = _map.find(name);

      if (it == std::end(_map))
         throw UnknownColumnException(name);

      return it->second;
   }

}
