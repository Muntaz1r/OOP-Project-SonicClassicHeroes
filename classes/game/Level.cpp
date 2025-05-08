#include "Level.h"

void Levels::drawUI(sf::RenderWindow& window, float cameraOffset) const {
    static sf::Sprite playerSprite;
    static sf::Sprite heartSprite;
    static bool uiInitialized = true;
    static sf::Texture sonicIcon;
    static sf::Texture tailsIcon;
    static sf::Texture knucklesIcon;
    static sf::Texture heartIcon;
    static sf::Texture emptyHeartIcon;

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

        playerSprite.setPosition(8.f, 8.f);  // Top left
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

    // Get the texture pointer directly
    const sf::Texture* texture = playerSprite.getTexture();
    if (!texture) return;

    // Get texture dimensions without using Vector2u
    unsigned int texWidth = texture->getSize().x;
    unsigned int texHeight = texture->getSize().y;

    // Set texture rect to full texture area
    playerSprite.setTextureRect(sf::IntRect(0, 0, texWidth, texHeight));

    // Calculate scale for consistent visual size
    float targetSize = player->getBoosting() ? 112.f : 96.f;
    float scale = targetSize / static_cast<float>(texWidth); // assumes square textures
    playerSprite.setScale(scale, scale);

    window.draw(playerSprite);
    

    // Set constant heart scale
    heartSprite.setScale(0.4f, 0.4f);

    // Draw filled hearts
    heartSprite.setTexture(heartIcon);
    for (int i = 0; i < Player::getHP(); ++i) {
        heartSprite.setPosition(220.f + 80 * i, 50.f);
        window.draw(heartSprite);
    }

    // Draw empty hearts
    for (int i = Player::getHP(); i < 3; ++i) {
        heartSprite.setTexture(emptyHeartIcon);
        heartSprite.setPosition(220.f + 80 * i, 50.f);
        window.draw(heartSprite);
    }
}