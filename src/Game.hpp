#pragma once
#include "raylib.h"

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

    void placeShipOnField(Ship &, Player &);
    bool isPossibleToPlaceShip(Ship &, int, int, Player &, std::vector<std::vector<Rectangle>> &map);
    void inaccessibleAreaInRed(Ship &);
    void repaintingRed(Ship &, int, int);
    void deleteEverythingDeadShip(Ship &);

    void drawMap(std::vector<std::vector<Rectangle>> &);
    void drawShips(Player &);

    void loadFonts();

    void fillBot();

    // Rectangle map[10][10];
    // Rectangle map2[10][10];

    std::vector<std::vector<Rectangle>> map;
    std::vector<std::vector<Rectangle>> map2;

private:
    static const int SCREEN_WIDTH = 1920;
    static const int SCREEN_HEIGHT = 1080;
    static const int MAP_SIZE = 10;
    static const int COUNT_FONTS = 1;
    std::vector<Font> fonts;
    bool nextState = false;

    Player player = Player{};
    Player bot = Player{};

    GameStates currentGlobalState = STATE_SHIP_PLACEMENT;
    BattleStates currentBattleState = BATTLE_STATE_PLAYER;

    // Mouse
    Vector2 mousePoint;
    bool isMove = false;
    int dX = 0, dY = 0;
};