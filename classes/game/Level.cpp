#include "Level.h"
#include <iostream>
#include <fstream>
#include <ctime> 

bool Level::loadFromFile(const string& filePath) {
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
            // if(line[j] == 'R')
            //     grid[i][j] = 'u'; // for testing
            // else
                grid[i][j] = line[j];


        }
    }

    myFile.close();
    return true;
}

void Level::update(float deltaTime) {
    ringAnimation.update(deltaTime);

    if (Keyboard::isKeyPressed(Keyboard::D)) {
        player->moveRight();
    }
    else if (Keyboard::isKeyPressed(Keyboard::A)) {
        player->moveLeft();
    }
    
    if (Keyboard::isKeyPressed(Keyboard::W)) {
        player->jump();
        levelSounds->play(0);
    }
    if (Keyboard::isKeyPressed(Keyboard::S)) {
        player->flyDown();
    }
    if (Keyboard::isKeyPressed(Keyboard::F) && player->isOnGround()) {
        player->specialAbility();
    }
    if (Keyboard::isKeyPressed(Keyboard::Z) && !player->getBoosting()) {
        if (switchCooldownClock.getElapsedTime().asSeconds() >= 1.0f) {
            levelSounds->play(1);
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
    player->update(deltaTime, score, levelVolume);

    for (int i = 0; i < numBatBrain; i++) {
        batBrains[i]->update(deltaTime, player->getPosX(), player->getPosY(), score, levelVolume);
        batBrains[i]->checkCollisionWithPlayer(*player);
    }

    for (int i = 0; i < numBeeBot; i++) {
        beeBots[i]->update(deltaTime, score, levelVolume);
        beeBots[i]->checkCollisionWithPlayer(*player);
        beeBots[i]->checkProjectilesHitPlayer(*player);
    }

    for (int i = 0; i < numMotoBug; i++) {
        motoBugs[i]->update(deltaTime, player->getPosX(), player->getPosY(), score, levelVolume);
        motoBugs[i]->checkCollisionWithPlayer(*player);
    }
    
    for (int i = 0; i < numCrabMeat; i++) {
        crabMeats[i]->update(deltaTime, score, levelVolume);
        crabMeats[i]->checkCollisionWithPlayer(*player);
        crabMeats[i]->checkProjectilesHitPlayer(*player);
    }
}

void Level::render(RenderWindow& window, float cameraOffsetX) {
    srand(static_cast<unsigned int>(time(nullptr)));
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
            }else if (cell == 's') {
                if(specialItemCount <= 2){
                    specialItemCount++;
                    int r = rand() % 100;
                
                    // 'l' has 1 in 500 chance -> values 0–2
                    // 'u' has 1 in 1500 chance -> value 3
                
                    if (r < 3)        // 0,1,2 (3 values out of 1500)
                        grid[i][j] = 'l'; // 1 in 500
                    else if (r == 3)  // 1 value out of 1500
                        grid[i][j] = 'u'; // 1 in 1500
                }
            }
            else if (cell == 'l') {
                extraLifeSprite.setPosition(drawX, drawY);
                window.draw(extraLifeSprite);
            }
            else if (cell == 'u') {
                powerUpSprite.setPosition(drawX, drawY);
                window.draw(powerUpSprite);    
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

bool Level::isLevelComplete() const {
    // add logic here
    return false;
}

float Level::getPlayerX() const {
    return player->getPosX();
}

int Level::getPlayerWidth() const {
    return player->getWidth();
}

int Level::getLevelWidthinTiles() const {
    return width;
}

int Level::getCellSize() const {
    return cellSize;
}


void Level::drawUI(sf::RenderWindow& window, float cameraOffset) const {
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

bool Level::exitCheck(float cameraOffSetX){
    //Exit portal check

    float portalX = width * cellSize - 40 - cameraOffSetX;
    static const float portalY = 450;

    float playerLeft   = player->getPosX() - cameraOffSetX;
    float playerRight  = playerLeft + player->getWidth()/2;
    float playerTop    = player->getPosY();
    float playerBottom = playerTop + player->getHeight();
    float portalLeft   = width * cellSize - 40 - cameraOffSetX + 25;
    float portalRight  = portalLeft + portalTex.getSize().x - 100;
    float portalTop    = 450 + 50;
    float portalBottom = portalTop + portalTex.getSize().y - 50;

    bool xOverlap = playerRight >= portalLeft  && playerLeft <= portalRight ;
    bool yOverlap = playerBottom >= portalTop  && playerTop <= portalBottom ;


    if (xOverlap && yOverlap) {
        cout << "Exit level" << endl;
        return true;
    }

    return false;
}

int Level::getScore() const {
    return score;
}
sf::Clock Level::getGameTime() const{
    return gameTime;
}
    


