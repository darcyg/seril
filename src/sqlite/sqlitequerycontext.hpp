#pragma once
#include "Iquerycontext.hpp"
#include "Idatacontract.hpp"
#include <unordered_map>
#include <functional>
#include <sqlite/sqlite3.h>

namespace seril {

   class SQLiteQueryContext : public IQueryContext {
   public:
      typedef std::unordered_map<std::string, std::function<int(sqlite3_stmt*, int)>> Binds;

      SQLiteQueryContext();
      virtual ~SQLiteQueryContext();

      virtual ISerialized* apply();
      virtual void integer(const std::string& name, const int& value);
      virtual void number(const std::string& name, const double& value);
      virtual void string(const std::string& name, const std::string& value);
      virtual void wstring(const std::string& name, const std::wstring& value);

   private:
      Binds _binds;

      SQLiteQueryContext(const SQLiteQueryContext&);
      SQLiteQueryContext& operator =(const SQLiteQueryContext&);
   };

}
