#pragma once
#include "column.hpp"

namespace seril {

   class IntegerColumn : public Column {
   public:
      IntegerColumn(const std::string& name, const unsigned int& flags = 0);
      virtual ~IntegerColumn();

      virtual bool is_integer() const;
   };

}
