#pragma once
#include "Iserialized.hpp"
#include <string>

namespace seril {

   class IQueryContext {
   public:
      virtual ~IQueryContext() {}

      virtual ISerialized* apply() = 0;
      virtual void integer(const std::string& name, const int& value) = 0;
      virtual void number(const std::string& name, const double& value) = 0;
      virtual void string(const std::string& name, const std::string& value) = 0;
      virtual void wstring(const std::string& name, const std::wstring& value) = 0;
   };

}
