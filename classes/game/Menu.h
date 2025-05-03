#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>

using namespace sf;
using namespace std;

// Setting menu states using int variables
const int STATE_MAIN = 0;
const int STATE_OPTIONS = 1;
const int STATE_NAME_INPUT = 2;
const int STATE_LEADERBOARD = 3;
const int STATE_EXIT = 4;

class Menu {
private:
    int screen_x, screen_y;

    RenderWindow& window;
    Font font;
    Music& music;

    int currentState;
    int volume;
    int prevVolume;
    int mainSelected;

    bool isMuted;
    bool leaderboardRequested; // used to transfer control over to leaderboard in Game class
    bool gameStarted;

    const int menuItemCount = 5;
    string mainOptions[5] = {"New Game", "Options", "Continue", "LeaderBoard", "Exit"};

    // Options
    Text volumeText;

    // Name Input
    string playerName;
    Text namePrompt;
    Text nameText;

    // UI
    Text title;
    Text menuText[5];

    // Background
    Texture bgTexture;
    Sprite bgSprite;

    int bgWidth;
    int bgHeight;

public:
    Menu(RenderWindow& window, Music & music, int screen_x, int screen_y);

    void handleInput(Event& event);
    void update();
    void draw();

    bool isLeaderboardRequested() const;
    void resetLeaderboardRequest();
    bool hasGameStarted() const;
    void resetGameStarted();
    string getPlayerName() const;
};
