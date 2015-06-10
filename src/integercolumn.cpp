#include "integercolumn.hpp"

namespace seril {

   IntegerColumn::IntegerColumn(const std::string& name, const unsigned int& flags)
      : Column(name, flags)
   {

   }
   
   IntegerColumn::~IntegerColumn() {

   }

   bool IntegerColumn::is_integer() const {
      return true;
   }

}
