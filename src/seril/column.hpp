#pragma once
#include "Idatacolumn.hpp"
#include <string>

namespace seril {

   class Column : public IDataColumn {
   public:
      Column(const std::string& name, const unsigned int& flags = 0);
      virtual ~Column();

      virtual const std::string& name() const;
      virtual bool is(const Flag& flag) const;

      virtual bool is_integer() const;
      virtual bool is_number() const;
      virtual bool is_string() const;
      virtual bool is_binary() const;

   private:
      std::string _name;
      unsigned int _flags;
   };

}
