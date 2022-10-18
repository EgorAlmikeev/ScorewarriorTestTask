#pragma once

#include <sstream>
#include <string>

struct Unit
{
   enum Type
   {
      WARRIOR
   };

   struct Position
   {
      int mX{ 0 }, mY{ 0 };
   };

   struct Utils
   {
      static std::string UnitTypeToString( const Unit::Type& value )
      {
         switch( value )
         {
         case Unit::Type::WARRIOR:
            return "WARRIOR";
         }
      }
   };

   int mId{ -1 }, mStrength{ -1 };
   Type mType{ Type::WARRIOR };
   Position mPosition{ Position{ 0, 0 } };

   Unit( int id, Type type, Position position, int strenght )
      : mId( id )
      , mType( type )
      , mPosition( position )
      , mStrength( strenght ){};

   std::string ToString() const
   {
      std::stringstream string_stream;

      string_stream << "Unit { id:" << mId << ", type: " << Utils::UnitTypeToString( mType ) << ", position: ["
                    << mPosition.mX << ", " << mPosition.mY << "] }";

      return string_stream.str();
   }
};
