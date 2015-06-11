#pragma once
#include "Idatacontract.hpp"
#include "sqlitequerycontext.hpp"
#include "sqliteserializationcontext.hpp"
#include "sqlitedeserializationcontext.hpp"
#include <unordered_set>
#include <sqlite/sqlite3.h>

namespace seril {

   class SQLiteDataContract : public IDataContract {
   public:
      SQLiteDataContract(sqlite3* db = nullptr);
      SQLiteDataContract(SQLiteDataContract&& other);
      virtual ~SQLiteDataContract();

      virtual SQLiteQueryContext* query();
      virtual SQLiteSerializationContext* serialization(const std::string& name, const Schema& schema, Transaction& transaction);
      virtual SQLiteDeserializationContext* deserialization(const std::string& name, const Schema& schema, const ISerialized* serialized);

      SQLiteDataContract& operator =(SQLiteDataContract&& other);

   private:
      static bool is_valid_name(const std::string& str);

      void use_schema(const std::string& name, const Schema& schema);
      void check_errors(int rc) const;

      sqlite3* _db;
      std::unordered_set<std::string> _closed_set;

      SQLiteDataContract(const SQLiteDataContract&);
      SQLiteDataContract& operator =(const SQLiteDataContract&);
   };

}
