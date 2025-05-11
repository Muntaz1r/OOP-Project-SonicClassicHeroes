#include "Level1_Labyrinth.h"
#include <iostream>
#include <fstream>

Level1_Labyrinth::Level1_Labyrinth() : Level(200, 1, 1, 1, 1) {
    levelID = 1;
    // Load Textures
    if (!bgTex.loadFromFile("Data/bg1.png")) {
        cout << "Failed to load to bg1.png" << endl;
    }

    bgSprite.setTexture(bgTex);

    if (!wallTex1.loadFromFile("Data/brick1.png")) {
        cout << "Failed to load brick1.png" << endl;
    }

    wallSprite1.setTexture(wallTex1);

    if (!wallTex2.loadFromFile("Data/brick2.png")) {
        cout << "Failed to load brick2.png" << endl;
    }

    wallSprite2.setTexture(wallTex2);

    if (!wallTex3.loadFromFile("Data/brick3.png")) {
        cout << "Failed to load brick3.png" << endl;
    }

    wallSprite3.setTexture(wallTex3);
    initializeGrid();
}

Level1_Labyrinth::~Level1_Labyrinth() {
    
}

void Level1_Labyrinth::initializeGrid() {
    // 's' - space
    // 'p' - pit
    // 'w' - wall1
    // 'q' - platform
    // 'b' - breakable wall
    // 'x' - spike
    // 'C' - crystal
    // 'R' - ring
    // '\0' - out of bounds

    if (!loadFromFile("Data/Level1_Layout.txt")) {
        cout << "Unable to load level layout file!" << endl;
    }
}

void Level1_Labyrinth::loadAssets(int volume) {
    levelVolume = volume;
    levelSounds->setVolume(levelVolume);
    // acc_x, acc_y, friction, gravity
    player = sonicMaker.createPlayer(0.5f, 13.0f, 0.2f, 0.4f);
    player->setLeader(true);
    currentPlayer = 's';
    player->setFollowers(tailsMaker.createPlayer(0.47f, 13.0f, 0.2f, 0.4f), knucklesMaker.createPlayer(0.42f, 14.0f, 0.2f, 0.4f));
    player->setCollidingTiles(64, 14, 200, grid);

    batBrains[0] = batBrainMaker.createBatBrain(800.0f, 100.0f);

    beeBots[0] = beeBotMaker.createBeeBot(500.0f, 50.0f, 4864.0f, 3584.0f);

    motoBugs[0] = motoBugMaker.createMotoBug(7500.0f, 768.0f, 8511.0f, 6536.0f);

    crabMeats[0] = crabMeatMaker.createCrabMeat(10800.0f, 768.0f, 11392.0f, 10752.0f);

    if (!levelMusic.openFromFile("Data/Level1.ogg")) {
        cout << "Failed to load Level1.ogg" << endl;
    }
    else {
        levelMusic.setVolume(volume);
        levelMusic.setLoop(true);
        levelMusic.play();
    }
}