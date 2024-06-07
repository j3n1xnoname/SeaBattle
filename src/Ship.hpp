#pragma once
#include "raylib.h"

struct Ship
{
public:
    Ship();
    ~Ship();

    int shipParts;
    Rectangle rect;
    bool isAfloat;
    bool placedOnField;
    bool isMove;
    bool verticalPos;
    bool isHitted;
};