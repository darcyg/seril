#pragma once
#include "Iserialized.hpp"
#include "sqlitequerycontext.hpp"

namespace seril {

   class SQLiteSerialized : public ISerialized {
   public:
      SQLiteSerialized(SQLiteQueryContext::Binds&& binds);
      virtual ~SQLiteSerialized();

      const SQLiteQueryContext::Binds& binds() const;

   private:
      SQLiteQueryContext::Binds _binds;
   };

}
