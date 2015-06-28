#pragma once
#include "Iserialized.hpp"
#include <string>

namespace seril {

   class IQueryContext {
   public:
      virtual ~IQueryContext() {}

      virtual ISerialized* apply() = 0;
      virtual void sint(const std::string& name, const int& value) = 0;
      virtual void uint(const std::string& name, const unsigned int& value) = 0;
      virtual void fpoint(const std::string& name, const float& value) = 0;
      virtual void dpoint(const std::string& name, const double& value) = 0;
      virtual void utf8(const std::string& name, const std::string& value) = 0;
      virtual void utf16(const std::string& name, const std::u16string& value) = 0;
   };

}
