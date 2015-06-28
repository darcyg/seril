#pragma once
#include <vector>
#include <string>

namespace seril {

   class IDeserializationContext {
   public:
      virtual ~IDeserializationContext() {}

      virtual bool next() = 0;
      virtual int sint(const std::string& name) = 0;
      virtual unsigned int uint(const std::string& name) = 0;
      virtual float fpoint(const std::string& name) = 0;
      virtual double dpoint(const std::string& name) = 0;
      virtual std::string utf8(const std::string& name) = 0;
      virtual std::u16string utf16(const std::string& name) = 0;
      virtual std::vector<unsigned char> binary(const std::string& name) = 0;
   };

}
