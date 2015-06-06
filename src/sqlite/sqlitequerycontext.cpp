#include "sqlitequerycontext.hpp"
#include "serializationexception.hpp"

namespace seril {

   SQLiteQueryContext::SQLiteQueryContext() : _binds() {

   }

   SQLiteQueryContext::~SQLiteQueryContext() {

   }

   SQLiteSerialized* SQLiteQueryContext::apply() {
      return new SQLiteSerialized(std::move(_binds));
   }

   void SQLiteQueryContext::sint(const std::string& name, const int& value) {
      _binds.insert(std::make_pair(name, [value](sqlite3_stmt* stmt, int position) {
         return sqlite3_bind_int(stmt, position, value);
      }));
   }

}
