#pragma once
#include <atomic>

namespace seril {

   class Transaction {
   public:
      Transaction();
      ~Transaction();

      bool join();
      bool leave();
      bool complete();

      Transaction(const Transaction&) = delete;
      Transaction& operator =(const Transaction&) = delete;

   private:
      std::atomic<size_t> _joins, _leaves, _completions;
   };


   inline
   Transaction::Transaction() {

   }

   inline
   Transaction::~Transaction() {

   }

   inline
   bool Transaction::join() {
      return _joins.fetch_add(1) == 0;
   }

   inline
   bool Transaction::leave() {
      return _leaves.fetch_add(1) + 1 == _joins && _completions != _joins;
   }

   inline
   bool Transaction::complete() {
      return _completions.fetch_add(1) + 1 == _joins;
   }


}
