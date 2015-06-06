#pragma once
#include "column.hpp"

namespace seril {

   class BinaryColumn : public Column {
   public:
      BinaryColumn(const std::string& name, const unsigned int& flags);
      virtual ~BinaryColumn();

      virtual bool is_binary() const;
   };

}
