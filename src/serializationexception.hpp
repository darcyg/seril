#pragma once
#include <stdexcept>

namespace seril {

   class Exception : public std::runtime_error {
   public:
      Exception(const std::string& message);
      virtual ~Exception();
   };

   class InvalidNameException : public Exception {
   public:
      InvalidNameException(const std::string& name);
      virtual ~InvalidNameException();
   };

   class SchemaIsEmptyException : public Exception {
   public:
      SchemaIsEmptyException(const std::string& name);
      virtual ~SchemaIsEmptyException();
   };

   class ColumnAlreadyExistsException : public Exception {
   public:
      ColumnAlreadyExistsException(const std::string& column);
      virtual ~ColumnAlreadyExistsException();
   };

   class UnknownColumnException : public Exception {
   public:
      UnknownColumnException(const std::string& column);
      virtual ~UnknownColumnException();
   };

   class IdentifierColumnIsNullException : public Exception {
   public:
      IdentifierColumnIsNullException(const std::string& column);
      virtual ~IdentifierColumnIsNullException();
   };

   class ColumnTypeMismatchException : public Exception {
   public:
      ColumnTypeMismatchException(const std::string& column);
      virtual ~ColumnTypeMismatchException();
   };

   class ColumnDoesNotAllowNullsException : public Exception {
   public:
      ColumnDoesNotAllowNullsException(const std::string& column);
      virtual ~ColumnDoesNotAllowNullsException();
   };

}
