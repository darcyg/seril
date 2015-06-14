#include "sqliteserialized.hpp"

namespace seril {

   SQLiteSerialized::SQLiteSerialized(SQLiteQueryContext::Binds&& binds)
      : _binds(std::move(binds))
   {

   }

   SQLiteSerialized::~SQLiteSerialized() {

   }

   const SQLiteQueryContext::Binds& SQLiteSerialized::binds() const {
      return _binds;
   }

}
