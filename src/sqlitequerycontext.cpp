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

   void SQLiteQueryContext::sint(const std::string& name, const int& value) {
      _binds.insert(std::make_pair(name, [value](sqlite3_stmt* stmt, int position) {
         return sqlite3_bind_int(stmt, position, value);
      }));
   }

   void SQLiteQueryContext::uint(const std::string& name, const unsigned int& value) {
      _binds.insert(std::make_pair(name, [value](sqlite3_stmt* stmt, int position) {
         return sqlite3_bind_int(stmt, position, (int)value);
      }));
   }

   void SQLiteQueryContext::fpoint(const std::string& name, const float& value) {
      _binds.insert(std::make_pair(name, [value](sqlite3_stmt* stmt, int position) {
         return sqlite3_bind_double(stmt, position, (double)value);
      }));
   }

   void SQLiteQueryContext::dpoint(const std::string& name, const double& value) {
      _binds.insert(std::make_pair(name, [value](sqlite3_stmt* stmt, int position) {
         return sqlite3_bind_double(stmt, position, value);
      }));
   }

   void SQLiteQueryContext::utf8(const std::string& name, const std::string& value) {
      _binds.insert(std::make_pair(name, [value](sqlite3_stmt* stmt, int position) {
         return sqlite3_bind_text(stmt, position, value.data(), (int)value.length(), SQLITE_TRANSIENT);
      }));
   }

   void SQLiteQueryContext::utf16(const std::string& name, const std::u16string& value) {
      _binds.insert(std::make_pair(name, [value](sqlite3_stmt* stmt, int position) {
         return sqlite3_bind_text16(stmt, position, value.data(), (int)(value.length() * sizeof(std::u16string::value_type)), SQLITE_TRANSIENT);
      }));
   }

}
