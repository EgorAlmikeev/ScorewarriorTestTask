#include "game.hpp"
#include "engine_command.hpp"
#include "unit.hpp"

#include <iostream>

const int Game::SINGLE_TICK_TIME = 1;

void Game::Start()
{
   bool all_commands_done = false;

   while( !all_commands_done )
   {
      all_commands_done = true;

      for( auto current_command_iter = mCommands.begin(); current_command_iter != mCommands.end();
           current_command_iter++ )
      {
         const auto current_command = *current_command_iter;

         if( !current_command->mDone )
         {
            all_commands_done = false;
            current_command->Execute( *this );

            if( current_command->mType == EngineCommand::Type::WAIT )
            {
               current_command_iter = mCommands.begin();
               break;
            }
         }
      }

      ++mCurrentTickNumber;
   }

   std::cout << "[" << mCurrentTickNumber << "]"
             << "FINISH" << std::endl;
}
