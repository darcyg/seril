#include "column.hpp"

namespace seril {

   Column::Column(const std::string& name, const unsigned int& flags) : _name(name), _flags(flags) {

   }
   
   Column::~Column() {

   }

   const std::string& Column::name() const {
      return _name;
   }

   bool Column::is(const Flag& flag) const {
      return (_flags & flag) == (unsigned int) flag;
   }

   bool Column::is_integer() const {
      return false;
   }

   bool Column::is_number() const {
      return false;
   }

   bool Column::is_string() const {
      return false;
   }

   bool Column::is_binary() const {
      return false;
   }

}
