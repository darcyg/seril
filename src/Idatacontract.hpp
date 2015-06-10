#pragma once
#include "Idatacolumn.hpp"
#include <string>
#include <set>
#include <memory>

namespace seril {

   class IQueryContext;
   class ISerializationContext;
   class IDeserializationContext;
   class ISerialized;
   class Transaction;

   class IDataContract {
   public:
      struct less {
         bool operator()(const std::shared_ptr<IDataColumn>& a, const std::shared_ptr<IDataColumn>& b) {
            return a->name() < b->name();
         }
      };

      typedef std::set<std::shared_ptr<IDataColumn>, less> Schema;

      virtual ~IDataContract() {}

      virtual IQueryContext* query() = 0;
      virtual ISerializationContext* serialization(const std::string& name, const Schema& schema, Transaction& transaction) = 0;
      virtual IDeserializationContext* deserialization(const std::string& name, const Schema& schema, const std::shared_ptr<ISerialized>& serialized = nullptr) = 0;
   };

}
