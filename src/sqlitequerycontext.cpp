#include "sqlitequerycontext.hpp"
#include "serializationexception.hpp"
#include "sqliteserialized.hpp"

namespace seril {

   SQLiteQueryContext::SQLiteQueryContext() : _binds() {

   }

   SQLiteQueryContext::~SQLiteQueryContext() {

   }

   ISerialized* SQLiteQueryContext::apply() {
      return new SQLiteSerialized(std::move(_binds));
   }

   void SQLiteQueryContext::integer(const std::string& name, const int& value) {
      _binds.insert(std::make_pair(name, [value](sqlite3_stmt* stmt, int position) {
         return sqlite3_bind_int(stmt, position, value);
      }));
   }

   void SQLiteQueryContext::number(const std::string& name, const double& value) {
      _binds.insert(std::make_pair(name, [value](sqlite3_stmt* stmt, int position) {
         return sqlite3_bind_double(stmt, position, value);
      }));
   }

   void SQLiteQueryContext::string(const std::string& name, const std::string& value) {
      _binds.insert(std::make_pair(name, [value](sqlite3_stmt* stmt, int position) {
         return sqlite3_bind_text(stmt, position, value.data(), (int)value.size(), SQLITE_TRANSIENT);
      }));
   }

   void SQLiteQueryContext::wstring(const std::string& name, const std::wstring& value) {
      static_assert(sizeof(std::wstring::value_type) != 16, "System's wchar_t is other than 16 bits long which is not supported");

      _binds.insert(std::make_pair(name, [value](sqlite3_stmt* stmt, int position) {
         return sqlite3_bind_text16(stmt, position, value.data(), (int)(value.size() * sizeof(std::wstring::value_type)), SQLITE_TRANSIENT);
      }));
   }

}
