#include "Level2_IceCap.h"
#include <iostream>
#include <fstream>

Level2_IceCap::Level2_IceCap() : Level(250, 2, 1, 1, 4) {
    levelID = 2;
    // Load Textures
    if (!bgTex.loadFromFile("Data/icecap/bg1.png")) {
        cout << "Failed to load to bg1.png" << endl;
    }

    bgSprite.setTexture(bgTex);

    if (!wallTex1.loadFromFile("Data/icecap/brick1.png")) {
        cout << "Failed to load brick1.png" << endl;
    }

    wallSprite1.setTexture(wallTex1);

    if (!wallTex2.loadFromFile("Data/icecap/brick2.png")) {
        cout << "Failed to load brick2.png" << endl;
    }

    wallSprite2.setTexture(wallTex2);

    if (!wallTex3.loadFromFile("Data/icecap/brick3.png")) {
        cout << "Failed to load brick3.png" << endl;
    }

    wallSprite3.setTexture(wallTex3);
    initializeGrid();
}

Level2_IceCap::~Level2_IceCap() {
    
}

void Level2_IceCap::initializeGrid() {
    // 's' - space
    // 'p' - pit
    // 'w' - wall1
    // 'q' - platform
    // 'b' - breakable wall
    // 'x' - spike
    // 'C' - crystal
    // 'R' - ring
    // '\0' - out of bounds

    if (!loadFromFile("Data/Level2_Layout.txt")) {
        cout << "Unable to load level layout file!" << endl;
    }
}

void Level2_IceCap::loadAssets(int volume) {
    levelVolume = volume;
    levelSounds->setVolume(levelVolume);
    // acc_x, acc_y, friction, gravity
    player = sonicMaker.createPlayer(0.7f, 13.0f, 0.05f, 0.4f);
    player->setLeader(true);
    currentPlayer = 's';
    player->setFollowers(tailsMaker.createPlayer(0.67f, 13.0f, 0.05f, 0.4f), knucklesMaker.createPlayer(0.62f, 13.0f, 0.05f, 0.4f));
    player->setCollidingTiles(64, 14, 200, grid);

    batBrains[0] = batBrainMaker.createBatBrain(800.0f, 100.0f);
    batBrains[1] = batBrainMaker.createBatBrain(10816.0f, 100.0f);

    beeBots[0] = beeBotMaker.createBeeBot(12800.0f, 50.0f, 13888.0f, 11136.0f);

    motoBugs[0] = motoBugMaker.createMotoBug(7936.0f, 320.0f, 7936.0f, 6976.0f);

    crabMeats[0] = crabMeatMaker.createCrabMeat(768.0f, 768.0f, 1152.0f, 512.0f);
    crabMeats[1] = crabMeatMaker.createCrabMeat(3520.0f, 768.0f, 3840.0f, 2944.0f);
    crabMeats[2] = crabMeatMaker.createCrabMeat(9600.0f, 448.0f, 10816.0f, 9024.0f);
    crabMeats[3] = crabMeatMaker.createCrabMeat(10048.0f, 768.0f, 10816.0f, 9024.0f);

    if (!levelMusic.openFromFile("Data/Level2.ogg")) {
        cout << "Failed to load Level2.ogg" << endl;
    }
    else {
        levelMusic.setVolume(volume);
        levelMusic.setLoop(true);
        levelMusic.play();
    }
}

void Level2_IceCap::render(RenderWindow& window, float cameraOffsetX){
    Level::render(window, cameraOffsetX);
    portalSprite.setPosition(width*cellSize - 200 - cameraOffsetX, 450); // exit portal
    window.draw(portalSprite);
}