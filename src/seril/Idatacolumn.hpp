#pragma once
#include "Idatacolumn.hpp"
#include <string>

namespace seril {

   class IDataColumn {
   public:
      enum Flag {
         None = 0,
         Identifier = 1,
         NotNull = 2
      };

      virtual ~IDataColumn() {}

      virtual const std::string& name() const = 0;
      virtual bool is(const Flag& flag) const = 0;
      virtual unsigned int type() const = 0;
   };

}
