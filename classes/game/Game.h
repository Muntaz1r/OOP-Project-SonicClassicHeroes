#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>

#include "Menu.h"
#include "Scoreboard.h"
#include "Level1_Labyrinth.h"
#include "Level2_IceCap.h"
#include "Level3_DeathEgg.h"
#include "BossLevel.h"
#include "SaveState.h"

using namespace sf;
using namespace std;




class Game {
private:
    // GAME STATES
    const int GAME_STATE_MENU = 0;
    const int GAME_STATE_PLAYING = 1;
    const int GAME_STATE_LEADERBOARD = 2;
    
    RenderWindow window;
    Music music;

    Menu* menu;
    Scoreboard* scoreboard;
    Level* level;

    int screen_x;
    int screen_y;

    int currentState;
   

    float cameraOffsetX; // used to store offset for horizontal scrolling

public:
    Game(int screen_x, int screen_y);
    ~Game();

    void runGame();
    void loadGameFromSave(int levelWidth, int levelHeight);
};