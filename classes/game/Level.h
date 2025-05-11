#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>

using namespace sf;
using namespace std;

#include "Animation.h"
#include "PlayerFactory.h"
#include "EnemyFactory.h"
#include "SoundManager.h"
#include "SaveState.h"

class Level {
protected:
    const int height = 14;    
    const int width;
    static const int cellSize = 64;

    const int numBatBrain;
    const int numBeeBot;
    const int numMotoBug;
    const int numCrabMeat;

    int levelID;

    int score;
    int levelVolume;

    char** grid;

    char currentPlayer;

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

    Texture portalTex;
    Sprite portalSprite;

    Texture powerUpTex;
    Sprite powerUpSprite;

    Texture extraLifeTex;
    Sprite extraLifeSprite;

    int specialItemCount;

    Texture bgTex;
    Sprite bgSprite;

    SonicFactory sonicMaker;
    TailsFactory tailsMaker;
    KnucklesFactory knucklesMaker;

    Player* player;

    BatBrainFactory batBrainMaker;
    BatBrain** batBrains;

    BeeBotFactory beeBotMaker;
    BeeBot** beeBots;

    MotoBugFactory motoBugMaker;
    MotoBug** motoBugs;

    CrabMeatFactory crabMeatMaker;
    CrabMeat** crabMeats;

    Clock gameTime;
    Clock switchCooldownClock;
    
    Time gameStartOffset;

    Music levelMusic;

    SoundManager* levelSounds;
public:
    Level(int width, int numBatBrain, int numBeeBot, int numMotoBug, int numCrabMeat);
    virtual void loadAssets(int volume) = 0; // load level assets
    bool loadFromFile(const string& filePath);
    virtual void update(float deltaTime); // update logic per frame
    virtual void render(RenderWindow& window, float cameraOffsetX); // Draw the level
    //virtual void reset() = 0;
    virtual bool isLevelComplete() const;
    //virtual bool isFailed() const;
    // used for exposing the player details that Game needs, without violating encapsulation
    virtual float getPlayerX() const;
    virtual int getPlayerWidth() const;
    virtual int getLevelWidthinTiles() const;
    virtual int getCellSize() const;
    virtual int getScore() const;
    virtual float getGameTime() const;
    virtual char getCurrentPlayer() const;
    virtual void drawUI(sf::RenderWindow& window, float cameraOffset);
    virtual bool exitCheck(float cameraOffSetX);
    virtual void initializeFromSave(
        float savedTimer, int savedScore, float playerX, float playerY,
        float velX, float velY, int hp, char currentChar, char** grid,
        BatBrain** batBrains, int numBatBrains,
        BeeBot** beeBots, int numBeeBots,
        MotoBug** motoBugs, int numMotoBugs,
        CrabMeat** crabMeats, int numCrabMeats
    );
    int getLevelID() const;
    void setLevelID(int value);
    void countSpecialItems();
    void spawnHelper(int, int);
    void spawnSpecialItems();
    virtual void saveLevelState() const;
    virtual ~Level();
};
