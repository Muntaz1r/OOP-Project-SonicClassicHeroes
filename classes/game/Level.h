#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>

using namespace sf;
using namespace std;

#include "Animation.h"
#include "PlayerFactory.h"

class Levels {
public:
    virtual void loadAssets() = 0; // load level assets
    virtual void update(float deltaTime) = 0; // update logic per frame
    virtual void render(RenderWindow& window) = 0; // Draw the level
    //virtual void reset() = 0;
    virtual bool isLevelComplete() const = 0;
    //virtual bool isFailed() const = 0;
    
    virtual ~Levels() {}
};

struct Position {
    int posX;
    int posY;
};

class Level1_Labyrinth : public Levels {
private:
    static const int height = 14;    
    static const int width = 200;
    static const int cellSize = 64;
    static const int MAX_RINGS = 100;

    char** grid;

    bool completed;
    bool failed;

    Texture wallTex1;
    Sprite wallSprite1;

    Texture wallTex2;
    Sprite wallSprite2;

    Texture ringTex;
    Sprite ringSprite;
    //Sprite ringSprites[MAX_RINGS];
    //Position ringPositions[MAX_RINGS];
    //bool ringCollected[MAX_RINGS];
    //int ringCount;
    Animation ringAnimation;

    Texture bgTex;
    Sprite bgSprite;

    SonicFactory sonicMaker;
    Player* player;

    // int ringFrameIndex;
    // Clock ringAnimationClock;

    Music levelMusic;

    void initializeGrid();

public:
    Level1_Labyrinth();
    ~Level1_Labyrinth();

    void loadAssets() override;
    void update(float deltaTime) override;
    void render(RenderWindow& window) override;
    //void reset() override;
    bool isLevelComplete() const override;
    //bool isFailed() const override;
};