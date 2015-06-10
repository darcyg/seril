#pragma once
#include <vector>

namespace seril {

   class IDeserializationContext {
   public:
      virtual ~IDeserializationContext() {}

      virtual bool next() = 0;
      virtual int sint(const std::string& name) = 0;
      virtual unsigned int uint(const std::string& name) = 0;
      virtual float fnumber(const std::string& name) = 0;
      virtual double dnumber(const std::string& name) = 0;
      virtual std::string string(const std::string& name) = 0;
      virtual std::wstring wstring(const std::string& name) = 0;
      virtual std::vector<unsigned char> binary(const std::string& name) = 0;
   };

}
