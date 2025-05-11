#include "MotoBug.h"

MotoBug::MotoBug(float px, float py, int h, int w, sf::Texture* texture,
float vx, float vy, float terminal, float maxX, float minX, int hp, float maxSpeed, bool alive, bool movinRight)
: Enemy(px, py, h, w, texture, vx, vy, terminal, hp, maxSpeed, alive, movingRight),
    maxX(maxX), minX(minX) {

    if (!walkRightTexture.loadFromFile("Data/moto_bugR.png")) {
        cout << "Failed to load moto_bugR.png\n";
    }
    if (!walkLeftTexture.loadFromFile("Data/moto_bugL.png")) {
        cout << "Failed to load moto_bugL.png\n";
    }

    sprite.setTexture(walkRightTexture);
    sprite.setTextureRect(sf::IntRect(0, 0, 32, 32)); // single frame
    sprite.setScale(2.0f, 2.0f);

    // Setup animation for walking
    walkRightAnimation.initialize(&sprite, &walkRightTexture, 32, 32, 12, 0.1f);
    walkLeftAnimation.initialize(&sprite, &walkLeftTexture, 32, 32, 12, 0.1f);
}

void MotoBug::update(float deltaTime, float playerX, float playerY, int &score, int volume) {
    Enemy::update(deltaTime, score, volume);

    if (!alive) return;

    float dx = playerX - pos_x;

    // For animation
    if (dx < 0) {
        movingRight = false;
    }
    else {
        movingRight = true;
    }

    float distance = sqrt(dx * dx);
    if (distance > 0.7f) { // small distance to avoid jitteryness
        pos_x += (dx / distance) * velocity_x;
    }

    if (pos_x < minX) {
        pos_x = minX;
    }
    else if (pos_x > maxX) {
        pos_x = maxX;
    }
}

void MotoBug::onDeath() {
    cout << "MotoBug destroyed!\n";
}

float MotoBug::getMinX() const { return minX; }
float MotoBug::getMaxX() const { return maxX; }

void MotoBug::setMinX(float minX) { this->minX = minX; }
void MotoBug::setMaxX(float maxX) { this->maxX = maxX; }