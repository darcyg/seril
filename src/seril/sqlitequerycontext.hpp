#pragma once
#include "Iquerycontext.hpp"
#include "Idatacontract.hpp"
#include <unordered_map>
#include <functional>
#include <sqlite3.h>

namespace seril {

   class SQLiteQueryContext : public IQueryContext {
   public:
      typedef std::unordered_map<std::string, std::function<int(sqlite3_stmt*, int)>> Binds;

      SQLiteQueryContext();
      virtual ~SQLiteQueryContext();

      virtual ISerialized* apply();
      virtual void sint(const std::string& name, const int& value);
      virtual void uint(const std::string& name, const unsigned int& value);
      virtual void fpoint(const std::string& name, const float& value);
      virtual void dpoint(const std::string& name, const double& value);
      virtual void utf8(const std::string& name, const std::string& value);
      virtual void utf16(const std::string& name, const std::u16string& value);

   private:
      Binds _binds;

      SQLiteQueryContext(const SQLiteQueryContext&);
      SQLiteQueryContext& operator =(const SQLiteQueryContext&);
   };

}
