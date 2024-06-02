#include "Game.hpp"
#include "Enum.hpp"

#include <iostream>

void Game::fillBot()
{
    for (int i = 0; i < MAP_SIZE; i++)
    {
        int xPos = std::rand() % MAP_SIZE;
        int yPos = std::rand() % MAP_SIZE;
        while (bot.field[xPos][yPos] != 0)
        {
            xPos = std::rand() % MAP_SIZE;
            yPos = std::rand() % MAP_SIZE;
        }

        if (isPossibleToPlaceShip(bot.ships[i], xPos, yPos, bot, map2))
        {
            continue;
        }

        std::swap(bot.ships[i].rect.height, bot.ships[i].rect.width);

        if (isPossibleToPlaceShip(bot.ships[i], xPos, yPos, bot, map2))
        {
            continue;
        }
        std::swap(bot.ships[i].rect.height, bot.ships[i].rect.width);
    }

    for (int i = 0; i <= 9; i++)
    {
        if (!bot.ships[i].placedOnField)
        {
            for (int k = 0; k < MAP_SIZE; k++)
            {
                for (int l = 0; l < MAP_SIZE; l++)
                {
                    if (isPossibleToPlaceShip(bot.ships[i], k, l, bot, map2))
                    {
                        k = MAP_SIZE;
                        l = MAP_SIZE;
                        continue;
                    }
                    std::swap(bot.ships[i].rect.height, bot.ships[i].rect.width);
                    if (isPossibleToPlaceShip(bot.ships[i], k, l, bot, map2))
                    {
                        k = MAP_SIZE;
                        l = MAP_SIZE;
                        continue;
                    }

                    std::swap(bot.ships[i].rect.height, bot.ships[i].rect.width);
                }
            }
        }
    }
}

void Game::init()
{
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Battle Ship");

    SetTargetFPS(60);

    map.resize(MAP_SIZE);
    map2.resize(MAP_SIZE);
    for (int i = 0; i < MAP_SIZE; i++)
    {
        map[i].resize(MAP_SIZE);
        map2[i].resize(MAP_SIZE);
    }

    player.init(SCREEN_WIDTH, SCREEN_HEIGHT);
    bot.init(SCREEN_WIDTH, SCREEN_HEIGHT);

    for (int i = 0; i < MAP_SIZE; i++)
    {
        for (int j = 0; j < MAP_SIZE; j++)
        {
            Rectangle pect;
            pect.height = 50;
            pect.width = 50;
            pect.x = SCREEN_WIDTH / 20 + j * 50;
            pect.y = SCREEN_HEIGHT / 8 + i * 50;
            map[i][j] = pect;
            pect.x = SCREEN_WIDTH * 27 / 40 + j * 50;
            pect.y = SCREEN_HEIGHT / 8 + i * 50;
            map2[i][j] = pect;
        }
    }

    fonts.resize(COUNT_FONTS);

    loadFonts();

    fillBot();
}

void Game::close()
{
    CloseWindow();
}

void Game::handle()
{
    switch (currentGlobalState)
    {
    case STATE_SHIP_PLACEMENT:
    {

        mousePoint = GetMousePosition();

        for (int i = 0; i <= 9; i++)
        {
            if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && !player.ships[i].placedOnField)
            {
                if (CheckCollisionPointRec(mousePoint, player.ships[i].rect))
                {
                    player.ships[i].isMove = true;
                    dX = mousePoint.x - player.ships[i].rect.x;
                    dY = mousePoint.y - player.ships[i].rect.y;
                }
            }

            if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT) && !player.ships[i].placedOnField)
            {
                player.ships[i].isMove = false;
                placeShipOnField(player.ships[i], player);
            }

            if (player.ships[i].isMove && !player.ships[i].placedOnField)
            {
                player.ships[i].rect.x = mousePoint.x - dX;
                player.ships[i].rect.y = mousePoint.y - dY;
                if (IsKeyPressed(KEY_SPACE))
                {
                    std::swap(player.ships[i].rect.height, player.ships[i].rect.width);
                }
            }
        }
        break;
    }
    case STATE_BATTLE:
    {
        if (currentBattleState == BATTLE_STATE_PLAYER)
        {
            mousePoint = GetMousePosition();

            int coordY = (mousePoint.x - map2[0][0].x) / 50;
            int coordX = (mousePoint.y - map2[0][0].y) / 50;

            for (int i = 0; i <= 9; i++)
            {

                if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && CheckCollisionPointRec(Vector2{mousePoint.x, mousePoint.y}, bot.ships[i].rect) && bot.alive && bot.field[coordX][coordY] == 1)
                {
                    bot.field[coordX][coordY] = 2;
                    break;
                }
            }
        }
        break;
    }
    }
}

void Game::update()
{

    // switch (currentGlobalState)
    // {
    // case STATE_BATTLE:
    // {
    //     if (currentBattleState == BATTLE_STATE_PLAYER)
    //     {
    //         int temp = 0;

    //         for (int i = 0; i <= 9; i++)
    //         {
    //             temp = 0;
    //             if (bot.ships[i].rect.height >= bot.ships[i].rect.width)
    //             {
    //                 for (int iInd = bot.ships[i].rect.x; iInd <= bot.ships[i].shipParts + iInd - 1; iInd++)
    //                 {
    //                     if (bot.field[iInd][(int)bot.ships[i].rect.y] == 2)
    //                     {
    //                         temp++;
    //                     }
    //                 }
    //             }
    //             else
    //             {
    //                 for (int jInd = bot.ships[i].rect.y; jInd <= bot.ships[i].shipParts + jInd - 1; jInd++)
    //                 {
    //                     if (bot.field[(int)bot.ships[i].rect.x][jInd] == 2)
    //                     {
    //                         temp++;
    //                     }
    //                 }
    //             }

    //             if (temp == bot.ships[i].shipParts)
    //             {
    //                 bot.ships[i].isAfloat = false;
    //                 deleteEverythingDeadShip(bot.ships[i]);
    //             }
    //         }

    //         temp = 0;
    //         for (int i = 0; i <= 9; i++)
    //         {
    //             if (!bot.ships[i].isAfloat)
    //             {
    //                 temp++;
    //             }
    //         }

    //         if (temp >= 10)
    //         {
    //             bot.alive = false;
    //         }
    //     }
    //     break;
    // }
    // }
}

void Game::display()
{
    BeginDrawing();
    ClearBackground(RAYWHITE);

    drawShips(player);

    drawMap(map);

    switch (currentGlobalState)
    {
    case STATE_SHIP_PLACEMENT:
    {
        for (int i = 0; i <= 9; i++)
        {
            if (!player.ships[i].placedOnField)
            {
                inaccessibleAreaInRed(player.ships[i]);
            }
        }

        DrawTextEx(fonts[FONT_PACMAN], "Place the ships on the field", Vector2{SCREEN_WIDTH * 5 / 16, 20}, fonts[FONT_PACMAN].baseSize, 0, Color{0, 0, 0, 255});
        break;
    }
    case STATE_BATTLE:
    {
        DrawTextEx(fonts[FONT_PACMAN], "Battle!", Vector2{SCREEN_WIDTH * 7 / 16, 20}, fonts[FONT_PACMAN].baseSize, 0, Color{0, 0, 0, 255});
        drawMap(map2);
        drawShips(bot);

        for (int i = 0; i < MAP_SIZE; i++)
        {
            for (int j = 0; j < MAP_SIZE; j++)
            {
                if (bot.field[i][j] == 2)
                {
                    DrawRectangleRec(map2[i][j], Color{255, 178, 25, 255});
                }
                else if (bot.field[i][j] == 3)
                {
                    DrawRectangleRec(map2[i][j], Color{255, 0, 0, 255});
                }
            }
        }
        break;
    }
    }

    EndDrawing();
}

void Game::stateTransition()
{
    switch (currentGlobalState)
    {
    case STATE_SHIP_PLACEMENT:
    {
        int temp = 0;
        for (int i = 0; i <= 9; i++)
        {
            if (player.ships[i].placedOnField)
            {
                temp++;
            }
            if (bot.ships[i].placedOnField)
            {
                temp++;
            }
        }

        if (temp >= 20)
        {
            nextState = true;
        }
        break;
    }
    }

    if (nextState)
    {
        nextState = false;
        currentGlobalState = (GameStates)((int)currentGlobalState + 1);
    }
}

Game::Game() {}

Game::~Game() {}

void Game::placeShipOnField(Ship &ship, Player &player)
{
    for (int i = 0; i < MAP_SIZE; i++)
    {
        for (int j = 0; j < MAP_SIZE; j++)
        {
            if (CheckCollisionPointRec(Vector2{ship.rect.x, ship.rect.y}, map[i][j]))
            {
                if (isPossibleToPlaceShip(ship, i, j, player, map))
                {
                    ship.placedOnField = true;
                }
            }
        }
    }
}

bool Game::isPossibleToPlaceShip(Ship &ship, int iInd, int jInd, Player &player, std::vector<std::vector<Rectangle>> &map)
{
    bool success = true;

    for (int i = iInd; i <= iInd + ship.rect.height / 50 - 1; i++)
    {
        for (int j = jInd; j <= jInd + ship.rect.width / 50 - 1; j++)
        {
            if (i < 0 || j < 0 || j >= MAP_SIZE || i >= MAP_SIZE)
            {
                success = false;
            }

            if (player.field[i][j] != 0)
            {
                success = false;
            }

            if (i + 1 < MAP_SIZE && player.field[i + 1][j] != 0)
            {
                success = false;
            }
            if (j + 1 < MAP_SIZE && player.field[i][j + 1] != 0)
            {
                success = false;
            }
            if (i - 1 >= 0 && player.field[i - 1][j] != 0)
            {
                success = false;
            }
            if (j - 1 >= 0 && player.field[i][j - 1] != 0)
            {
                success = false;
            }
            if (i - 1 >= 0 && j - 1 >= 0 && player.field[i - 1][j - 1] != 0)
            {
                success = false;
            }
            if (i - 1 >= 0 && j + 1 < MAP_SIZE && player.field[i - 1][j + 1] != 0)
            {
                success = false;
            }
            if (i + 1 < MAP_SIZE && j - 1 >= 0 && player.field[i + 1][j - 1] != 0)
            {
                success = false;
            }
            if (i + 1 < MAP_SIZE && j + 1 < MAP_SIZE && player.field[i + 1][j + 1] != 0)
            {
                success = false;
            }
        }
    }

    if (success)
    {
        for (int i = iInd; i <= iInd + ship.rect.height / 50 - 1; i++)
        {
            for (int j = jInd; j <= jInd + ship.rect.width / 50 - 1; j++)
            {
                player.field[i][j] = 1;
            }
        }
        ship.placedOnField = true;
        ship.rect.x = map[iInd][jInd].x;
        ship.rect.y = map[iInd][jInd].y;
    }
    return success;
}

void Game::inaccessibleAreaInRed(Ship &ship)
{
    for (int i = 0; i < MAP_SIZE; i++)
    {
        for (int j = 0; j < MAP_SIZE; j++)
        {
            if (CheckCollisionPointRec(Vector2{ship.rect.x, ship.rect.y}, map[i][j]))
            {
                repaintingRed(ship, i, j);
            }
        }
    }
}

void Game::repaintingRed(Ship &ship, int iInd, int jInd)
{
    bool success = true;

    for (int i = iInd; i <= iInd + ship.rect.height / 50 - 1; i++)
    {
        for (int j = jInd; j <= jInd + ship.rect.width / 50 - 1; j++)
        {
            if (i < 0 || j < 0 || j >= MAP_SIZE || i >= MAP_SIZE)
            {
                success = false;
            }

            if (i + 1 < MAP_SIZE && player.field[i + 1][j] != 0)
            {
                success = false;
            }
            if (j + 1 < MAP_SIZE && player.field[i][j + 1] != 0)
            {
                success = false;
            }
            if (i - 1 >= 0 && player.field[i - 1][j] != 0)
            {
                success = false;
            }
            if (j - 1 >= 0 && player.field[i][j - 1] != 0)
            {
                success = false;
            }
            if (i - 1 >= 0 && j - 1 >= 0 && player.field[i - 1][j - 1] != 0)
            {
                success = false;
            }
            if (i - 1 >= 0 && j + 1 < MAP_SIZE && player.field[i - 1][j + 1] != 0)
            {
                success = false;
            }
            if (i + 1 < MAP_SIZE && j - 1 >= 0 && player.field[i + 1][j - 1] != 0)
            {
                success = false;
            }
            if (i + 1 < MAP_SIZE && j + 1 < MAP_SIZE && player.field[i + 1][j + 1] != 0)
            {
                success = false;
            }
        }
    }

    if (!success)
    {
        for (int i = iInd; i <= iInd + ship.rect.height / 50 - 1; i++)
        {
            for (int j = jInd; j <= jInd + ship.rect.width / 50 - 1; j++)
            {
                if (i >= 0 && j >= 0 && i < MAP_SIZE && j < MAP_SIZE)
                {
                    DrawRectangleRec(map[i][j], Color{255, 0, 0, 100});
                }
            }
        }
    }
}

void Game::loadFonts()
{
    fonts[FONT_PACMAN] = LoadFont("assets/Fonts/PAC-FONT.ttf");
}

void Game::drawMap(std::vector<std::vector<Rectangle>> &map)
{
    for (int i = 0; i < MAP_SIZE; i++)
    {
        for (int j = 0; j < MAP_SIZE; j++)
        {
            DrawRectangleLinesEx(map[i][j], 1.5, Color{0, 0, 0, 255});
        }
    }

    for (int j = 0; j <= 9; j++)
    {
        std::string tempString;
        DrawText(std::to_string(j + 1).c_str(), map[0][j].x + 12, map[0][j].y - 45, 42, Color{0, 0, 0, 255});
        switch ('A' + j)
        {
        case 'A':
        {
            tempString = "A";
            break;
        }
        case 'B':
        {
            tempString = "B";
            break;
        }
        case 'C':
        {
            tempString = "C";
            break;
        }
        case 'D':
        {
            tempString = "D";
            break;
        }
        case 'E':
        {
            tempString = "E";
            break;
        }
        case 'F':
        {
            tempString = "F";
            break;
        }
        case 'G':
        {
            tempString = "G";
            break;
        }
        case 'H':
        {
            tempString = "H";
            break;
        }
        case 'I':
        {
            tempString = "A";
            break;
        }
        case 'J':
        {
            tempString = "J";
            break;
        }
        }
        DrawText(tempString.c_str(), map[j][0].x - 30, map[j][0].y + 3, 42, Color{0, 0, 0, 255});
    }
}

void Game::drawShips(Player &player)
{
    for (int i = 0; i <= 9; i++)
    {
        Rectangle tempRect = player.ships[i].rect;
        DrawRectangleRec(tempRect, Color{0, 0, 0, 255});
    }
}

void Game::deleteEverythingDeadShip(Ship &ship)
{

    int iInd = ship.rect.x;

    int jInd = ship.rect.y;

    int iIndEnd = iInd;
    int jIndEnd = jInd;

    if (ship.rect.height >= ship.rect.width)
    {
        iIndEnd = ship.shipParts + iInd - 1;
    }
    else
    {
        jIndEnd = ship.shipParts + jInd - 1;
    }

    for (int i = iInd; i <= iIndEnd; i++)
    {
        for (int j = jInd; j <= jIndEnd; j++)
        {

            bot.field[i][j] == 3;

            if (i - 1 >= 0)
            {
                bot.field[i - 1][j] = 3;
            }

            if (j - 1 >= 0)
            {
                bot.field[i][j - 1] = 3;
            }
            if (i + 1 < MAP_SIZE)
            {
                bot.field[i + 1][j] = 3;
            }
            if (j + 1 < MAP_SIZE)
            {
                bot.field[i][j + 1] = 3;
            }

            if (i - 1 >= 0 && j - 1 >= 0)
            {
                bot.field[i - 1][j - 1] = 3;
            }
            if (i - 1 >= 0 && j + 1 < MAP_SIZE)
            {
                bot.field[i - 1][j + 1] = 3;
            }
            if (i + 1 < MAP_SIZE && j + 1 < MAP_SIZE)
            {
                bot.field[i + 1][j + 1] = 3;
            }
            if (i + 1 < MAP_SIZE && j - 1 >= 0)
            {
                bot.field[i + 1][j - 1] = 3;
            }
        }
    }
}