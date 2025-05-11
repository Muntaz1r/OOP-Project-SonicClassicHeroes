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

class Level {
protected:
    const int height = 14;    
    const int width;
    static const int cellSize = 64;

    const int numBatBrain;
    const int numBeeBot;
    const int numMotoBug;
    const int numCrabMeat;

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
    
    Music levelMusic;

    SoundManager* levelSounds;
public:
    Level(int width, int numBatBrain, int numBeeBot, int numMotoBug, int numCrabMeat)
    : width(width), numBatBrain(numBatBrain), numBeeBot(numBeeBot), numMotoBug(numMotoBug), numCrabMeat(numCrabMeat) {
        score = 0;
        completed = false;
        failed = false;
        specialItemCount = 0;

        grid = new char*[height];
        for (int i = 0; i < height; i++) {
            grid[i] = new char[width];
        }

        for (int i = 0; i < height; i++) {
            for (int j = 0; j < width; j++) {
                grid[i][j] = 's';
            }
        }
        
        if (numBatBrain > 0) { batBrains = new BatBrain*[numBatBrain]; }
        if (numBeeBot > 0) { beeBots = new BeeBot*[numBeeBot]; }
        if (numMotoBug > 0) { motoBugs = new MotoBug*[numMotoBug]; }
        if (numCrabMeat > 0) { crabMeats = new CrabMeat*[numCrabMeat]; }
        
        for (int i = 0; i < numBatBrain; i++) {
            batBrains[i] = nullptr;
        }

        for (int i = 0; i < numBeeBot; i++) {
            beeBots[i] = nullptr;
        }

        for (int i = 0; i < numMotoBug; i++) {
            motoBugs[i] = nullptr;
        }

        for (int i = 0; i < numCrabMeat; i++) {
            crabMeats[i] = nullptr;
        }
    
        if (!spikeTex.loadFromFile("Data/spike.png")) {
            cout << "Failed to load spike.png" << endl;
        }
    
        spikeSprite.setTexture(spikeTex);
    
        if (!crystalTex.loadFromFile("Data/crystal.png")) {
            cout << "Failed to load crystal.png" << endl;
        }
    
        crystalSprite.setTexture(crystalTex);
    
        if (!ringTex.loadFromFile("Data/ring.png")) {
            cout << "Failed to load ring.png" << endl;
        }
    
        ringSprite.setTexture(ringTex);
        ringSprite.setScale(4.0f, 4.0f);

        if(!portalTex.loadFromFile("Data/portal.png")){
            cout<<"Failed to load portal.png";
        }
    
        portalSprite.setTexture(portalTex);
        portalSprite.setScale(0.75f,0.75f);

        if(!powerUpTex.loadFromFile("Data/powerUp.png")){
            cout<<"Failed to load powerUp.png";
        }
        
        powerUpSprite.setTexture(powerUpTex);
        powerUpSprite.setScale(0.13f, 0.13f);        

        if(!extraLifeTex.loadFromFile("Data/heartIcon.png")){
            cout<<"Failed to load powerUp.png";
        }

        
        extraLifeSprite.setTexture(extraLifeTex);
        extraLifeSprite.setScale(0.5f, 0.5f);
    
        ringAnimation.initialize(&ringSprite, &ringTex, 16, 16, 4, 0.2f);

        levelSounds = new SoundManager();
        
        // Index 0: Jump
        // Index 1: Switch
        levelSounds->loadSound(0, "Data/sfx/jump.wav");
        levelSounds->loadSound(1, "Data/sfx/switch.wav");

        gameTime.restart();
    }
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
    virtual sf::Clock getGameTime() const;
    virtual void drawUI(sf::RenderWindow& window, float cameraOffset) ;
    virtual bool exitCheck(float cameraOffSetX);
    
    virtual ~Level() {
        
    }
};
