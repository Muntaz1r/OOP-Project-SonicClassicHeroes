#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>

using namespace sf;
using namespace std;

#include "PlayerFactory.h"

class Levels {
protected:
    sf::Clock gameTime;
    int score;
    sf::Clock switchCooldownClock;
    char currentPlayer;
    Player* player;
    Texture portalTex;
    Sprite portalSprite;
    float exitX;

public:
    Levels(){
        score = 0;
        gameTime.restart();
    }
    virtual void loadAssets() = 0; // load level assets
    virtual void update(float deltaTime) =0; // update logic per frame
    virtual void render(RenderWindow& window, float cameraOffsetX) = 0; // Draw the level
    //virtual void reset() = 0;
    virtual bool isLevelComplete() const = 0;
    //virtual bool isFailed() const = 0;
    // used for exposing the player details that Game needs, without violating encapsulation
    virtual float getPlayerX() const = 0;
    virtual int getPlayerWidth() const = 0;
    virtual int getLevelWidthinTiles() const = 0;
    virtual int getCellSize() const = 0;
    virtual int getScore() const;
    virtual sf::Clock getGameTime() const;
    virtual void drawUI(sf::RenderWindow& window, float cameraOffset) const;
    virtual bool exitCheck(float cameraOffset);
    virtual ~Levels() {}
};
