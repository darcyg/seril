#pragma once
#include "Ideserializationcontext.hpp"
#include "Idatacontract.hpp"
#include <unordered_map>
#include <sqlite/sqlite3.h>

namespace seril {

   class SQLiteDeserializationContext : public IDeserializationContext {
   public:
      SQLiteDeserializationContext(sqlite3* db, const std::string& name, const IDataContract::Schema& schema, const std::shared_ptr<ISerialized>& serialized);
      virtual ~SQLiteDeserializationContext();

      virtual bool next();
      virtual int sint(const std::string& name);
      virtual std::vector<unsigned char> binary(const std::string& name);

   private:
      std::unordered_map<std::string, int> _map;
      sqlite3_stmt* _stmt;

      int get_column(const std::string& name) const;
      void check_errors(int rc) const;

      SQLiteDeserializationContext(const SQLiteDeserializationContext&);
      SQLiteDeserializationContext& operator =(const SQLiteDeserializationContext&);
   };

}
