#pragma once

#include <iostream>
#include <string>

#include "unit.hpp"

class Game;

class EngineCommand
{
public:
   enum Type
   {
      CREATE_MAP,
      SPAWN,
      MARCH,
      WAIT,
      FINISH
   };

   struct Utils
   {
      static std::string CommandTypeToString( const EngineCommand::Type value );
   };

   EngineCommand( Type type )
      : mType( type )
   {
   }

   virtual ~EngineCommand() = default;

   void DefaultExecute( Game& game );
   virtual void Execute( Game& game ) = 0;
   virtual void Done( Game& game ) = 0;

   bool mDone{ false };

   Type mType{ FINISH };
};

class EngineCommandCreateMap : public EngineCommand
{
   int mWorldWidth{ -1 }, mWorldHeight{ -1 };

public:
   EngineCommandCreateMap( const int width, const int height )
      : EngineCommand( EngineCommand::Type::CREATE_MAP )
      , mWorldWidth( width )
      , mWorldHeight( height )
   {
   }

   void Execute( Game& game ) override;
   void Done( Game& game ) override;
};

class EngineCommandSpawn : public EngineCommand
{
   int mUnitId{ -1 }, mUnitX{ -1 }, mUnitY{ -1 }, mUnitStrength{ -1 };
   Unit::Type mUnitType{ Unit::Type::WARRIOR };

public:
   EngineCommandSpawn( const int id, const Unit::Type type, const int x, const int y, int strength )
      : EngineCommand( EngineCommand::Type::SPAWN )
      , mUnitId( id )
      , mUnitX( x )
      , mUnitY( y )
      , mUnitType( type )
      , mUnitStrength( strength )
   {
   }

   void Execute( Game& game ) override;
   void Done( Game& game ) override;
};

class EngineCommandMarch : public EngineCommand
{
   int mUnitId{ -1 }, mTargetX{ -1 }, mTargetY{ -1 }, mTimeLeft{ -1 };
   bool mFirstCall{ true };

public:
   EngineCommandMarch( const int unit_id, const int target_x, const int target_y )
      : EngineCommand( EngineCommand::Type::MARCH )
      , mTargetX( target_x )
      , mTargetY( target_y )
      , mUnitId( unit_id )
   {
   }

   void Execute( Game& game ) override;
   void Done( Game& game ) override;
   void Start( Game& game );
   void Continue( Game& game );
   void Finish( Game& game );
   void Fight( Game& game );
};

class EngineCommandWait : public EngineCommand
{
   int mTicksLeft{ -1 };

public:
   EngineCommandWait( const int ticks )
      : EngineCommand( EngineCommand::Type::WAIT )
      , mTicksLeft( ticks )
   {
   }

   void Execute( Game& game ) override;
   void Done( Game& game ) override;
};
