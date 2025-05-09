
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

class Level1_Labyrinth : public Levels {
    private:
        static const int height = 14;    
        static const int width = 200;
        static const int cellSize = 64;
        static const int MAX_RINGS = 100;
    
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
        BatBrain* batBrain;
    
        BeeBotFactory beeBotMaker;
        BeeBot* beeBot;
    
        MotoBugFactory motoBugMaker;
        MotoBug* motoBug;
    
        CrabMeatFactory crabMeatMaker;
        CrabMeat* crabMeat;
    
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