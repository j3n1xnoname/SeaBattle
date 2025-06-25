#pragma once
#include "raylib.h"
#include "SDL_mixer.h"
#include "SDL.h"

#include "Enum.hpp"
#include "Player.hpp"

class Game
{
public:
    Game();
    ~Game();
    void init();
    void close();
    void handle();
    void update();
    void display();
    void stateTransition();
    void playSounds();

    void placeShipOnField(Ship &, Player &);
    bool isPossibleToPlaceShip(Ship &, int, int, Player &, std::vector<std::vector<Rectangle>> &map, bool botPlacement = false);
    void inaccessibleAreaInRed(Ship &);
    void repaintingRed(Ship &, int, int);
    void deleteEverythingDeadShip(Ship &, Player &, std::vector<std::vector<Rectangle>> &);
    void updateStateBattle(Player &, std::vector<std::vector<Rectangle>> &);
    void markDeadPLayer(Player &);

    void drawMap(std::vector<std::vector<Rectangle>> &);
    void drawShips(Player &, bool hideShips = false);
    void drawImages(Player &, std::vector<std::vector<Rectangle>> &);
    void drawRemainingShips();

    void loadFonts();
    void loadTextures();
    void loadMusics();

    int findValue(std::pair<int, int> value);

    void fillBot();

    std::vector<std::vector<Rectangle>> map;
    std::vector<std::vector<Rectangle>> map2;

    std::vector<Mix_Music *> musics;
    std::vector<Mix_Chunk *> chunks;

private:
    static const int SCREEN_WIDTH = 1920;
    static const int SCREEN_HEIGHT = 1080;
    static const int MAP_SIZE = 10;
    static const int COUNT_FONTS = 1;
    static const int COUNT_IMAGES = 7;
    static const int COUNT_TEXTURES = 7;
    static const int COUNT_MUSICS = 1;
    static const int COUNT_CHUNKS = 4;
    std::vector<Font> fonts;
    std::vector<Image> images;
    std::vector<Texture2D> textures;
    bool nextState = false;
    bool nextBattleState = false;

    Player player = Player{};
    Player bot = Player{};

    GameStates currentGlobalState = STATE_SHIP_PLACEMENT;
    BattleStates currentBattleState = BATTLE_STATE_PLAYER;

    // Mouse
    Vector2 mousePoint;
    bool isMove = false;
    int dX = 0, dY = 0;

    // bot
    int iCurrentMove, jCurrentMove;
    int iPrevMove = -1, jPrevMove = -1;
    bool gotHit = false;
    std::vector<std::pair<int, int>> emptyCellsBot;

    // Sounds

    bool shout = false;
    bool projectileInShip = false;
    bool explosion = false;
};