#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>

#include "SoundManager.h"

using namespace sf;
using namespace std;

// Setting menu states using int variables
const int STATE_MAIN = 0;
const int STATE_OPTIONS = 1;
const int STATE_NAME_INPUT = 2;
const int STATE_LEADERBOARD = 3;
const int STATE_EXIT = 4;
const int STATE_SELECT_LEVEL = 5;

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
    int selectedLevel;

    bool isMuted;
    bool leaderboardRequested; // used to transfer control over to leaderboard in Game class
    bool gameStarted;
    bool wantsToContinue;

    const int menuItemCount = 6;
    string mainOptions[6] = {"New Game", "Options", "Continue", "LeaderBoard", "Select Level", "Exit"};

    // Options
    Text volumeText;

    // Name Input
    string playerName;
    Text namePrompt;
    Text nameText;

    // UI
    Text title;
    Text menuText[6];
    Text levelText[4];

    // Background
    Texture bgTexture;
    Sprite bgSprite;

    int bgWidth;
    int bgHeight;

    SoundManager* menuSounds;

public:
    Menu(RenderWindow& window, Music & music, int screen_x, int screen_y);
    ~Menu();
    void handleInput(Event& event);
    void update();
    void draw();

    void setCurrentState(int state);
    bool isLeaderboardRequested() const;
    void resetLeaderboardRequest();
    bool hasGameStarted() const;
    void resetGameStarted();
    string getPlayerName() const;
    int getSelectedLevel() const;
    int getVolume() const;
    bool isMutedStatus() const;
    bool getWantsToContinue() const;
    void resetWantsToContinue();
};
