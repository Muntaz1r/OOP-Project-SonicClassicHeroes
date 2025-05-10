#include "BossLevel.h"
#include <iostream>
#include <fstream>

BossLevel::BossLevel() {
    completed = false;
    failed = false;
    waitingAtBottom = false;

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

BossLevel::~BossLevel() {
    for (int i = 0; i < height; i++) {
        delete grid[i];
    }
    delete[] grid;
}

bool BossLevel::loadFromFile(const string& filePath) {
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

void BossLevel::initializeGrid() {
    if (!loadFromFile("Data/BossLevel_Layout.txt")) {
        cout << "Unable to load level layout file!" << endl;
    }
}

void BossLevel::loadAssets() {
    if (!bgTex.loadFromFile("Data/eggstinger/bg.png")) {
        cout << "Failed to load to eggstinger/bg.png" << endl;
    }

    bgSprite.setTexture(bgTex);

    if (!wallTex.loadFromFile("Data/eggstinger/brick.png")) {
        cout << "Failed to load brick1.png" << endl;
    }

    wallSprite.setTexture(wallTex);
    wallSprite.setScale(2.0f, 2.0f);

    // acc_x, acc_y, friction, gravity
    player = sonicMaker.createPlayer(0.5f, 13.0f, 0.2f, 0.4f);
    player->setLeader(true);
    currentPlayer = 's';
    player->setFollowers(tailsMaker.createPlayer(0.47f, 13.0f, 0.2f, 0.4f), knucklesMaker.createPlayer(0.42f, 14.0f, 0.2f, 0.4f));
    player->setCollidingTiles(64, 14, 200, grid);

    eggStinger = eggStingerMaker.createEggStinger(576.0f, 64.0f, 1040.0f, 0.0f);
    eggStinger->checkCollisionWithPlayer(*player);

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

void BossLevel::update(float deltaTime) {

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

    eggStinger->update(deltaTime, player->getPosX(), score);
    eggStinger->checkCollisionWithPlayer(*player);

    if (eggStinger->isDropping() && eggStinger->getPosY() > 670.0f) {
        waitingAtBottom = true;
        int groundX = eggStinger->getTargetIndexX();
        int groundY = 13;
        cout << "Break Ground at " << groundX << endl;
        if (waitClock.getElapsedTime().asSeconds() > 1.5f) {
            waitingAtBottom = false;
            waitClock.restart();
        }
        if (grid[groundY][groundX] == 'w') {
            grid[groundY][groundX] = 'p';
        }
    }
}

void BossLevel::render(RenderWindow& window, float cameraOffsetX) {
    cameraOffsetX = 0; // since the level is static
    window.draw(bgSprite);
    eggStinger->render(window, cameraOffsetX);

    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            char cell = grid[i][j];
            float drawX = j * cellSize - cameraOffsetX;
            float drawY = i * cellSize;

            if (cell == 'w') {
                wallSprite.setPosition(drawX, drawY);
                window.draw(wallSprite);
            }
        }
    }
    player->render(window, cameraOffsetX);
}

bool BossLevel::isLevelComplete() const {
    return false;
}

float BossLevel::getPlayerX() const {
    return player->getPosX();
}

int BossLevel::getPlayerWidth() const {
    return player->getWidth();
}

int BossLevel::getLevelWidthinTiles() const {
    return width;
}

int BossLevel::getCellSize() const {
    return cellSize;
}