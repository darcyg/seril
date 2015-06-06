#include "integercolumn.hpp"

namespace seril {

   IntegerColumn::IntegerColumn(const std::string& name, const bool& is_unsigned, const unsigned int& flags)
      : _is_unsigned(is_unsigned), Column(name, flags)
   {

   }
   
   IntegerColumn::~IntegerColumn() {

   }

   bool IntegerColumn::is_unsigned() const {
      return _is_unsigned;
   }

   bool IntegerColumn::is_integer() const {
      return true;
   }

}
