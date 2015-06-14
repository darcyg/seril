#pragma once
#include "column.hpp"

namespace seril {

   class BinaryColumn : public Column {
   public:
      BinaryColumn(const std::string& name, const unsigned int& flags = 0);
      virtual ~BinaryColumn();

      virtual bool is_binary() const;
   };

}
