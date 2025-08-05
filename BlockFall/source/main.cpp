#include <iostream>

#include "Config/Config.h"
#include "Game/Game.h"

int main(int argc, char* argv[]) {

    Config::getInstance().loadConfig(argv[0]);

    if (Config::getInstance().getConfigData().hasError)
    {
        std::cerr << "Configuration file has an error!" << std::endl;
        return 1;
    }

    Game game;
    if (game.init())
    {
        game.run();
    }
    game.shutdown();
 
    return 0;
}
