#include "Level.h"
#include <iostream>
#include <fstream>

Level1_Labyrinth::Level1_Labyrinth() {
    completed = false;
    failed = false;

    grid = new char*[height];
    for (int i = 0; i < height; i++) {
        grid[i] = new char[width];
    }

    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            grid[i][j] = 's';
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

bool Level1_Labyrinth::loadFromFile(const string& filePath) {
    ifstream myFile(filePath);

    if (!myFile.is_open()) {
        cout << "Unable to open level layout file " << filePath << endl;
        return false;
    }

    for (int i = 0; i < height; i++) {
        string line;
        
        if (!getline(myFile, line)) {
            cout << "Not enough lines to fill the level!" << endl;
            return false;
        }

        for (int j = 0; j < width && j < line.length(); j++) {
            grid[i][j] = line[j];
        }
    }

    myFile.close();
    return true;
}

void Level1_Labyrinth::initializeGrid() {
    // 's' - space
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

    if (!wallTex3.loadFromFile("Data/brick3.png")) {
        cout << "Failed to load brick3.png" << endl;
    }

    wallSprite3.setTexture(wallTex3);

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

    ringAnimation.initialize(&ringSprite, &ringTex, 16, 16, 4, 0.15f);

    player = sonicMaker.createPlayer();
    player->setLeader(true);
    player->setFollowers(tailsMaker.createPlayer(), knucklesMaker.createPlayer());
    player->setCollidingTiles(64, 14, 200, grid);


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
    ringAnimation.update(deltaTime);

    if (Keyboard::isKeyPressed(Keyboard::D)) {
        player->moveRight();
    }
    else if (Keyboard::isKeyPressed(Keyboard::A)) {
        player->moveLeft();
    }
    
    if (Keyboard::isKeyPressed(Keyboard::W) && player->isOnGround()) {
        player->jump();
    }
    if (Keyboard::isKeyPressed(Keyboard::F) && player->isOnGround()) {
        player->specialAbility();
    }
    if (Keyboard::isKeyPressed(Keyboard::Z)) {
        if (switchCooldownClock.getElapsedTime().asSeconds() >= 5.0f){
        switchCooldownClock.restart();
        cout<<"Switch\n";
        Player* temp;
        player->setLeader(false); // Old leader no longer leader
       
        temp = player;
        player = temp->getFollower1();
        player->setFollowers(temp->getFollower2(), temp);
        player->setLeader(true);

        temp->setFollowers(nullptr, nullptr); // Old leaders followers null
        }
    }
    player->setCollidingTiles(64, 14, 200, grid);
    player->update(deltaTime);



    // if (ringAnimationClock.getElapsedTime().asSeconds() > 0.15f) {
    //     ringFrameIndex = (ringFrameIndex + 1) % 4;
    //     ringAnimationClock.restart();

    //     ringSprite.setTextureRect(IntRect(ringFrameIndex * 16, 0, 16, 16));

    //     // for (int i = 0; i < ringCount; i++) {
    //     //     if (!ringCollected[i]) {
    //     //         ringSprites[i].setTextureRect(IntRect(ringFrameIndex * 16, 0, 16, 16));
    //     //     }
    //     // }
    // }
}

void Level1_Labyrinth::render(RenderWindow& window, float cameraOffsetX) {
    window.draw(bgSprite);

    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            char cell = grid[i][j];
            float drawX = j * cellSize - cameraOffsetX;
            float drawY = i * cellSize;

            if (cell == 'w') {
                wallSprite1.setPosition(drawX, drawY);
                window.draw(wallSprite1);
            }
            else if (cell == 'q') {
                wallSprite2.setPosition(drawX, drawY);
                window.draw(wallSprite2);
            }
            else if (cell == 'b') {
                wallSprite3.setPosition(drawX, drawY);
                window.draw(wallSprite3);
            }
            else if (cell == 'x') {
                spikeSprite.setPosition(drawX, drawY);
                window.draw(spikeSprite);
            }
            else if (cell == 'C') {
                crystalSprite.setPosition(drawX, drawY);
                window.draw(crystalSprite);
            }
            else if (cell == 'R') {
                ringSprite.setPosition(drawX, drawY);
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

    player->render(window, cameraOffsetX);
}


bool Level1_Labyrinth::isLevelComplete() const {
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            if (grid[i][j] == 'R') {
                return false; // If any rings remain, level is not complete
            }
        }
    }
    return true; // all rings collected
}

float Level1_Labyrinth::getPlayerX() const {
    return player->getPosX();
}

int Level1_Labyrinth::getPlayerWidth() const {
    return player->getWidth();
}

int Level1_Labyrinth::getLevelWidthinTiles() const {
    return width;
}

int Level1_Labyrinth::getCellSize() const {
    return cellSize;
}


