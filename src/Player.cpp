#include "Player.hpp"

Player::Player()
{
    for (int i = 0; i < SIZE_FIELD; i++)
    {
        for (int j = 0; j < SIZE_FIELD; j++)
        {
            field[i][j] = 0;
        }
    }
}

Player::~Player() {}

void Player::init(const int SCREEN_WIDTH, const int SCREEN_HEIGHT)
{
    ships.resize(10);
    for (int i = 0; i <= 3; i++)
    {
        ships[i].shipParts = 1;
        Rectangle tempRect;
        tempRect.height = 50;
        tempRect.width = 50;
        tempRect.x = SCREEN_WIDTH * 7 / 15 + i * tempRect.width * 3 / 2;
        tempRect.y = SCREEN_HEIGHT * 2 / 16;
        ships[i].rect = tempRect;
    }

    for (int i = 4; i <= 6; i++)
    {
        ships[i].shipParts = 2;
        Rectangle tempRect;
        tempRect.height = 100;
        tempRect.width = 50;
        tempRect.x = SCREEN_WIDTH * 7 / 15 + (i - 4) * tempRect.width * 3 / 2;
        tempRect.y = SCREEN_HEIGHT * 4 / 16;
        ships[i].rect = tempRect;
    }

    for (int i = 7; i <= 8; i++)
    {
        ships[i].shipParts = 3;
        Rectangle tempRect;
        tempRect.height = 150;
        tempRect.width = 50;
        tempRect.x = SCREEN_WIDTH * 7 / 15 + (i - 7) * tempRect.width * 3 / 2;
        tempRect.y = SCREEN_HEIGHT * 7 / 16;
        ships[i].rect = tempRect;
    }

    for (int i = 9; i <= 9; i++)
    {
        ships[i].shipParts = 4;
        Rectangle tempRect;
        tempRect.height = 200;
        tempRect.width = 50;
        tempRect.x = SCREEN_WIDTH * 7 / 15 + (i - 9) * tempRect.width * 3 / 2;
        tempRect.y = SCREEN_HEIGHT * 11 / 16;
        ships[i].rect = tempRect;
    }
}