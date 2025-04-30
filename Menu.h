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

class Menu {
private:
    RenderWindow& window;
    Font font;
    Music& music;

    int currentState;
    int volume;
    int mainSelected;

    string mainOptions[4] = {"New Game", "Options", "Continue", "LeaderBoard"};

    // Options
    Text volumeText;

    // Name Input
    string playerName;
    Text namePrompt;
    Text nameText;

    // UI
    Text title;
    Text menutext[4];

public:
    Menu(RenderWindow& window, Music & music);

    void handleInput(Event& event) {}
    void update();
    void draw();

    bool shouldExit() const;
    bool startGameTriggered() const;
    string getPlayerName() const;
};