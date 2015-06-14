include_directories(seril)

add_library(seril STATIC

   serializationexception.cpp

   column.cpp
   binarycolumn.cpp
   numbercolumn.cpp
   integercolumn.cpp
   stringcolumn.cpp

   sqlitedatacontract.cpp
   sqliteserializationcontext.cpp
   sqlitedeserializationcontext.cpp
   sqlitequerycontext.cpp
   sqliteserialized.cpp
)
