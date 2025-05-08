#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>

using namespace sf;
using namespace std;

#include "Level.h"
#include "Animation.h"
#include "PlayerFactory.h"
#include "EnemyFactory.h"

class Level3_DeathEgg : public Levels {
private:
    static const int height = 14;    
    static const int width = 300;
    static const int cellSize = 64;
    static const int MAX_RINGS = 100;

    const int numBatBrain = 2;
    const int numBeeBot = 2;
    const int numMotoBug = 2;
    const int numCrabMeat = 4;

    char** grid;

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
    Animation ringAnimation;

    Texture bgTex;
    Sprite bgSprite;

    SonicFactory sonicMaker;
    TailsFactory tailsMaker;
    KnucklesFactory knucklesMaker;

    BatBrainFactory batBrainMaker;
    BatBrain** batBrains;

    BeeBotFactory beeBotMaker;
    BeeBot** beeBots;

    MotoBugFactory motoBugMaker;
    MotoBug** motoBugs;

    CrabMeatFactory crabMeatMaker;
    CrabMeat** crabMeats;

    Music levelMusic;

    void initializeGrid();

public:
    Level3_DeathEgg();
    ~Level3_DeathEgg();

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