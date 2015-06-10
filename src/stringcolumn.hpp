#pragma once
#include "column.hpp"

namespace seril {

   class StringColumn : public Column {
   public:
      StringColumn(const std::string& name, const unsigned int& flags = 0);
      virtual ~StringColumn();

      virtual bool is_string() const;
   };

}
