#include "raylib.h"
#include "SDL2/SDL_mixer.h"

#include <ctime>
#include <cstdlib>
#include <iostream>

#include "Game.hpp"
#include "Enum.hpp"

int main()
{
    std::srand(std::time(0));
    Game *game = new Game;

    game->init();

    Mix_PlayMusic(game->musics[MUSIC_SEA], -1);

    while (!WindowShouldClose())
    {
        game->handle();

        game->update();

        game->playSounds();

        game->display();

        game->stateTransition();
    }

    Mix_HaltMusic();
    game->close();
    return 0;
}