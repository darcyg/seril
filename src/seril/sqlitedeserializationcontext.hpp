#pragma once
#include "Ideserializationcontext.hpp"
#include "Idatacontract.hpp"
#include "sqliteconnection.hpp"
#include <unordered_map>
#include <sqlite3.h>

namespace seril {

   class SQLiteDeserializationContext : public IDeserializationContext {
   public:
      SQLiteDeserializationContext(SQLiteConnection&& connection, const std::string& name, const IDataContract::Schema& schema, const ISerialized* serialized);
      virtual ~SQLiteDeserializationContext();

      virtual bool next();
      virtual int sint(const std::string& name);
      virtual unsigned int uint(const std::string& name);
      virtual float fnumber(const std::string& name);
      virtual double dnumber(const std::string& name);
      virtual std::string string(const std::string& name);
      virtual std::wstring wstring(const std::string& name);
      virtual std::vector<unsigned char> binary(const std::string& name);

   private:
      SQLiteConnection _connection;
      std::unordered_map<std::string, int> _map;
      sqlite3_stmt* _stmt;

      int get_column(const std::string& name) const;
      void check_errors(int rc) const;

      SQLiteDeserializationContext(const SQLiteDeserializationContext&);
      SQLiteDeserializationContext& operator =(const SQLiteDeserializationContext&);
   };

}
