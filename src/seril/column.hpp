#pragma once
#include "Idatacolumn.hpp"
#include <string>

namespace seril {

   template <unsigned int Type>
   class Column : public IDataColumn {
   public:
      static const unsigned int type_id = Type;

      Column(const std::string& name, const unsigned int& flags = 0);
      virtual ~Column();

      virtual const std::string& name() const;
      virtual bool is(const Flag& flag) const;
      virtual unsigned int type() const;

   private:
      std::string _name;
      unsigned int _flags;
   };

   typedef Column<0> sint_column;
   typedef Column<1> uint_column;
   typedef Column<2> fpoint_column;
   typedef Column<3> dpoint_column;
   typedef Column<4> binary_column;
   typedef Column<5> utf8_column;
   typedef Column<6> utf16_column;
}

#include "column.inl"
