
namespace seril {

   template <unsigned int Type> inline
   Column<Type>::Column(const std::string& name, const unsigned int& flags)
      : _name(name), _flags(flags)
   {

   }

   template <unsigned int Type>
   Column<Type>::~Column() {

   }

   template <unsigned int Type>
   const std::string& Column<Type>::name() const {
      return _name;
   }

   template <unsigned int Type>
   bool Column<Type>::is(const Flag& flag) const {
      return (_flags & flag) == (unsigned int) flag;
   }

   template <unsigned int Type>
   unsigned int Column<Type>::type() const {
      return Type;
   }

}
