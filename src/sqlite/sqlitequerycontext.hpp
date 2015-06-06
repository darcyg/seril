#pragma once
#include "Iquerycontext.hpp"
#include "Idatacontract.hpp"
#include "sqliteserializationcontext.hpp"
#include "sqliteserialized.hpp"
#include "sqlitequerycontext.hpp"

namespace seril {

   class SQLiteQueryContext : public IQueryContext {
   public:
      SQLiteQueryContext();
      virtual ~SQLiteQueryContext();

      virtual SQLiteSerialized* apply();
      virtual void sint(const std::string& name, const int& value);

   private:
      SQLiteSerializationContext::Binds _binds;

      SQLiteQueryContext(const SQLiteQueryContext&);
      SQLiteQueryContext& operator =(const SQLiteQueryContext&);
   };

}
