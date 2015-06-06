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


   ColumnAlreadyExistsException::ColumnAlreadyExistsException(const std::string& column)
      : Exception("Column with the same name already exists in schema: " + column)
   {

   }

   ColumnAlreadyExistsException::~ColumnAlreadyExistsException() {

   }


   UnknownColumnException::UnknownColumnException(const std::string& column)
      : Exception("Unknown column name: " + column)
   {

   }

   UnknownColumnException::~UnknownColumnException() {

   }


   IdentifierColumnIsNullException::IdentifierColumnIsNullException(const std::string& column)
      : Exception("Identifier column cannot be empty: " + column)
   {

   }

   IdentifierColumnIsNullException::~IdentifierColumnIsNullException() {

   }


   ColumnTypeMismatchException::ColumnTypeMismatchException(const std::string& column)
      : Exception("Column type does not match the requested value type: " + column)
   {

   }

   ColumnTypeMismatchException::~ColumnTypeMismatchException() {

   }


   ColumnDoesNotAllowNullsException::ColumnDoesNotAllowNullsException(const std::string& column)
      : Exception("Column is NotNull but has no value")
   {

   }

   ColumnDoesNotAllowNullsException::~ColumnDoesNotAllowNullsException() {

   }

}
