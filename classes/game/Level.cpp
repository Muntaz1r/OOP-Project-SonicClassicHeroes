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
    currentPlayer = 's';
    player->setFollowers(tailsMaker.createPlayer(), knucklesMaker.createPlayer());
    player->setCollidingTiles(64, 14, 200, grid);

    batBrain = batBrainMaker.createBatBrain();

    beeBot = beeBotMaker.createBeeBot();

    motoBug = motoBugMaker.createMotoBug();

    crabMeat = crabMeatMaker.createCrabMeat();

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

    player->setCollidingTiles(64, 14, 200, grid);
    player->update(deltaTime, score);

    batBrain->update(deltaTime, player->getPosX(), player->getPosY(), score);
    batBrain->checkCollisionWithPlayer(*player);

    beeBot->update(deltaTime, score);
    beeBot->checkCollisionWithPlayer(*player);
    beeBot->checkProjectilesHitPlayer(*player);

    motoBug->update(deltaTime, player->getPosX(), player->getPosY(), score);
    motoBug->checkCollisionWithPlayer(*player);

    crabMeat->update(deltaTime, score);
    crabMeat->checkCollisionWithPlayer(*player);
    crabMeat->checkProjectilesHitPlayer(*player);

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
    batBrain->render(window, cameraOffsetX);
    beeBot->render(window, cameraOffsetX);
    motoBug->render(window, cameraOffsetX);
    crabMeat->render(window, cameraOffsetX);
}

void Levels::drawUI(sf::RenderWindow& window, float cameraOffset) const {
    static sf::Sprite playerSprite;
    static sf::Sprite heartSprite;
    static bool uiInitialized = true;
    static sf::Texture sonicIcon;
    static sf::Texture tailsIcon;
    static sf::Texture knucklesIcon;
    static sf::Texture heartIcon;
    static sf::Texture emptyHeartIcon;
    static sf::Texture specialAbillityRingTex;
    static sf::Sprite specialAbillityRingSprite;
    static sf::Font uiFont;
    static float iterate = 0;

    if (uiInitialized) {
        if (!sonicIcon.loadFromFile("Data/sonicIcon.png")) {
            cout << "Failed to load Data/sonicIcon.png\n";
        }
        if (!tailsIcon.loadFromFile("Data/tailsIcon.png")) {
            cout << "Failed to load Data/tailsIcon.png\n";
        }
        if (!knucklesIcon.loadFromFile("Data/knucklesIcon.png")) {
            cout << "Failed to load Data/knucklesIcon.png\n";
        }
        if (!heartIcon.loadFromFile("Data/heartIcon.png")) {
            cout << "Failed to load Data/heart.png\n";
        }
        if (!emptyHeartIcon.loadFromFile("Data/emptyHeartIcon.png")) {
            cout << "Failed to load Data/emptyHeartIcon.png\n";
        }
        if (!specialAbillityRingTex.loadFromFile("Data/specialAbillityRing2.png")) {
            cout << "Failed to load ring texture\n";
        }
        if (!uiFont.loadFromFile("Data/Retro Gaming.ttf")) {
            std::cout << "Failed to load Retro Gaming.ttf\n";
        }

        playerSprite.setPosition(15.f, 40.f);  // Top left
        uiInitialized = false;
    }

    // Assign player Icon each frame
    // Assign player Icon each frame
    switch (currentPlayer) {
        case 's':
            playerSprite.setTexture(sonicIcon);
            break;
        case 't':
            playerSprite.setTexture(tailsIcon);
            break;
        case 'k':
            playerSprite.setTexture(knucklesIcon);
            break;
        default:
            return; // Invalid player
    }
    iterate = player->getBoosting()? 30 : 0;

    // Get the texture pointer directly
    const sf::Texture* texture = playerSprite.getTexture();
    if (!texture) return;

    // Get texture dimensions
    unsigned int texWidth = texture->getSize().x;
    unsigned int texHeight = texture->getSize().y;

    // Set texture area
    playerSprite.setTextureRect(sf::IntRect(0, 0, texWidth, texHeight));

    // Calculate uniform scale (assumes square texture)
    float targetSize = 140.f;
    float scale = targetSize / static_cast<float>(texWidth); // assuming square
    playerSprite.setScale(scale, scale);

    // Compute final screen position so that the center is always at (desiredX, desiredY)
    float desiredX = 6.f + targetSize / 2.f + 5.f + iterate/2.f;
    float desiredY = 4.f + targetSize / 2.f + iterate;

    // Compute adjusted top-left position
    float adjustedX = desiredX - (texWidth * scale) / 2.f;
    float adjustedY = desiredY - (texHeight * scale) / 2.f;

    playerSprite.setPosition(adjustedX, adjustedY);

    window.draw(playerSprite);
    

    // Desired display size for hearts (square )
    static const float heartTargetSize = 40.f;
    
    // Draw filled hearts 
    heartSprite.setTexture(heartIcon);
    {
        unsigned texWidth = heartIcon.getSize().x; 
        float scale = heartTargetSize / static_cast<float>(texWidth);
        heartSprite.setScale(scale, scale);

        for (int i = 0; i < Player::getHP(); ++i) {
            heartSprite.setPosition(17 + 40 * i + iterate/1.7f, 140.f + iterate*1.8f);
            window.draw(heartSprite);
        }
    }

    // Draw empty hearts 
    heartSprite.setTexture(emptyHeartIcon);
    {
        unsigned texWidth = emptyHeartIcon.getSize().x; 
        float scale = heartTargetSize / static_cast<float>(texWidth);
        heartSprite.setScale(scale, scale);

        for (int i = Player::getHP(); i < 3; ++i) {
            heartSprite.setPosition(17 + 40 * i + iterate/1.7f, 140.f + iterate*1.8f);
            window.draw(heartSprite);
        }
    }
    


    // Draw special ability ring if boosting
    if (player->getBoosting()) {
        // Assume ring is slightly larger than icon
        const unsigned int specialAbillityRingWidth = specialAbillityRingTex.getSize().x;
        const unsigned int specialAbillityRingHeight = specialAbillityRingTex.getSize().y;

        specialAbillityRingSprite.setTexture(specialAbillityRingTex);
        specialAbillityRingSprite.setTextureRect(sf::IntRect(0, 0, specialAbillityRingWidth, specialAbillityRingHeight));

        // Calculate scale so it's slightly larger than icon
        float specialAbillityRingTargetSize = 260.f; // larger than 140 icon
        float specialAbillityRingScale = specialAbillityRingTargetSize / static_cast<float>(specialAbillityRingWidth);
        specialAbillityRingSprite.setScale(specialAbillityRingScale, specialAbillityRingScale);

        // Same center as icon
        float specialAbillityRingAdjustedX = desiredX - (specialAbillityRingWidth * specialAbillityRingScale) / 2.f;
        float specialAbillityRingAdjustedY = desiredY - (specialAbillityRingHeight * specialAbillityRingScale) / 2.f;

        specialAbillityRingSprite.setPosition(specialAbillityRingAdjustedX, specialAbillityRingAdjustedY);
        window.draw(specialAbillityRingSprite);
    }

    sf::Time elapsed = gameTime.getElapsedTime();
    int minutes = static_cast<int>(elapsed.asSeconds()) / 60;
    int seconds = static_cast<int>(elapsed.asSeconds()) % 60;

    std::string timeText = 
        (minutes < 10 ? "0" : "") + std::to_string(minutes) + ":" +
        (seconds < 10 ? "0" : "") + std::to_string(seconds);

    // Create SFML Text objects
    sf::Text scoreText("Score: " + to_string(score), uiFont, 20);
    sf::Text timeDisplay(timeText, uiFont, 20);

    // Position them in top-right
    scoreText.setFillColor(sf::Color::White);
    timeDisplay.setFillColor(sf::Color::White);

    scoreText.setPosition(1200 - scoreText.getGlobalBounds().width - 10.f, 10.f);
    timeDisplay.setPosition(1200- timeDisplay.getGlobalBounds().width - 10.f, 35.f);

    window.draw(scoreText);
    window.draw(timeDisplay);

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

int Levels::getScore() const {
    return score;
}
sf::Clock Levels::getGameTime() const{
    return gameTime;
}
    


