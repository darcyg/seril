#include "serializationexception.hpp"

namespace seril {

   Exception::Exception(const std::string& message) : std::runtime_error(message) {

   }

   Exception::~Exception() {

   }


   InvalidNameException::InvalidNameException(const std::string& name)
      : Exception("Invalid name, only alphanumericals are allowed: " + name)
   {

   }

   InvalidNameException::~InvalidNameException() {

   }


   SchemaIsEmptyException::SchemaIsEmptyException(const std::string& name)
      : Exception("Schema cannot be empty: " + name)
   {

   }

   SchemaIsEmptyException::~SchemaIsEmptyException() {

   }


   UnknownColumnException::UnknownColumnException(const std::string& column)
      : Exception("Unknown column name: " + column)
   {

   }

   UnknownColumnException::~UnknownColumnException() {

   }


   UnsupportedColumnException::UnsupportedColumnException(const std::string& column)
      : Exception("Unsupported column: " + column)
   {

   }

   UnsupportedColumnException::~UnsupportedColumnException() {

   }

}
