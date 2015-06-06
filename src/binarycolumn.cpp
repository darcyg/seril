#include "binarycolumn.hpp"

namespace seril {

   BinaryColumn::BinaryColumn(const std::string& name, const unsigned int& flags) : Column(name, flags) {

   }

   BinaryColumn::~BinaryColumn() {

   }

   bool BinaryColumn::is_binary() const {
      return true;
   }

}
