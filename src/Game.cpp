#include "Game.hpp"
#include "Enum.hpp"

#include <iostream>
#include <string>

void Game::fillBot()
{
    for (int i = 0; i <= 9;)
    {
        int ind = rand() % emptyCellsBot.size();
        int xPos = emptyCellsBot[ind].first;
        int yPos = emptyCellsBot[ind].second;
        if (isPossibleToPlaceShip(bot.ships[i], xPos, yPos, bot, map2, true))
        {
            i++;
            continue;
        }
        std::swap(bot.ships[i].rect.height, bot.ships[i].rect.width);
        bot.ships[i].verticalPos = false;
        if (isPossibleToPlaceShip(bot.ships[i], xPos, yPos, bot, map2, true))
        {
            i++;
            continue;
        }
        std::swap(bot.ships[i].rect.height, bot.ships[i].rect.width);
        bot.ships[i].verticalPos = true;
    }
}

void Game::init()
{
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Battle Ship");

    SetTargetFPS(60);

    Mix_Init(MIX_INIT_MP3 | MIX_INIT_WAVPACK);

    Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048);

    map.resize(MAP_SIZE);
    map2.resize(MAP_SIZE);
    for (int i = 0; i < MAP_SIZE; i++)
    {
        map[i].resize(MAP_SIZE);
        map2[i].resize(MAP_SIZE);
    }

    for (int i = 0; i < MAP_SIZE; i++)
    {
        for (int j = 0; j < MAP_SIZE; j++)
        {
            emptyCellsBot.push_back({i, j});
        }
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
    images.resize(COUNT_TEXTURES);
    textures.resize(COUNT_TEXTURES);
    musics.resize(COUNT_MUSICS);
    chunks.resize(COUNT_CHUNKS);

    loadFonts();
    loadTextures();
    loadMusics();

    fillBot();

    emptyCellsBot.clear();
    // fill emptyCellsBot
    for (int i = 0; i < MAP_SIZE; i++)
    {
        for (int j = 0; j < MAP_SIZE; j++)
        {
            if (bot.field[i][j] == 0 || bot.field[i][j] == 1)
            {
                emptyCellsBot.push_back({i, j});
            }
        }
    }
}

void Game::close()
{
    for (int i = 0; i < COUNT_FONTS; i++)
    {
        UnloadFont(fonts[i]);
    }

    for (int i = 0; i < COUNT_IMAGES; i++)
    {
        UnloadImage(images[i]);
    }

    for (int i = 0; i < COUNT_TEXTURES; i++)
    {
        UnloadTexture(textures[i]);
    }

    for (int i = 0; i < COUNT_MUSICS; i++)
    {
        Mix_FreeMusic(musics[i]);
        musics[i] = nullptr;
    }
    for (int i = 0; i < COUNT_CHUNKS; i++)
    {
        Mix_FreeChunk(chunks[i]);
        chunks[i] = nullptr;
    }

    CloseWindow();
    Mix_Quit();
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
                    if (player.ships[i].verticalPos)
                    {
                        player.ships[i].verticalPos = false;
                    }
                    else
                    {
                        player.ships[i].verticalPos = true;
                    }
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

            int temp = 0;
            for (int i = 0; i < MAP_SIZE; i++)
            {
                for (int j = 0; j < MAP_SIZE; j++)
                {
                    if (CheckCollisionPointRec(Vector2{mousePoint.x, mousePoint.y}, map2[i][j]))
                    {
                        temp++;
                    }
                }
            }

            bool flag = false;
            if (temp > 0)
                flag = true;

            for (int i = 0; i <= 9; i++)
            {

                if (flag && IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && CheckCollisionPointRec(Vector2{mousePoint.x, mousePoint.y}, bot.ships[i].rect) && bot.alive && bot.field[coordX][coordY] == 1)
                {

                    bot.field[coordX][coordY] = 2;
                    shout = true;
                    explosion = true;

                    gotHit = true;
                    break;
                }
            }

            if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && bot.field[coordX][coordY] == 0 && flag)
            {
                bot.field[coordX][coordY] = 4;
                shout = true;
                nextBattleState = true;
            }
        }
        else if (currentBattleState == BATTLE_STATE_BOT)
        {
            /*
            int iCurrentMove, jCurrentMove;
            int iPrevMove = -1, jPrevMove = -1;
            */
            int ind = rand() % emptyCellsBot.size();
            iCurrentMove = emptyCellsBot[ind].first;
            jCurrentMove = emptyCellsBot[ind].second;
            if (player.field[iCurrentMove][jCurrentMove] == 1)
            {
                player.field[iCurrentMove][jCurrentMove] = 2;
                gotHit = true;
                projectileInShip = true;
                explosion = true;
            }
            else if (player.field[iCurrentMove][jCurrentMove] == 0)
            {
                player.field[iCurrentMove][jCurrentMove] = 4;
                nextBattleState = true;
                projectileInShip = true;
            }

            emptyCellsBot.erase(emptyCellsBot.begin() + ind);
        }
        break;
    }
    case STATE_WINNER:
    {
        break;
    }
    }
}

void Game::update()
{

    switch (currentGlobalState)
    {
    case STATE_BATTLE:
    {

        updateStateBattle(player, map);

        updateStateBattle(bot, map2);

        markDeadPLayer(player);
        markDeadPLayer(bot);

        break;
    }
    }
}

void Game::display()
{

    BeginDrawing();
    ClearBackground(RAYWHITE);

    drawMap(map);
    drawShips(player);

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

        DrawText("Press enter to flip a ship", SCREEN_WIDTH * 2 / 30, SCREEN_HEIGHT * 5 / 8, 32, Color{0, 0, 0, 255});
        DrawText("(when holding it with the mouse)", SCREEN_WIDTH * 2 / 30, SCREEN_HEIGHT * 11 / 16, 32, Color{0, 0, 0, 255});

        DrawTextEx(fonts[FONT_PACMAN], "Place the ships on the field", Vector2{SCREEN_WIDTH * 5 / 16, 20}, fonts[FONT_PACMAN].baseSize, 0, Color{0, 0, 0, 255});
        break;
    }
    case STATE_BATTLE:
    {
        drawRemainingShips();
        if (currentBattleState == BATTLE_STATE_PLAYER)
        {
            DrawTextEx(fonts[FONT_PACMAN], "Player's move...", Vector2{SCREEN_WIDTH * 13 / 32, SCREEN_HEIGHT * 7 / 16}, fonts[FONT_PACMAN].baseSize, 0, Color{0, 0, 0, 255});
        }
        else if (currentBattleState == BATTLE_STATE_BOT)
        {
            DrawTextEx(fonts[FONT_PACMAN], "Bot's move...", Vector2{SCREEN_WIDTH * 13 / 32, SCREEN_HEIGHT * 7 / 16}, fonts[FONT_PACMAN].baseSize, 0, Color{0, 0, 0, 255});
        }
        DrawTextEx(fonts[FONT_PACMAN], "Battle!", Vector2{SCREEN_WIDTH * 7 / 16, 20}, fonts[FONT_PACMAN].baseSize, 0, Color{0, 0, 0, 255});
        drawMap(map2);
        drawShips(bot, true);

        drawImages(bot, map2);

        drawImages(player, map);

        break;
    }
    case STATE_WINNER:
    {
        drawRemainingShips();
        std::string tempString;
        if (player.alive)
        {
            tempString = "Player WINS!";
        }
        else
        {
            tempString = "Bot WINS!";
        }
        drawMap(map2);
        drawShips(bot);
        DrawTextEx(fonts[FONT_PACMAN], tempString.c_str(), Vector2{SCREEN_WIDTH * 7 / 16, SCREEN_HEIGHT * 14 / 32}, fonts[FONT_PACMAN].baseSize, 0, Color{0, 0, 0, 255});
        drawImages(bot, map2);
        drawImages(player, map);
        break;
    }
    }

    EndDrawing();

    if (nextBattleState || gotHit)
    {
        WaitTime(2);
    }
}

void Game::stateTransition()
{
    shout = false;
    projectileInShip = false;
    explosion = false;
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
    case STATE_BATTLE:
    {
        gotHit = false;
        if (nextBattleState)
        {
            currentBattleState = (BattleStates)((int)currentBattleState + 1);
            if (currentBattleState > BATTLE_STATE_BOT)
            {
                currentBattleState = BATTLE_STATE_PLAYER;
            }
            nextBattleState = false;
        }

        if (!player.alive || !bot.alive)
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

bool Game::isPossibleToPlaceShip(Ship &ship, int iInd, int jInd, Player &player, std::vector<std::vector<Rectangle>> &map, bool botPlacement)
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
        if (botPlacement)
        {
            for (int i = iInd; i <= iInd + ship.rect.height / 50 - 1; i++)
            {
                for (int j = jInd; j <= jInd + ship.rect.width / 50 - 1; j++)
                {

                    if (player.field[i][j] != 0)
                    {
                        int index = findValue({i, j});
                        if (index != -1)
                        {
                            emptyCellsBot.erase(emptyCellsBot.begin() + index);
                        }
                    }

                    if (i + 1 < MAP_SIZE && player.field[i + 1][j] != 0)
                    {
                        int index = findValue({i + 1, j});
                        if (index != -1)
                        {
                            emptyCellsBot.erase(emptyCellsBot.begin() + index);
                        }
                    }
                    if (j + 1 < MAP_SIZE && player.field[i][j + 1] != 0)
                    {
                        int index = findValue({i, j + 1});
                        if (index != -1)
                        {
                            emptyCellsBot.erase(emptyCellsBot.begin() + index);
                        }
                    }
                    if (i - 1 >= 0 && player.field[i - 1][j] != 0)
                    {
                        int index = findValue({i - 1, j});
                        if (index != -1)
                        {
                            emptyCellsBot.erase(emptyCellsBot.begin() + index);
                        }
                    }
                    if (j - 1 >= 0 && player.field[i][j - 1] != 0)
                    {
                        int index = findValue({i, j - 1});
                        if (index != -1)
                        {
                            emptyCellsBot.erase(emptyCellsBot.begin() + index);
                        }
                    }
                    if (i - 1 >= 0 && j - 1 >= 0 && player.field[i - 1][j - 1] != 0)
                    {
                        int index = findValue({i - 1, j - 1});
                        if (index != -1)
                        {
                            emptyCellsBot.erase(emptyCellsBot.begin() + index);
                        }
                    }
                    if (i - 1 >= 0 && j + 1 < MAP_SIZE && player.field[i - 1][j + 1] != 0)
                    {
                        int index = findValue({i - 1, j + 1});
                        if (index != -1)
                        {
                            emptyCellsBot.erase(emptyCellsBot.begin() + index);
                        }
                    }
                    if (i + 1 < MAP_SIZE && j - 1 >= 0 && player.field[i + 1][j - 1] != 0)
                    {
                        int index = findValue({i + 1, j - 1});
                        if (index != -1)
                        {
                            emptyCellsBot.erase(emptyCellsBot.begin() + index);
                        }
                    }
                    if (i + 1 < MAP_SIZE && j + 1 < MAP_SIZE && player.field[i + 1][j + 1] != 0)
                    {
                        int index = findValue({i + 1, j + 1});
                        if (index != -1)
                        {
                            emptyCellsBot.erase(emptyCellsBot.begin() + index);
                        }
                    }
                }
            }
        }
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
            for (int l = 0; l < 2; l++)
            {
                for (int k = 0; k < 2; k++)
                {
                    DrawTexture(textures[TEXTURE_WATER], map[i][j].x + l * 25, map[i][j].y + k * 25, WHITE);
                }
            }
            // DrawTexture(textures[TEXTURE_WATER], map[i][j].x, map[i][j].y, WHITE);
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
            tempString = "I";
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

void Game::drawShips(Player &player, bool hideShips)
{
    for (int i = 0; i <= 9; i++)
    {
        if (!hideShips || !player.ships[i].isAfloat)
        {
            Rectangle tempRect = player.ships[i].rect;
            // DrawRectangleRec(tempRect, Color{0, 0, 0, 255});

            switch (player.ships[i].shipParts)
            {
            case 1:
            {

                if (player.ships[i].verticalPos)
                {
                    DrawTexture(textures[TEXTURE_SHIP_1], tempRect.x, tempRect.y, WHITE);
                }
                else
                {
                    DrawTextureEx(textures[TEXTURE_SHIP_1], Vector2{tempRect.x, tempRect.y + textures[TEXTURE_SHIP_1].width}, -90, 1, WHITE);
                }
                break;
            }
            case 2:
            {
                if (player.ships[i].verticalPos)
                {
                    DrawTexture(textures[TEXTURE_SHIP_2], tempRect.x, tempRect.y, WHITE);
                }
                else
                {
                    DrawTextureEx(textures[TEXTURE_SHIP_2], Vector2{tempRect.x, tempRect.y + textures[TEXTURE_SHIP_2].width}, -90, 1, WHITE);
                }
                break;
            }
            case 3:
            {
                if (player.ships[i].verticalPos)
                {
                    DrawTexture(textures[TEXTURE_SHIP_3], tempRect.x, tempRect.y, WHITE);
                }
                else
                {
                    DrawTextureEx(textures[TEXTURE_SHIP_3], Vector2{tempRect.x, tempRect.y + textures[TEXTURE_SHIP_3].width}, -90, 1, WHITE);
                }
                break;
            }
            case 4:
            {
                if (player.ships[i].verticalPos)
                {
                    DrawTexture(textures[TEXTURE_SHIP_4], tempRect.x, tempRect.y, WHITE);
                }
                else
                {
                    DrawTextureEx(textures[TEXTURE_SHIP_4], Vector2{tempRect.x, tempRect.y + textures[TEXTURE_SHIP_4].width}, -90, 1, WHITE);
                }
                break;
            }
            }
        }
    }
}

void Game::deleteEverythingDeadShip(Ship &ship, Player &player, std::vector<std::vector<Rectangle>> &map)
{

    int iInd = (ship.rect.y - map[0][0].y) / 50;
    int jInd = (ship.rect.x - map[0][0].x) / 50;

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

            player.field[i][j] = 3;

            if (i - 1 >= 0)
            {
                player.field[i - 1][j] = 3;
            }

            if (j - 1 >= 0)
            {
                player.field[i][j - 1] = 3;
            }
            if (i + 1 < MAP_SIZE)
            {
                player.field[i + 1][j] = 3;
            }
            if (j + 1 < MAP_SIZE)
            {
                player.field[i][j + 1] = 3;
            }

            if (i - 1 >= 0 && j - 1 >= 0)
            {
                player.field[i - 1][j - 1] = 3;
            }
            if (i - 1 >= 0 && j + 1 < MAP_SIZE)
            {
                player.field[i - 1][j + 1] = 3;
            }
            if (i + 1 < MAP_SIZE && j + 1 < MAP_SIZE)
            {
                player.field[i + 1][j + 1] = 3;
            }
            if (i + 1 < MAP_SIZE && j - 1 >= 0)
            {
                player.field[i + 1][j - 1] = 3;
            }
        }
    }
}

void Game::updateStateBattle(Player &player, std::vector<std::vector<Rectangle>> &map)
{
    int temp;
    for (int i = 0; i <= 9; i++)
    {
        temp = 0;
        int iInd = (player.ships[i].rect.y - map[0][0].y) / 50;
        int jInd = (player.ships[i].rect.x - map[0][0].x) / 50;
        if (player.ships[i].rect.height >= player.ships[i].rect.width)
        {
            for (int k = iInd; k <= iInd + player.ships[i].shipParts - 1; k++)
            {
                if (player.field[k][jInd] == 2)
                {
                    player.ships[i].isHitted = true;
                    temp++;
                }
            }
        }
        else
        {
            for (int k = jInd; k <= jInd + player.ships[i].shipParts - 1; k++)
            {
                if (player.field[iInd][k] == 2)
                {
                    player.ships[i].isHitted = true;
                    temp++;
                }
            }
        }

        if (temp == player.ships[i].shipParts)
        {
            deleteEverythingDeadShip(player.ships[i], player, map);
            player.ships[i].isAfloat = false;
            player.ships[i].isHitted = false;
        }
    }
}

void Game::markDeadPLayer(Player &player)
{
    int temp = 0;
    for (int i = 0; i <= 9; i++)
    {
        if (!player.ships[i].isAfloat)
        {
            temp++;
        }
    }
    if (temp >= 10)
    {
        player.alive = false;
    }
}

int Game::findValue(std::pair<int, int> value)
{
    for (int i = 0; i < emptyCellsBot.size(); i++)
    {
        if (emptyCellsBot[i] == value)
        {
            return i;
        }
    }

    return -1;
}

void Game::loadTextures()
{
    images[IMAGE_FIRE] = LoadImage("assets/fire.png");
    ImageCrop(&images[IMAGE_FIRE], Rectangle{134, 143, 37, 42});
    ImageResize(&images[IMAGE_FIRE], 45, 45);
    textures[TEXTURE_FIRE] = LoadTextureFromImage(images[IMAGE_FIRE]);

    images[IMAGE_CROSS] = LoadImage("assets/cross.png");
    ImageCrop(&images[IMAGE_CROSS], Rectangle{89, 205, 736, 714});
    ImageResize(&images[IMAGE_CROSS], 45, 45);
    textures[TEXTURE_CROSS] = LoadTextureFromImage(images[IMAGE_CROSS]);

    images[IMAGE_WATER] = LoadImage("assets/water.png");
    ImageCrop(&images[IMAGE_WATER], Rectangle{80, 0, 15, 15}); // 80 0 15 15
    ImageResize(&images[IMAGE_WATER], 25, 25);
    textures[TEXTURE_WATER] = LoadTextureFromImage(images[IMAGE_WATER]);

    images[IMAGE_SHIP_1] = LoadImage("assets/ships.png");
    ImageCrop(&images[IMAGE_SHIP_1], Rectangle{1, 1, 29, 60});
    ImageResize(&images[IMAGE_SHIP_1], 50, 50);
    textures[TEXTURE_SHIP_1] = LoadTextureFromImage(images[IMAGE_SHIP_1]);

    images[IMAGE_SHIP_2] = LoadImage("assets/ships.png");
    ImageCrop(&images[IMAGE_SHIP_2], Rectangle{76, 1, 29, 91});
    ImageResize(&images[IMAGE_SHIP_2], 50, 100);
    textures[TEXTURE_SHIP_2] = LoadTextureFromImage(images[IMAGE_SHIP_2]);

    images[IMAGE_SHIP_3] = LoadImage("assets/ships.png");
    ImageCrop(&images[IMAGE_SHIP_3], Rectangle{113, 1, 29, 122});
    ImageResize(&images[IMAGE_SHIP_3], 50, 150);
    textures[TEXTURE_SHIP_3] = LoadTextureFromImage(images[IMAGE_SHIP_3]);

    images[IMAGE_SHIP_4] = LoadImage("assets/ships.png");
    ImageCrop(&images[IMAGE_SHIP_4], Rectangle{151, 1, 29, 153});
    ImageResize(&images[IMAGE_SHIP_4], 50, 200);
    textures[TEXTURE_SHIP_4] = LoadTextureFromImage(images[IMAGE_SHIP_4]);
}

void Game::drawImages(Player &player, std::vector<std::vector<Rectangle>> &map)
{

    for (int i = 0; i < MAP_SIZE; i++)
    {
        for (int j = 0; j < MAP_SIZE; j++)
        {
            if (player.field[i][j] == 2)
            {
                DrawTexture(textures[TEXTURE_CROSS], map[i][j].x, map[i][j].y, WHITE);
            }
            else if (player.field[i][j] == 3)
            {
                DrawTexture(textures[TEXTURE_FIRE], map[i][j].x, map[i][j].y, WHITE);
            }
            else if (player.field[i][j] == 4)
            {

                DrawCircle(map[i][j].x + 25, map[i][j].y + 25, 12, Color{0, 0, 0, 255});
            }
        }
    }
}

void Game::loadMusics()
{
    musics[MUSIC_SEA] = Mix_LoadMUS("assets/Sounds/sea.mp3");

    chunks[CHUNK_SIREN] = Mix_LoadWAV("assets/Sounds/siren.mp3");
    chunks[CHUNK_SHOUT] = Mix_LoadWAV("assets/Sounds/shout.mp3");
    chunks[CHUNK_SKY] = Mix_LoadWAV("assets/Sounds/sky.mp3");
    chunks[CHUNK_EXPLOSION] = Mix_LoadWAV("assets/Sounds/explosion.mp3");
}

void Game::playSounds()
{
    if (currentGlobalState == STATE_BATTLE)
    {

        if (shout)
        {
            Mix_PlayChannelTimed(CHANNEL_SHOUT, chunks[CHUNK_SHOUT], 0, 2000);
        }

        if (projectileInShip)
        {
            Mix_PlayChannel(CHANNEL_SKY, chunks[CHUNK_SKY], 0);
        }

        if (explosion)
        {
            SDL_Delay(2000);
            Mix_PlayChannel(CHANNEL_EXPLOSION, chunks[CHUNK_EXPLOSION], 0);
        }

        bool flag = false;
        for (int i = 0; i <= 9; i++)
        {
            if (player.ships[i].isHitted)
            {
                flag = true;
            }
        }

        if (flag)
        {
            if (!Mix_Playing(CHANNEL_SIREN))
            {
                Mix_PlayChannel(CHANNEL_SIREN, chunks[CHUNK_SIREN], -1);
            }
        }
        else
        {
            if (Mix_Playing(CHANNEL_SIREN))
            {
                Mix_HaltChannel(CHANNEL_SIREN);
            }
        }
    }
    else if (currentGlobalState == STATE_WINNER)
    {
        if (Mix_Playing(CHANNEL_SIREN))
        {
            Mix_HaltChannel(CHANNEL_SIREN);
        }
    }
}

void Game::drawRemainingShips()
{
    for (int i = 0; i <= 3; i++)
    {
        DrawTexture(textures[TEXTURE_SHIP_1], SCREEN_WIDTH * 3 * (i + 1) / 60, SCREEN_HEIGHT * 10 / 16, WHITE);
        DrawTexture(textures[TEXTURE_SHIP_1], SCREEN_WIDTH * 3 * (i + 1 + 13) / 60 - textures[TEXTURE_SHIP_1].width, SCREEN_HEIGHT * 10 / 16, WHITE);
        if (!player.ships[i].isAfloat)
        {
            DrawTexture(textures[TEXTURE_CROSS], SCREEN_WIDTH * 3 * (i + 1) / 60, SCREEN_HEIGHT * 10 / 16, WHITE);
        }

        if (!bot.ships[i].isAfloat)
        {
            DrawTexture(textures[TEXTURE_CROSS], SCREEN_WIDTH * 3 * (i + 1 + 13) / 60, SCREEN_HEIGHT * 10 / 16, WHITE);
        }
    }
    for (int i = 4; i <= 6; i++)
    {
        DrawTexture(textures[TEXTURE_SHIP_2], SCREEN_WIDTH * 3 * (i + 1 - 4) / 60, SCREEN_HEIGHT * 22 / 32, WHITE);
        DrawTexture(textures[TEXTURE_SHIP_2], SCREEN_WIDTH * 3 * (i + 1 + 13 - 4) / 60 - textures[TEXTURE_SHIP_1].width, SCREEN_HEIGHT * 22 / 32, WHITE);

        if (!player.ships[i].isAfloat)
        {
            DrawTextureEx(textures[TEXTURE_CROSS], Vector2{(float)SCREEN_WIDTH * 3 * (i + 1 - 4) / 60 - 15, SCREEN_HEIGHT * 22 / 32}, 0, 2, WHITE);
        }

        if (!bot.ships[i].isAfloat)
        {
            DrawTextureEx(textures[TEXTURE_CROSS], Vector2{(float)SCREEN_WIDTH * 3 * (i + 1 + 13 - 4) / 60 - 15, SCREEN_HEIGHT * 22 / 32}, 0, 2, WHITE);
        }
    }

    for (int i = 7; i <= 8; i++)
    {
        DrawTexture(textures[TEXTURE_SHIP_3], SCREEN_WIDTH * 3 * (i + 1 - 7) / 60, SCREEN_HEIGHT * 51 / 64, WHITE);
        DrawTexture(textures[TEXTURE_SHIP_3], SCREEN_WIDTH * 3 * (i + 1 + 13 - 7) / 60 - textures[TEXTURE_SHIP_1].width, SCREEN_HEIGHT * 51 / 64, WHITE);

        if (!player.ships[i].isAfloat)
        {
            DrawTextureEx(textures[TEXTURE_CROSS], Vector2{(float)SCREEN_WIDTH * 3 * (i + 1 - 7) / 60 - textures[TEXTURE_CROSS].width / 2, (float)SCREEN_HEIGHT * 51 / 64 + textures[TEXTURE_CROSS].height / 2}, 0, 2, WHITE);
        }

        if (!bot.ships[i].isAfloat)
        {
            DrawTextureEx(textures[TEXTURE_CROSS], Vector2{(float)SCREEN_WIDTH * 3 * (i + 1 + 13 - 7) / 60 - textures[TEXTURE_CROSS].width / 2, (float)SCREEN_HEIGHT * 51 / 64 + textures[TEXTURE_CROSS].height / 2}, 0, 2, WHITE);
        }
    }
    for (int i = 9; i <= 9; i++)
    {
        DrawTextureEx(textures[TEXTURE_SHIP_4], Vector2{SCREEN_WIDTH * 9 / 60, SCREEN_HEIGHT * 57 / 64}, -90, 1, WHITE);
        DrawTextureEx(textures[TEXTURE_SHIP_4], Vector2{(float)SCREEN_WIDTH * 48 / 60 - textures[TEXTURE_SHIP_1].width, SCREEN_HEIGHT * 57 / 64}, -90, 1, WHITE);

        if (!player.ships[i].isAfloat)
        {
            DrawTextureEx(textures[TEXTURE_CROSS], Vector2{(float)SCREEN_WIDTH * 9 / 60 + textures[TEXTURE_CROSS].width, (float)SCREEN_HEIGHT * 57 / 64 - textures[TEXTURE_CROSS].height * 3 / 2}, 0, 2, WHITE);
        }

        if (!bot.ships[i].isAfloat)
        {
            DrawTextureEx(textures[TEXTURE_CROSS], Vector2{(float)SCREEN_WIDTH * 48 / 60 + textures[TEXTURE_CROSS].width, (float)SCREEN_HEIGHT * 57 / 64 - textures[TEXTURE_CROSS].height * 3 / 2}, 0, 2, WHITE);
        }
    }
}