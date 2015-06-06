#pragma once
#include "Iserialized.hpp"
#include "sqliteserializationcontext.hpp"

namespace seril {

   class SQLiteSerialized : public ISerialized {
   public:
      SQLiteSerialized(SQLiteSerializationContext::Binds&& binds);
      virtual ~SQLiteSerialized();

      const SQLiteSerializationContext::Binds& binds() const;

   private:
      SQLiteSerializationContext::Binds _binds;
   };

}
