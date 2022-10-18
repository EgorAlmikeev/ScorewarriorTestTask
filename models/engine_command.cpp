#include "engine_command.hpp"
#include "game.hpp"
#include "unit.hpp"

#include <chrono>
#include <math.h>
#include <thread>

std::string EngineCommand::Utils::CommandTypeToString( const Type value )
{
   switch( value )
   {
   case EngineCommand::Type::CREATE_MAP:
      return "CREATE_MAP";
   case EngineCommand::Type::SPAWN:
      return "SPAWN";
   case EngineCommand::Type::MARCH:
      return "MARCH";
   case EngineCommand::Type::WAIT:
      return "WAIT";
   case EngineCommand::Type::FINISH:
      return "FINISH";
   }
}

void EngineCommand::DefaultExecute( Game& game )
{
   std::this_thread::sleep_for( std::chrono::seconds( game.SINGLE_TICK_TIME ) );
}

void EngineCommandCreateMap::Execute( Game& game )
{
   DefaultExecute( game );

   game.mWorldSize = Game::WorldSize{ mWorldWidth, mWorldHeight };
   Done( game );
}

void EngineCommandCreateMap::Done( Game& game )
{
   mDone = true;
   std::cout << "[" << game.mCurrentTickNumber << "] "
             << "MAP CREATED " << mWorldWidth << " " << mWorldHeight << std::endl;
}

void EngineCommandSpawn::Execute( Game& game )
{
   DefaultExecute( game );

   if( mUnitX >= 0 && mUnitY >= 0 && mUnitX <= game.mWorldSize.mWidth && mUnitY <= game.mWorldSize.mHeight )
      game.mUnits.emplace( mUnitId, Unit( mUnitId, mUnitType, Unit::Position{ mUnitX, mUnitY }, mUnitStrength ) );

   Done( game );
}

void EngineCommandSpawn::Done( Game& game )
{
   mDone = true;
   std::cout << "[" << game.mCurrentTickNumber << "] " << Unit::Utils::UnitTypeToString( mUnitType ) << " SPAWNED "
             << mUnitId << " " << mUnitX << " " << mUnitY << std::endl; // дописать силу
}

void EngineCommandMarch::Execute( Game& game )
{
   DefaultExecute( game );

   if( mFirstCall )
      Start( game );
   else if( mTimeLeft )
      Continue( game );
   else
      Finish( game );
}

void EngineCommandMarch::Done( Game& game )
{
   mDone = true;
}

void EngineCommandMarch::Start( Game& game )
{
   if( auto unit_iter = game.mUnits.find( mUnitId ); unit_iter != game.mUnits.end() )
   {
      auto marching_unit = unit_iter->second;

      std::cout << "[" << game.mCurrentTickNumber << "] "
                << "MARCH STARTED " << mUnitId << " TO " << mTargetX << " " << mTargetY << std::endl;

      mTimeLeft = sqrt( pow( ( mTargetX - marching_unit.mPosition.mX ), 2 ) +
                        pow( ( mTargetY - marching_unit.mPosition.mY ), 2 ) );
      mFirstCall = false;
   }
   else
   {
      Done( game );
   }
}

void EngineCommandMarch::Continue( Game& game )
{
   mTimeLeft--;
}

void EngineCommandMarch::Finish( Game& game )
{
   if( auto unit_iter = game.mUnits.find( mUnitId ); unit_iter != game.mUnits.end() )
   {
      unit_iter->second.mPosition = Unit::Position{ mTargetX, mTargetY };

      std::cout << "[" << game.mCurrentTickNumber << "] "
                << "MARCH " << mUnitId << " FINISHED " << mTargetX << " " << mTargetY;

      Fight( game );
   }

   Done( game );
}

void EngineCommandMarch::Fight( Game& game )
{
   if( auto marching_unit_iter = game.mUnits.find( mUnitId ); marching_unit_iter != game.mUnits.end() )
   {
      auto marching_unit = marching_unit_iter->second;

      auto staying_unit_iter = std::find_if( game.mUnits.begin(), game.mUnits.end(),
                                             [ &marching_unit ]( std::pair< const int, Unit >& kv )
                                             {
                                                return kv.second.mId != marching_unit.mId &&
                                                       kv.second.mPosition.mX == marching_unit.mPosition.mX &&
                                                       kv.second.mPosition.mY == marching_unit.mPosition.mY;
                                             } );

      if( staying_unit_iter != game.mUnits.end() )
      {
         auto staying_unit = staying_unit_iter->second;

         std::cout << " BATTLE " << marching_unit.mId << " " << staying_unit.mId;

         if( marching_unit.mStrength > staying_unit.mStrength )
         {
            game.mUnits.erase( staying_unit_iter );
            std::cout << " WINNER IS " << marching_unit.mId;
         }
         else if( marching_unit.mStrength < staying_unit.mStrength )
         {
            game.mUnits.erase( marching_unit_iter );
            std::cout << " WINNER IS " << staying_unit.mId;
         }
         else
         {
            game.mUnits.erase( staying_unit_iter );
            game.mUnits.erase( marching_unit_iter );
            std::cout << " ALL DEAD";
         }
      }
   }

   std::cout << std::endl;
}

void EngineCommandWait::Execute( Game& game )
{
   DefaultExecute( game );

   if( !mTicksLeft-- )
      Done( game );
}

void EngineCommandWait::Done( Game& game )
{
   mDone = true;
}
