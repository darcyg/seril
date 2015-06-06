#pragma once
#include <vector>

namespace seril {

   class IDeserializationContext {
   public:
      virtual ~IDeserializationContext() {}

      virtual bool next() = 0;
      virtual int sint(const std::string& name) = 0;
      virtual std::vector<unsigned char> binary(const std::string& name) = 0;
   };

}
