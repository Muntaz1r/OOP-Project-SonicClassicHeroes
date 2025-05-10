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

class BossLevel : public Levels {
private:
    static const int height = 14;    
    static const int width = 19;
    static const int cellSize = 64;

    char** grid;

    bool completed;
    bool failed;
    bool waitingAtBottom;

    Texture wallTex; // ground
    Sprite wallSprite;

    Texture bgTex;
    Sprite bgSprite;

    SonicFactory sonicMaker;
    TailsFactory tailsMaker;
    KnucklesFactory knucklesMaker;

    EggStingerFactory eggStingerMaker;
    EggStinger* eggStinger;

    Music levelMusic;

    Clock waitClock;


    void initializeGrid();

    struct Brick {
        float x, y;
        bool isBroken;
        float breakTimer;
    };

    Brick bricks[19]; // 1200 / 64 ≈ 18.75 → 19 bricks
    const int groundY = 900 - 64;

public:
    BossLevel();
    ~BossLevel();

    void loadAssets() override;
    bool loadFromFile(const string& filePath);
    void update(float deltaTime) override;
    void render(RenderWindow& window, float cameraOffsetX = 0.0f) override;

    //void breakGroundAt(float targetX);
    //void renderBrokenBrick(RenderWindow& window, Brick& brick, float cameraOffsetX);

    bool isLevelComplete() const override;
    float getPlayerX() const override;
    int getPlayerWidth() const override;
    int getLevelWidthinTiles() const override;
    int getCellSize() const override;
};