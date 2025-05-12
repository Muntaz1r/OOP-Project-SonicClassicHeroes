#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>

using namespace sf;
using namespace std;

//const int MAX_NUM = 10;

struct ScoreRecord {
    string name;
    int score;
};

class Scoreboard {
private:
    ScoreRecord scores[11]; // only top 10 scores + 1 extra for new score
    int numScores; // total number stored

    Font font;
    Text title;
    Text scoreTexts[10];

    string filePath;

    int screen_x, screen_y;

    int bgWidth;
    int bgHeight;

    // Background
    Texture bgTexture;
    Sprite bgSprite;
public:
    Scoreboard(int screen_x, int screen_y);

    void loadScores();
    void saveScores(const string name, int score);
    void draw(RenderWindow& window);
};