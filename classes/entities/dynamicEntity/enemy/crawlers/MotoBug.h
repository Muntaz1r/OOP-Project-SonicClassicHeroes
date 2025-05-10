#pragma once
#include "Enemy.h"
#include <iostream>

class MotoBug : public Enemy {
private:
    float maxX; // to confine it to a region of the screen
    float minX;

public:
    MotoBug(float px = 0, float py = 0, int h = 0, int w = 0, sf::Texture* texture = nullptr,
    float vx = 0, float vy = 0, float terminal = 0, float maxX = 0, float minX = 0, int hp = 2, float maxSpeed = 0, bool alive = true, bool movinRight = true)
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

    virtual void update(float deltaTime, float playerX, float playerY, int &score, int volume) {
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

    void onDeath() {
        cout << "MotoBug destroyed!\n";
    }

    float getMinX() const { return minX; }
    float getMaxX() const { return maxX; }
    void setMinX(float minX) { this->minX = minX; }
    void setMaxX(float maxX) { this->maxX = maxX; }
};
