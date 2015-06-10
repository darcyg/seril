#pragma once
#include "Idatacontract.hpp"
#include "Iserializationcontext.hpp"
#include "sqlitequerycontext.hpp"
#include <unordered_map>
#include <sqlite/sqlite3.h>

namespace seril {

   class SQLiteDataSchema;
   class SQLiteDataColumn;

   class SQLiteSerializationContext : public ISerializationContext {
   public:
      struct Slot {
         int position;
         bool is_pk;
         bool binded;
      };

      SQLiteSerializationContext(sqlite3* db, const std::string& name, const IDataContract::Schema& schema, Transaction& transaction);
      virtual ~SQLiteSerializationContext();

      virtual ISerialized* apply();
      virtual void integer(const std::string& name, const int& value);
      virtual void number(const std::string& name, const double& value);
      virtual void string(const std::string& name, const std::string& value);
      virtual void wstring(const std::string& name, const std::wstring& value);
      virtual void binary(const std::string& name, const std::vector<unsigned char>& value);

   private:
      Transaction& _transaction;
      std::unordered_map<std::string, Slot> _map;
      sqlite3_stmt* _stmt;
      sqlite3* _db;
      SQLiteQueryContext _pk_query;
      bool _in_transaction;

      const Slot& bind(const std::string& name);
      void check_errors(int rc) const;

      SQLiteSerializationContext(const SQLiteSerializationContext&);
      SQLiteSerializationContext& operator =(const SQLiteSerializationContext&);
   };

}
