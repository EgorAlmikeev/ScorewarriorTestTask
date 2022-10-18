#pragma once

#include <iostream>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

#include "unit.hpp"

class EngineCommand;

class Game
{
public:
   struct WorldSize
   {
      int mWidth{ 0 }, mHeight{ 0 };
   };

   Game( const std::vector< std::shared_ptr< EngineCommand > >& commands )
      : mCommands( commands )
   {
   }

   void Start();
   void Print();

   WorldSize mWorldSize{ WorldSize{ 0, 0 } };
   std::unordered_map< int, Unit > mUnits;
   std::vector< std::shared_ptr< EngineCommand > > mCommands;
   long int mCurrentTickNumber{ 0 };

   static const int SINGLE_TICK_TIME;
};
