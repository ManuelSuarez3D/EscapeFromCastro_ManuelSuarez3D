#include <SFML/Graphics.hpp>
#include "GameEngine.h"
int main()
{
    GameEngine game("../config.txt");
    game.run();

    return 0;
}