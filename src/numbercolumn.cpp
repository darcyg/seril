#include "numbercolumn.hpp"

namespace seril {

   NumberColumn::NumberColumn(const std::string& name, const unsigned int& flags)
      : Column(name, flags)
   {

   }
   
   NumberColumn::~NumberColumn() {

   }

   bool NumberColumn::is_number() const {
      return true;
   }

}
