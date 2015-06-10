#include "stringcolumn.hpp"

namespace seril {

   StringColumn::StringColumn(const std::string& name, const unsigned int& flags)
      : Column(name, flags)
   {

   }
   
   StringColumn::~StringColumn() {

   }

   bool StringColumn::is_string() const {
      return true;
   }

}
