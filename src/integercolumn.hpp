#pragma once
#include "column.hpp"

namespace seril {

   class IntegerColumn : public Column {
   public:
      IntegerColumn(const std::string& name, const bool& is_unsigned, const unsigned int& flags);
      virtual ~IntegerColumn();

      bool is_unsigned() const;

      virtual bool is_integer() const;

   private:
      bool _is_unsigned;
   };

}
