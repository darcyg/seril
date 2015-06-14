#pragma once
#include "Iserialized.hpp"
#include <string>
#include <vector>

namespace seril {

   class ISerializationContext {
   public:
      virtual ~ISerializationContext() {}

      virtual ISerialized* apply() = 0;
      virtual void integer(const std::string& name, const int& value) = 0;
      virtual void number(const std::string& name, const double& value) = 0;
      virtual void string(const std::string& name, const std::string& value) = 0;
      virtual void wstring(const std::string& name, const std::wstring& value) = 0;
      virtual void binary(const std::string& name, const std::vector<unsigned char>& value) = 0;
   };

}
