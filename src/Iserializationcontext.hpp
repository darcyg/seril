#pragma once
#include "Iserialized.hpp"
#include <string>
#include <vector>

namespace seril {

   class ISerializationContext {
   public:
      virtual ~ISerializationContext() {}

      virtual ISerialized* apply() = 0;
      virtual void sint(const std::string& name, const int& value) = 0;
      virtual void binary(const std::string& name, const std::vector<unsigned char>& value) = 0;
   };

}
