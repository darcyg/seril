#pragma once
#include "sqlitedatacontract.hpp"
#include <sqlite/sqlite3.h>

namespace seril {

   class SQLiteConnection {
   public:
      SQLiteConnection(SQLiteDataContract& contract)
         : _contract(contract), _db(contract.open())
      {

      }

      SQLiteConnection(SQLiteConnection&& other)
         : _contract(other._contract), _db(other._db)
      {
         other._db = nullptr;
      }

      ~SQLiteConnection() {
         if (_db != nullptr)
            _contract.close(_db);
      }

      sqlite3* operator *() const {
         return _db;
      }

   private:
      SQLiteDataContract& _contract;
      sqlite3* _db;

      SQLiteConnection(const SQLiteConnection&);
      SQLiteConnection& operator =(const SQLiteConnection&);
   };

}
