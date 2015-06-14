# seril

Object serialization library for C++.

The library allows you to do the following things:
   1. serialize object instances;
   2. deserialize object instances.

Object states are serialized in a platform independent manner so it is possible to serialize an object in one platform and restore it later in another. Serialization also takes care of types and saves other necessary information so that the serialization process is smooth and reliable as possible.


## Usage

Using the library requires you to declare `IDataContract::Schema` which defines how the data is laid down in the underlying data source. In schema you must define every column and their names that you want to include in the serialization. Classes derived from `Column` implement all possible column types that can be included in Schema. Column names are unique and they must be used consistently. Also, Schema must have a name - if you change the schema name, it will be recognized as new schema. When the schema is declared you can use `IDataContract::serialization()` and `IDataContract::deserialization()` methods to invoke the serialization flow.

Serialization flow consists two steps: serialiazation and deserialization. In serialization step, we save the current state which can later be restored in deserilization phase. For example, C++ classes can hold a state which can be serialized with this library. But the state doesn't have to be an object - it can be anything that should be saved and restored later. The state is not referenced with a filename, key or any other predefined identifier - rather you can query serialized states and define key constraints that naturally define state's identification methodology. Underlying data source makes sure that the identifiers are unique so not two states can be serialized with the same identification. This way of thinking seril is closer than an ORM than a common perception of serialization libraries.

Full usage example for serialization and deserialization of object states *:
```c++

#include <Idatacontract.hpp>
#include <Iserialized.hpp>
#include <transaction.hpp>
#include <sqlitedatacontract.hpp>
#include <string>
#include <sqlite3.h>
#include <iostream>

using namespace seril;

class ISerializable {
public:
   virtual ~ISerializable() {}

   virtual ISerialized* serialize(IDataContract& contract, Transaction&& transaction = Transaction()) const = 0;
   virtual bool deserialize(IDataContract& contract) = 0;
};

class Package {
public:
   Package(const std::string& filename) : _contract([filename]() {
      sqlite3* db;
      sqlite3_open(filename.data(), &db);

      return db;
   }) {

   }

   ISerialized* save(const ISerializable& obj) {
      return obj.serialize(_contract);
   }

   void load(ISerializable& obj) {
      obj.deserialize(_contract);
   }

private:
   SQLiteDataContract _contract;
};


static const std::string g_SchemaName("Greeter");
static const IDataContract::Schema g_Schema = {
   std::make_shared<IntegerColumn>("Id", IDataColumn::Identifier | IDataColumn::NotNull),
   std::make_shared<StringColumn>("Greeting", IDataColumn::NotNull)
};

// Any class that implements ISerializable interface.
class Greeter : public ISerializable {
public:
   Greeter(unsigned int id, const std::string& greeting = std::string()) : _id(id), _greeting(greeting) {

   }

   const std::string& greeting() const {
      return _greeting;
   }

   virtual ISerialized* serialize(IDataContract& contract, Transaction&& transaction) const {
      std::unique_ptr<ISerializationContext> context(contract.serialization(g_SchemaName, g_Schema, transaction));

      context->integer("Id", _id);
      context->string("Greeting", _greeting);

      return context->apply();
   }

   virtual bool deserialize(IDataContract& contract) {
      std::unique_ptr<IQueryContext> q(contract.query());

      q->integer("Id", _id);

      std::unique_ptr<ISerialized> serialized(q->apply());

      std::unique_ptr<IDeserializationContext> context(contract.deserialization(g_SchemaName, g_Schema, serialized));

      if (!context->next())
         return false;

      _id = context->uint("Id");
      _greeting = context->string("Greeting");

      return true;
   }

private:
   unsigned int _id;
   std::string _greeting;
};

void main() {

   // Create a package that contains all serialized data. You may want to create
   // it via a factory method instead.
   Package package("data.db");

   // Save a greeting in the package.
   {
      Greeter greeter(1, "Hello World!");

      package.save(greeter);

      std::cout << greeter.greeting() << std::endl;
   }

   // Restore a greeting from the package. We're required to fill identification fields to
   // restore appropriate greeting. Greeting instance is populated when load() is called so
   // the greeting text itself is fetched from the data source.
   {
      Greeter greeter(1);

      package.load(greeter);

      std::cout << greeter.greeting() << std::endl;
   }
}
```
<small>* Methods implemented inline, virtual destructors are omitted and errors are not handled for brevity.</small>
