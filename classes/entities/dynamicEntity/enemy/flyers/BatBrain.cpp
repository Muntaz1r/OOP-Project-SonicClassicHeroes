#include "BatBrain.h"

BatBrain::BatBrain(float px, float py, int h, int w, sf::Texture* texture,
float vx, float vy, float terminal, int hp, float maxSpeed, bool alive, bool movinRight)
: Enemy(px, py, h, w, texture, vx, vy, terminal, hp, maxSpeed, alive, movingRight) {

    if (!walkRightTexture.loadFromFile("Data/bat_brainR.png")) {
        cout << "Failed to load bat_brainR.png\n";
    }
    if (!walkLeftTexture.loadFromFile("Data/bat_brainL.png")) {
        cout << "Failed to load bat_brainR.png\n";
    }

    sprite.setTexture(walkRightTexture);
    sprite.setTextureRect(sf::IntRect(0, 0, 32, 32)); // single frame
    sprite.setScale(2.0f, 2.0f);

    // Setup animation for walking
    walkRightAnimation.initialize(&sprite, &walkRightTexture, 32, 32, 9, 0.09f);
    walkLeftAnimation.initialize(&sprite, &walkLeftTexture, 32, 32, 9, 0.09f);
}

void BatBrain::update(float deltaTime, float playerX, float playerY, int &score, int volume) {
    Enemy::update(deltaTime, score, volume);

    if (!alive) return;

    float dx = playerX - pos_x;
    float dy = playerY - pos_y;

    // For animation
    if (dx < 0) {
        movingRight = false;
    }
    else {
        movingRight = true;
    }

    float distance = sqrt(dx * dx + dy * dy);
    if (distance > 0.5) { // small distance to avoid jitteryness
        pos_x += (dx / distance) * velocity_x;
        pos_y += (dy / distance) * velocity_y;
    }
}

void BatBrain::onDeath() {
    cout << "BatBrain destroyed!\n";
}