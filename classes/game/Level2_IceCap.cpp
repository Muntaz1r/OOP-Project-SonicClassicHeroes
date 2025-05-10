#include "Level2_IceCap.h"
#include <iostream>
#include <fstream>

Level2_IceCap::Level2_IceCap() {
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

    batBrains = new BatBrain*[numBatBrain];
    beeBots = new BeeBot*[numBeeBot];
    motoBugs = new MotoBug*[numMotoBug];
    crabMeats = new CrabMeat*[numCrabMeat];
    
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

    initializeGrid();
}

Level2_IceCap::~Level2_IceCap() {
    for (int i = 0; i < height; i++) {
        delete grid[i];
    }
    delete[] grid;
}

bool Level2_IceCap::loadFromFile(const string& filePath) {
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

void Level2_IceCap::loadAssets() {
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

    if(!portalTex.loadFromFile("Data/portal.png")){
        cout<<"Failed to load portal.png";
    }

    portalSprite.setTexture(portalTex);
    portalSprite.setScale(0.75f,0.75f);

    ringSprite.setTexture(ringTex);
    ringSprite.setScale(4.0f, 4.0f);

    ringAnimation.initialize(&ringSprite, &ringTex, 16, 16, 4, 0.15f);

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

void Level2_IceCap::update(float deltaTime) {
    ringAnimation.update(deltaTime);

    if (Keyboard::isKeyPressed(Keyboard::D)) {
        player->moveRight();
    }
    else if (Keyboard::isKeyPressed(Keyboard::A)) {
        player->moveLeft();
    }
    
    if (Keyboard::isKeyPressed(Keyboard::W)) {
        player->jump();
    }
    if (Keyboard::isKeyPressed(Keyboard::S)) {
        player->flyDown();
    }
    if (Keyboard::isKeyPressed(Keyboard::F) && player->isOnGround()) {
        player->specialAbility();
    }
    if (Keyboard::isKeyPressed(Keyboard::Z) && !player->getBoosting()) {

        if (switchCooldownClock.getElapsedTime().asSeconds() >= 1.0f){
            player->setBoosting(false);
            switchCooldownClock.restart();
            switch (currentPlayer) {
            case 's':
                currentPlayer ='t';
                break;
            case 't':
                currentPlayer ='k';
                break;
            case 'k':
                currentPlayer ='s';
                break;
            default:
                return; // Invalid player
        }
    
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

    // When tails is flying, turn sonic and knuckles into hanging state 
    if (currentPlayer == 't' && player->getBoosting()) {
        player->getFollower1()->setHanging(true);
        player->getFollower2()->setHanging(true);
        if (player->isMovingRight()) {
            player->getFollower1()->setHangingRight(true);
            player->getFollower2()->setHangingRight(true);
        }
        else {
            player->getFollower1()->setHangingRight(false);
            player->getFollower2()->setHangingRight(false);
        }
    }
    else {
        player->getFollower1()->setHanging(false);
        player->getFollower2()->setHanging(false);
    }

    player->setCollidingTiles(cellSize, height, width, grid);
    player->update(deltaTime, score);

    for (int i = 0; i < numBatBrain; i++) {
        batBrains[i]->update(deltaTime, player->getPosX(), player->getPosY(), score);
        batBrains[i]->checkCollisionWithPlayer(*player);
    }

    for (int i = 0; i < numBeeBot; i++) {
        beeBots[i]->update(deltaTime, score);
        beeBots[i]->checkCollisionWithPlayer(*player);
        beeBots[i]->checkProjectilesHitPlayer(*player);
    }

    for (int i = 0; i < numMotoBug; i++) {
        motoBugs[i]->update(deltaTime, player->getPosX(), player->getPosY(), score);
        motoBugs[i]->checkCollisionWithPlayer(*player);
    }
    
    for (int i = 0; i < numCrabMeat; i++) {
        crabMeats[i]->update(deltaTime, score);
        crabMeats[i]->checkCollisionWithPlayer(*player);
        crabMeats[i]->checkProjectilesHitPlayer(*player);
    }
}

void Level2_IceCap::render(RenderWindow& window, float cameraOffsetX) {
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
        }
    }

    player->render(window, cameraOffsetX);

    for (int i = 0; i < numBatBrain; i++) {
        batBrains[i]->render(window, cameraOffsetX);
    }

    for (int i = 0; i < numBeeBot; i++) {
        beeBots[i]->render(window, cameraOffsetX);
    }

    for (int i = 0; i < numMotoBug; i++) {
        motoBugs[i]->render(window, cameraOffsetX);
    }
    
    for (int i = 0; i < numCrabMeat; i++) {
        crabMeats[i]->render(window, cameraOffsetX);
    }
}




bool Level2_IceCap::isLevelComplete() const {
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            if (grid[i][j] == 'R') {
                return false; // If any rings remain, level is not complete
            }
        }
    }
    return true; // all rings collected
}

float Level2_IceCap::getPlayerX() const {
    return player->getPosX();
}

int Level2_IceCap::getPlayerWidth() const {
    return player->getWidth();
}

int Level2_IceCap::getLevelWidthinTiles() const {
    return width;
}

int Level2_IceCap::getCellSize() const {
    return cellSize;
}