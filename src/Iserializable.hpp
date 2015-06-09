#pragma once
#include "Iserialized.hpp"
#include <memory>

namespace seril {

   class IDataContract;

   class ISerializable {
   public:
      virtual ~ISerializable() {}

      virtual ISerialized* serialize(IDataContract& contract) const = 0;
      virtual void deserialize(IDataContract& contract, const std::shared_ptr<ISerialized>& serialized = nullptr) = 0;
   };

}
