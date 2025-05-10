#include "Level3_DeathEgg.h"
#include <iostream>
#include <fstream>

Level3_DeathEgg::Level3_DeathEgg() : Level(300, 2, 2, 2, 4) {
    // Load Textures
    if (!bgTex.loadFromFile("Data/deathegg/bg1.png")) {
        cout << "Failed to load to bg1.png" << endl;
    }

    bgSprite.setTexture(bgTex);

    if (!wallTex1.loadFromFile("Data/deathegg/brick1.png")) {
        cout << "Failed to load brick1.png" << endl;
    }

    wallSprite1.setTexture(wallTex1);

    if (!wallTex2.loadFromFile("Data/deathegg/brick2.png")) {
        cout << "Failed to load brick2.png" << endl;
    }

    wallSprite2.setTexture(wallTex2);

    if (!wallTex3.loadFromFile("Data/deathegg/brick3.png")) {
        cout << "Failed to load brick3.png" << endl;
    }

    wallSprite3.setTexture(wallTex3);

    initializeGrid();
}

Level3_DeathEgg::~Level3_DeathEgg() {
    
}

void Level3_DeathEgg::initializeGrid() {
    // 's' - space
    // 'p' - pit
    // 'w' - wall1
    // 'q' - platform
    // 'b' - breakable wall
    // 'x' - spike
    // 'C' - crystal
    // 'R' - ring
    // '\0' - out of bounds

    if (!loadFromFile("Data/Level3_Layout.txt")) {
        cout << "Unable to load level layout file!" << endl;
    }
}

void Level3_DeathEgg::loadAssets(int volume) {
    levelVolume = volume;
    levelSounds->setVolume(levelVolume);
    // acc_x, acc_y, friction, gravity
    player = sonicMaker.createPlayer(0.4f, 13.0f, 0.1f, 0.255f);
    player->setLeader(true);
    currentPlayer = 's';
    player->setFollowers(tailsMaker.createPlayer(0.37f, 13.0f, 0.1f, 0.255f), knucklesMaker.createPlayer(0.32f, 13.0f, 0.1f, 0.255f));
    player->setCollidingTiles(64, 14, 200, grid);

    batBrains[0] = batBrainMaker.createBatBrain(800.0f, 100.0f);
    batBrains[1] = batBrainMaker.createBatBrain(14336.0f, 100.0f);

    beeBots[0] = beeBotMaker.createBeeBot(7360.0f, 50.0f, 8128.0f, 6784.0f);
    beeBots[1] = beeBotMaker.createBeeBot(7680.0f, 50.0f, 8128.0f, 6784.0f);

    motoBugs[0] = motoBugMaker.createMotoBug(7360.0f, 768.0f, 8128.0f, 6912.0f);
    motoBugs[1] = motoBugMaker.createMotoBug(14336.0f, 512.0f, 14400.0f, 14272.0f);

    crabMeats[0] = crabMeatMaker.createCrabMeat(768.0f, 768.0f, 960.0f, 448.0f);
    crabMeats[1] = crabMeatMaker.createCrabMeat(10560.0f, 384.0f, 10944.0f, 10368.0f);
    crabMeats[2] = crabMeatMaker.createCrabMeat(10752.0f, 640.0f, 11008.0f, 10560.0f);
    crabMeats[3] = crabMeatMaker.createCrabMeat(15744.0f, 384.0f, 15872.0f, 15616.0f);

    if (!levelMusic.openFromFile("Data/Level3.ogg")) {
        cout << "Failed to load Level3.ogg" << endl;
    }
    else {
        levelMusic.setVolume(volume);
        levelMusic.setLoop(true);
        levelMusic.play();
    }
}