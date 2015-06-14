#pragma once
#include "Idatacontract.hpp"
#include <unordered_set>
#include <deque>
#include <mutex>
#include <sqlite3.h>

namespace seril {

   class SQLiteDataContract : public IDataContract {
   public:
      typedef std::function<sqlite3*()> ConnectionFactory;

      SQLiteDataContract(const ConnectionFactory& connection_factory);
      SQLiteDataContract(SQLiteDataContract&& other);
      virtual ~SQLiteDataContract();

      virtual IQueryContext* query();
      virtual ISerializationContext* serialization(const std::string& name, const Schema& schema, Transaction& transaction);
      virtual IDeserializationContext* deserialization(const std::string& name, const Schema& schema, const ISerialized* serialized);

      SQLiteDataContract& operator =(SQLiteDataContract&& other);

   private:
      static bool is_valid_name(const std::string& str);

      void use_schema(const std::string& name, const Schema& schema);
      void check_errors(int rc) const;

      sqlite3* open();
      void close(sqlite3* connection);

      ConnectionFactory _connection_factory;
      std::mutex _mutex;
      std::deque<sqlite3*> _connection_pool;
      std::unordered_set<std::string> _closed_set;

      SQLiteDataContract(const SQLiteDataContract&);
      SQLiteDataContract& operator =(const SQLiteDataContract&);

      friend class SQLiteConnection;
   };

}
