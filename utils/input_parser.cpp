#include <fstream>
#include <iostream>
#include <optional>
#include <sstream>
#include <string>

#include "../models/engine_command.hpp"
#include "input_parser.hpp"

std::vector< std::shared_ptr< EngineCommand > >
InputParser::ParseScriptFile( const std::string& script_path ) noexcept( false )
{
   std::ifstream script_file( script_path );
   std::string script_line;
   std::vector< std::shared_ptr< EngineCommand > > result;

   while( std::getline( script_file, script_line ) )
   {
      if( auto parsed_command = ParseScriptLine( script_line ); parsed_command != nullptr )
         result.emplace_back( parsed_command );
   }

   return result;
}

std::shared_ptr< EngineCommand > InputParser::ParseScriptLine( const std::string& script_line ) noexcept( false )
{
   std::stringstream command_ss;
   std::optional< EngineCommand::Type > command_type;
   std::vector< int > command_arguments;

   for( const auto& symbol : script_line )
   {
      if( !command_type.has_value() && EOFSymbol( symbol ) )
      {
         command_type = CommandTypeFromString( command_ss.str() );
         command_ss.str( std::string() );
         command_ss.clear();
         continue;
      }

      if( command_type.has_value() && EOFSymbol( symbol ) )
      {
         int command_argument = -1;
         std::string int_buffer;
         command_ss >> int_buffer;

         if( std::stringstream( int_buffer ) >> command_argument )
            if( command_argument != -1 )
               command_arguments.emplace_back( command_argument );

         command_ss.str( std::string() );
         command_ss.clear();
         continue;
      }

      command_ss << symbol;
   }

   return command_type.has_value() ? CreateCommand( command_type.value(), command_arguments ) : nullptr;
}

EngineCommand::Type InputParser::CommandTypeFromString( const std::string& command_type_string ) noexcept( false )
{
   if( command_type_string == "CREATE_MAP" )
      return EngineCommand::Type::CREATE_MAP;
   if( command_type_string == "SPAWN" )
      return EngineCommand::Type::SPAWN;
   if( command_type_string == "MARCH" )
      return EngineCommand::Type::MARCH;
   if( command_type_string == "WAIT" )
      return EngineCommand::Type::WAIT;
   if( command_type_string == "FINISH" )
      return EngineCommand::Type::FINISH;

   throw UnknownScriptCommandException();
}

bool InputParser::EOFSymbol( const char& s )
{
   return s == ' ' || s == '\n' || s == '\0' || s == '\r';
}

std::shared_ptr< EngineCommand > InputParser::CreateCommand( const EngineCommand::Type& command_type,
                                                             std::vector< int > command_arguments )
{
   switch( command_type )
   {
   case EngineCommand::Type::CREATE_MAP:
   {
      if( command_arguments.size() == 2 )
         return CreateCommandCreateMap( command_arguments.at( 0 ), command_arguments.at( 1 ) );
   }
   case EngineCommand::Type::SPAWN:
   {
      if( command_arguments.size() == 4 )
         return CreateCommandSpawn( command_arguments.at( 0 ), command_arguments.at( 1 ), command_arguments.at( 2 ),
                                    command_arguments.at( 3 ) );
   }
   case EngineCommand::Type::MARCH:
      if( command_arguments.size() == 3 )
         return CreateCommandMarch( command_arguments.at( 0 ), command_arguments.at( 1 ), command_arguments.at( 2 ) );
   case EngineCommand::Type::WAIT:
      if( command_arguments.size() == 1 )
         return CreateCommandWait( command_arguments.at( 0 ) );
   case EngineCommand::Type::FINISH:;
   }

   return nullptr;
}

std::shared_ptr< EngineCommandCreateMap > InputParser::CreateCommandCreateMap( int w, int h )
{
   return std::shared_ptr< EngineCommandCreateMap >( new EngineCommandCreateMap( w, h ) );
}

std::shared_ptr< EngineCommandSpawn > InputParser::CreateCommandSpawn( int id, int x, int y, int strength,
                                                                       Unit::Type type )
{
   return std::shared_ptr< EngineCommandSpawn >( new EngineCommandSpawn( id, type, x, y, strength ) );
}

std::shared_ptr< EngineCommandMarch > InputParser::CreateCommandMarch( int id, int x, int y )
{
   return std::shared_ptr< EngineCommandMarch >( new EngineCommandMarch( id, x, y ) );
}

std::shared_ptr< EngineCommandWait > InputParser::CreateCommandWait( int ticks )
{
   return std::shared_ptr< EngineCommandWait >( new EngineCommandWait( ticks ) );
}

std::shared_ptr< InputParser > InputParser::Instance()
{
   static std::shared_ptr< InputParser > instance( new InputParser() );
   return instance;
}

std::vector< std::shared_ptr< EngineCommand > > InputParser::ParseArgv( int argc, char* argv[] )
{
   if( argc > 1 )
      return ParseScriptFile( argv[ 1 ] );
   else
      return {};
}
