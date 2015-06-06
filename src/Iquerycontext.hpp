#pragma once
#include "Iserialized.hpp"
#include <string>

namespace seril {

   class IQueryContext {
   public:
      virtual ~IQueryContext() {}

      virtual ISerialized* apply() = 0;
      virtual void sint(const std::string& name, const int& value) = 0;
   };

}
