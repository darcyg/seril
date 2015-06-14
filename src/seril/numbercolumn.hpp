#pragma once
#include "column.hpp"

namespace seril {

   class NumberColumn : public Column {
   public:
      NumberColumn(const std::string& name, const unsigned int& flags = 0);
      virtual ~NumberColumn();

      virtual bool is_number() const;
   };

}
