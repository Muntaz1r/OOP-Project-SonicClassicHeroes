#include "BossLevel.h"
#include <iostream>
#include <fstream>

BossLevel::BossLevel(): Level(19, 0, 0, 0, 0) {
    levelID = 4;
    waitingAtBottom = false;

    if (!bgTex.loadFromFile("Data/eggstinger/bg.png")) {
        cout << "Failed to load to eggstinger/bg.png" << endl;
    }

    bgSprite.setTexture(bgTex);

    if (!wallTex1.loadFromFile("Data/eggstinger/brick.png")) {
        cout << "Failed to load brick1.png" << endl;
    }

    wallSprite1.setTexture(wallTex1);
    wallSprite1.setScale(2.0f, 2.0f);

    initializeGrid();
}

BossLevel::~BossLevel() {
    if (eggStinger) {
        delete eggStinger;
        eggStinger = nullptr;
    }
}

void BossLevel::initializeGrid() {
    if (!loadFromFile("Data/BossLevel_Layout.txt")) {
        cout << "Unable to load level layout file!" << endl;
    }
}

void BossLevel::loadAssets(int volume) {
    levelVolume = volume;
    levelSounds->setVolume(levelVolume);
    // acc_x, acc_y, friction, gravity
    player = sonicMaker.createPlayer(0.5f, 13.0f, 0.2f, 0.4f);
    player->setLeader(true);
    currentPlayer = 's';
    player->setFollowers(tailsMaker.createPlayer(0.47f, 13.0f, 0.2f, 0.4f), knucklesMaker.createPlayer(0.42f, 14.0f, 0.2f, 0.4f));
    player->setCollidingTiles(64, 14, 200, grid);

    eggStinger = eggStingerMaker.createEggStinger(576.0f, 64.0f, 1040.0f, 0.0f);
    eggStinger->checkCollisionWithPlayer(*player);

    if (!levelMusic.openFromFile("Data/BossLevel.ogg")) {
        cout << "Failed to load BossLevel.ogg" << endl;
    }
    else {
        levelMusic.setVolume(volume);
        levelMusic.setLoop(true);
        levelMusic.play();
    }
}

void BossLevel::update(float deltaTime) {
    Level::update(deltaTime);
    
    eggStinger->update(deltaTime, player->getPosX(), score, levelVolume);
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
    cameraOffsetX = 0;
    Level::render(window, cameraOffsetX);
    eggStinger->render(window, cameraOffsetX);
}

bool BossLevel::exitCheck(float cameraOffSetX){
    completed = !eggStinger->isAlive();
    failed = (Player::getHP() == 0);
    return completed || failed;
}

bool BossLevel::isLevelComplete() const{
    return completed;
}
bool BossLevel::getFailed(){
    return failed;
}