#pragma once
#include "raylib.h"

struct Ship
{
public:
    Ship();
    ~Ship();
    void init();

    int shipParts;
    Rectangle rect;
    bool isAfloat;
    bool placedOnField;
    bool isMove;
};