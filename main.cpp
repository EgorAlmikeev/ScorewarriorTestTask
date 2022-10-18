#include <iostream>
#include <memory>
#include <queue>
#include <vector>

#include "models/engine_command.hpp"
#include "models/game.hpp"
#include "models/unit.hpp"
#include "utils/input_parser.hpp"

int main( int argc, char* argv[] )
{
   Game game( InputParser::Instance()->ParseArgv( argc, argv ) );
   game.Start();
   return 0;
}
