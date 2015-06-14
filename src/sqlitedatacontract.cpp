#include "sqlitedatacontract.hpp"
#include "serializationexception.hpp"
#include "sqliteconnection.hpp"
#include "sqliteserializationcontext.hpp"
#include "sqlitedeserializationcontext.hpp"
#include "sqlitequerycontext.hpp"
#include "column.hpp"
#include <sstream>

namespace seril {

   SQLiteDataContract::SQLiteDataContract(const ConnectionFactory& connection_factory)
      : _connection_factory(connection_factory), _mutex(), _connection_pool(), _closed_set()
   {

   }

   SQLiteDataContract::SQLiteDataContract(SQLiteDataContract&& other)
      : _connection_factory(std::move(other._connection_factory)), _mutex(), 
        _connection_pool(std::move(other._connection_pool)), _closed_set(std::move(other._closed_set))
   {

   }

   SQLiteDataContract::~SQLiteDataContract() {
      std::unique_lock<std::mutex> lock(_mutex);

      for (auto &connection : _connection_pool)
         sqlite3_close(connection);
   }

   IQueryContext* SQLiteDataContract::query() {
      return new SQLiteQueryContext();
   }

   ISerializationContext* SQLiteDataContract::serialization(const std::string& name, const IDataContract::Schema& schema, Transaction& transaction) {
      use_schema(name, schema);

      return new SQLiteSerializationContext(SQLiteConnection(*this), name, schema, transaction);
   }

   IDeserializationContext* SQLiteDataContract::deserialization(const std::string& name, const IDataContract::Schema& schema, const ISerialized* serialized) {
      use_schema(name, schema);

      return new SQLiteDeserializationContext(SQLiteConnection(*this), name, schema, serialized);
   }

   sqlite3* SQLiteDataContract::open() {
      std::unique_lock<std::mutex> lock(_mutex);

      if (_connection_pool.empty())
         return _connection_factory();

      auto connection = _connection_pool.front();
      _connection_pool.pop_front();

      return connection;
   }

   void SQLiteDataContract::close(sqlite3* connection) {
      std::unique_lock<std::mutex> lock(_mutex);

      _connection_pool.push_back(connection);
   }

   void SQLiteDataContract::use_schema(const std::string& name, const IDataContract::Schema& schema) {
      if (!is_valid_name(name))
         throw InvalidNameException(name);

      {
         std::unique_lock<std::mutex> lock(_mutex);

         auto it = _closed_set.find(name);

         if (it != std::end(_closed_set))
            return;
      }

      if (schema.empty())
         throw SchemaIsEmptyException(name);

      std::stringstream sql;

      sql << "CREATE TABLE IF NOT EXISTS ";
      sql << name;
      sql << '(';

      size_t total_pks = 0;

      for (auto it = std::begin(schema); it != std::end(schema); ++it) {
         auto column = std::static_pointer_cast<Column>(*it);

         if (it != std::begin(schema))
            sql << ", ";

         sql << column->name();

         if (column->is_integer())
            sql << " INT";

         else if (column->is_number())
            sql << " REAL";

         else if (column->is_string())
            sql << " TEXT";

         else if (column->is_binary())
            sql << " BLOB";

         else
            throw UnsupportedColumnException(column->name());

         if (column->is(IDataColumn::NotNull))
            sql << " NOT NULL";

         if (column->is(IDataColumn::Identifier))
            ++total_pks;
      }

      if (total_pks > 0) {
         size_t pk = 0;

         sql << ", PRIMARY KEY (";

         for (auto &column : schema) {
            if (!column->is(IDataColumn::Identifier))
               continue;

            if (pk > 0)
               sql << ", ";

            sql << column->name();
            ++pk;
         }

         sql << ')';
      }

      sql << ')';

      const std::string create(sql.str());

      {
         SQLiteConnection connection(*this);
         sqlite3_stmt* stmt;
         check_errors(sqlite3_prepare_v2(*connection, create.data(), (int)create.size(), &stmt, nullptr));

         try {
            auto rc = sqlite3_step(stmt);

            if (rc != SQLITE_DONE)
               check_errors(rc);
         }
         catch (...) {
            sqlite3_finalize(stmt);
            throw;
         }
      }

      {
         std::unique_lock<std::mutex> lock(_mutex);
         _closed_set.insert(name);
      }
   }

   void SQLiteDataContract::check_errors(int rc) const {
      if (rc != SQLITE_OK)
         throw Exception(std::string("SQLite error: ") + sqlite3_errstr(rc));
   }

   SQLiteDataContract& SQLiteDataContract::operator =(SQLiteDataContract&& other) {
      if (this != &other) {
         _connection_factory = std::move(other._connection_factory);
         _connection_pool = std::move(other._connection_pool);
         _closed_set = std::move(other._closed_set);
      }

      return *this;
   }

   bool SQLiteDataContract::is_valid_name(const std::string& str) {
      for (auto &c : str) {
         if (!isalnum(c) && !isspace(c))
            return false;
      }

      return !str.empty();
   }
}
