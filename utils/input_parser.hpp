#pragma once

#include <vector>

#include "../models/engine_command.hpp"

class InputParser
{
   InputParser() = default;

   std::vector< std::shared_ptr< EngineCommand > > ParseScriptFile( const std::string& script_path ) noexcept( false );
   std::shared_ptr< EngineCommand > ParseScriptLine( const std::string& script_line ) noexcept( false );
   EngineCommand::Type CommandTypeFromString( const std::string& command_type_string ) noexcept( false );

   bool EOFSymbol( const char& s );

   std::shared_ptr< EngineCommand > CreateCommand( const EngineCommand::Type& command_type,
                                                   std::vector< int > command_arguments );

   std::shared_ptr< EngineCommandCreateMap > CreateCommandCreateMap( int w, int h );
   std::shared_ptr< EngineCommandSpawn > CreateCommandSpawn( int id, int x, int y, int strength,
                                                             Unit::Type type = Unit::Type::WARRIOR );
   std::shared_ptr< EngineCommandMarch > CreateCommandMarch( int id, int x, int y );
   std::shared_ptr< EngineCommandWait > CreateCommandWait( int ticks );

public:
   static std::shared_ptr< InputParser > Instance();
   std::vector< std::shared_ptr< EngineCommand > > ParseArgv( int argc, char* argv[] );

   class UnknownScriptCommandException : public std::exception
   {
      const char* what() throw()
      {
         return "[InputParser] Unknown script command";
      }
   };
};
