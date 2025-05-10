#include "Level.h"
#include <iostream>
#include <fstream>


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


bool Levels::exitCheck(float cameraOffSetX){ //made in level1_labyrinth
    // //Exit portal
    // if(player->getPosY() >= 600 && player->getPosY() >= (600 + portalTex.getSize().y)){ //Y intercept
    //     if((player->getPosX() >= width*cellSize - exit - cameraOffSetX - portalTex.getSize().x)
    //     && (player->getPosX() <= width*cellSize - exit - cameraOffSetX)){ //X intercept
    //         cout<<"Exit level";
    //         return true;
    //     }
    // }
    return false;
}


int Levels::getScore() const {
    return score;
}
sf::Clock Levels::getGameTime() const{
    return gameTime;
}
    


