#include "Level.h"
#include <iostream>

Level1_Labyrinth::Level1_Labyrinth() {
    completed = false;
    failed = false;

    grid = new char*[height];
    for (int i = 0; i < height; i++) {
        grid[i] = new char[width];
    }

    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            grid[i][j] = ' ';
        }
    }

    initializeGrid();
}

Level1_Labyrinth::~Level1_Labyrinth() {
    for (int i = 0; i < height; i++) {
        delete grid[i];
    }
    delete[] grid;
}

void Level1_Labyrinth::initializeGrid() {
    for (int j = 0; j < width; j++) {
        grid[height - 3][j] = 'w'; // add floor
    }

    for (int j = 10; j < 20; j++) {
        grid[height - 6][j] = 'q'; // platform
    }

    grid[height - 4][12] = 'r'; // add rings
    grid[height - 7][15] = 'r';

    for (int j = 25; j < 35; j++) {
        grid[height - 5][j] = 'q'; // another platform
    }

    for (int j = 40; j < 55; j++) {
        grid[height - 7][j] = 'q'; // higher platform
    }

    grid[height - 8][42] = 'r'; // ring on higher platform
    grid[height - 6][28] = 'r'; // ring on middle platform
}

void Level1_Labyrinth::loadAssets() {
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

    ringCount = 3;
    ringFrameIndex = 0;

    if (!ringTex.loadFromFile("Data/ring.png")) {
        cout << "Failed to load ring.png" << endl;
    }

    ringSprite.setTexture(ringTex);
    ringSprite.setScale(4.0f, 4.0f);
    // for (int i = 0; i < ringCount; i++) {
    //     ringSprites[i].setTexture(ringTex);
    //     ringSprites[i].setTextureRect(IntRect(0, 0, 16, 16));
    //     ringSprites[i].setScale(4.0f, 4.0f);

    //     ringPositions[i].posX = (10 + i * 2) * cellSize;
    //     ringPositions[i].posY = 10 * 64;
    //     ringSprites[i].setPosition((float)ringPositions[i].posX, (float)ringPositions[i].posY);
        
    //     ringCollected[i] = false;
    // }


    // Music is currently being handled by Menu, might need changing if each level has a different track
    // if (!levelMusic.openFromFile("Data/labrynth.ogg")) {
    //     cout << "Failed to load labrynth.ogg" << endl;
    // }
    // else {
    //     levelMusic.setVolume(30);
    //     levelMusic.setLoop(true);
    //     levelMusic.play();
    // }

    
}

void Level1_Labyrinth::update(float deltaTime) {
    if (ringAnimationClock.getElapsedTime().asSeconds() > 0.15f) {
        ringFrameIndex = (ringFrameIndex + 1) % 4;
        ringAnimationClock.restart();

        ringSprite.setTextureRect(IntRect(ringFrameIndex * 16, 0, 16, 16));

        // for (int i = 0; i < ringCount; i++) {
        //     if (!ringCollected[i]) {
        //         ringSprites[i].setTextureRect(IntRect(ringFrameIndex * 16, 0, 16, 16));
        //     }
        // }
    }
}

void Level1_Labyrinth::render(RenderWindow& window) {
    window.draw(bgSprite);

    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            char cell = grid[i][j];
            if (cell == 'w') {
                wallSprite1.setPosition(j * cellSize, i * cellSize);
                window.draw(wallSprite1);
            }
            else if (cell == 'q') {
                wallSprite2.setPosition(j * cellSize, i * cellSize);
                window.draw(wallSprite2);
            }
            else if (cell == 'r') {
                ringSprite.setPosition(j * cellSize, i * cellSize);
                window.draw(ringSprite);
            }
            // for (int i = 0; i < ringCount; ++i) {
            //     if (!ringCollected[i]) {
            //         ringSprites[i].setPosition((float)ringPositions[i].posX, (float)ringPositions[i].posY); // update position if needed
            //         window.draw(ringSprites[i]);
            //     }
            // }
        }
    }
}


bool Level1_Labyrinth::isLevelComplete() const {
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            if (grid[i][j] == 'r') {
                return false; // If any rings remain, level is not complete
            }
        }
    }
    return true; // all rings collected
}


