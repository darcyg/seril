#pragma once
#include "Idatacontract.hpp"
#include "Iserializationcontext.hpp"
#include "sqlitequerycontext.hpp"
#include "sqliteconnection.hpp"
#include <unordered_map>
#include <sqlite3.h>

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

      SQLiteSerializationContext(SQLiteConnection&& connection, const std::string& name, const IDataContract::Schema& schema, Transaction& transaction);
      virtual ~SQLiteSerializationContext();

      virtual ISerialized* apply();
      virtual void sint(const std::string& name, const int& value);
      virtual void uint(const std::string& name, const unsigned int& value);
      virtual void fpoint(const std::string& name, const float& value);
      virtual void dpoint(const std::string& name, const double& value);
      virtual void utf8(const std::string& name, const std::string& value);
      virtual void utf16(const std::string& name, const std::u16string& value);
      virtual void binary(const std::string& name, const std::vector<unsigned char>& value);

   private:
      SQLiteConnection _connection;
      Transaction& _transaction;
      std::unordered_map<std::string, Slot> _map;
      sqlite3_stmt* _stmt;
      SQLiteQueryContext _pk_query;
      bool _in_transaction;

      const Slot& bind(const std::string& name);
      void check_errors(int rc) const;

      SQLiteSerializationContext(const SQLiteSerializationContext&);
      SQLiteSerializationContext& operator =(const SQLiteSerializationContext&);
   };

}
