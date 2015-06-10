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

   class UnknownColumnException : public Exception {
   public:
      UnknownColumnException(const std::string& column);
      virtual ~UnknownColumnException();
   };

   class UnsupportedColumnException : public Exception {
   public:
      UnsupportedColumnException(const std::string& column);
      virtual ~UnsupportedColumnException();
   };

}
