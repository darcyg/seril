#include "sqliteserialized.hpp"

namespace seril {

   SQLiteSerialized::SQLiteSerialized(SQLiteSerializationContext::Binds&& binds)
      : _binds(std::move(binds))
   {

   }

   SQLiteSerialized::~SQLiteSerialized() {

   }

   const SQLiteSerializationContext::Binds& SQLiteSerialized::binds() const {
      return _binds;
   }

}
