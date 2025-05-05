#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>

using namespace sf;
using namespace std;

#include "Animation.h"
#include "PlayerFactory.h"

class Levels {
protected:
    sf::Clock switchCooldownClock;
public:
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

    // int levelWidthInTiles = 14;
    // int cellSize = 64;

    bool completed;
    bool failed;

    Texture wallTex1; // wall
    Sprite wallSprite1;

    Texture wallTex2; // platform
    Sprite wallSprite2;

    Texture wallTex3; // breakable wall
    Sprite wallSprite3;

    Texture ringTex;
    Sprite ringSprite;

    Texture spikeTex;
    Sprite spikeSprite;

    Texture crystalTex;
    Sprite crystalSprite;
    //Sprite ringSprites[MAX_RINGS];
    //Position ringPositions[MAX_RINGS];
    //bool ringCollected[MAX_RINGS];
    //int ringCount;
    Animation ringAnimation;

    Texture bgTex;
    Sprite bgSprite;

    SonicFactory sonicMaker;
    TailsFactory tailsMaker;
    KnucklesFactory knucklesMaker;
    Player* player;

    // int ringFrameIndex;
    // Clock ringAnimationClock;

    Music levelMusic;

    void initializeGrid();

public:
    Level1_Labyrinth();
    ~Level1_Labyrinth();

    void loadAssets() override;
    bool loadFromFile(const string& filePath);
    void update(float deltaTime) override;
    void render(RenderWindow& window, float cameraOffsetX) override;
    //void reset() override;
    bool isLevelComplete() const override;
    //bool isFailed() const override;
    float getPlayerX() const override;
    int getPlayerWidth() const override;
    int getLevelWidthinTiles() const override;
    int getCellSize() const override;
};