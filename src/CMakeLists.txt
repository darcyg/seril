include_directories(seril)

add_library(seril STATIC

   serializationexception.cpp
   sqlitedatacontract.cpp
   sqliteserializationcontext.cpp
   sqlitedeserializationcontext.cpp
   sqlitequerycontext.cpp
   sqliteserialized.cpp
)
