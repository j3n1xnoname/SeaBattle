#include "raylib.h"

#include <ctime>
#include <cstdlib>

#include "Game.hpp"

int main()
{
    std::srand(std::time(0));
    Game *game = new Game;

    game->init();

    while (!WindowShouldClose())
    {
        game->handle();

        game->update();

        game->display();

        game->stateTransition();
    }

    game->close();
    return 0;
}