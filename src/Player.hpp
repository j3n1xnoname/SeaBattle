#pragma once
#include <vector>

#include "Ship.hpp"

struct Player
{
public:
    Player();
    ~Player();
    void init(const int SCREEN_WIDTH, const int SCREEN_HEIGHT);
    static const int SIZE_FIELD = 10;
    int field[SIZE_FIELD][SIZE_FIELD];
    bool alive = true;
    std::vector<Ship> ships;
};