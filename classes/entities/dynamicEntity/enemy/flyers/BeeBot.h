#pragma once

#include "SFML/System/Clock.hpp"
#include <iostream>
#include <cmath>

#include "Enemy.h"
#include "Projectile.h"
#include "ProjectileManager.h"

class BeeBot : public Enemy {
private:
    float zigzagAmp;
    float zigzagSpeed;
    float originalY; // for translating it on y-axis
    float maxX; // to confine it to a region of the screen
    float minX;
    Clock shootTimer;
    Clock movementClock; 
    
    ProjectileManager* projectileManager;

public:
    BeeBot(float px = 0, float py = 0, int h = 64, int w = 64, sf::Texture* texture = nullptr,
    float vx = 0, float vy = 0, float terminal = 0, float maxX = 0, float minX = 0, int hp = 5, float maxSpeed = 1.0f, bool alive = true, bool movinRight = true)
    : Enemy(px, py, h, w, texture, vx, vy, terminal, hp, maxSpeed, alive, movingRight),
      maxX(maxX), minX(minX), zigzagAmp(50.0f), zigzagSpeed(3.0f), originalY(py) {

        if (!walkRightTexture.loadFromFile("Data/bee_botR.png")) {
            cout << "Failed to load bat_brainR.png\n";
        }
        if (!walkLeftTexture.loadFromFile("Data/bee_botL.png")) {
            cout << "Failed to load bat_brainL.png\n";
        }

        sprite.setTexture(walkRightTexture);
        sprite.setTextureRect(sf::IntRect(0, 0, 64, 64)); // single frame
        //sprite.setScale(2.0f, 2.0f);

        // Setup animation for walking
        walkRightAnimation.initialize(&sprite, &walkRightTexture, 64, 64, 6, 0.07f);
        walkLeftAnimation.initialize(&sprite, &walkLeftTexture, 64, 64, 6, 0.07f);

        if (!texture->loadFromFile("Data/projectileD.png")) {
            cout << "Unable to load projectileD.png" << endl;
        }

        projectileManager = new ProjectileManager();
    }

    void update(float deltaTime, int &score, int volume) override {
        Enemy::update(deltaTime, score, volume);

        if (!isAlive()) {
            return;
        }

        float time = movementClock.getElapsedTime().asSeconds();
        
        // using the shm formula y = Asin(wt)
        pos_y = originalY + zigzagAmp * sin(zigzagSpeed * time);

        pos_x += movingRight ? velocity_x : -velocity_x;

        if (isMovingRight()) {
            pos_x += velocity_x;
        }
        else {
            pos_x += -velocity_x;
        }

        if (pos_x > maxX) {
            pos_x = maxX;
            movingRight = false;
        }
        else if (pos_x < minX) {
            pos_x = minX;
            movingRight = true;
        }

        if (shootTimer.getElapsedTime().asSeconds() > 5.0f) {
            projectileManager->spawn(pos_x, pos_y, true, false); // BeeBot only shoots downwards so down = true
            shootTimer.restart();
        }
        projectileManager->update(deltaTime, minX, maxX); // display the projectile over the confined region
    }

    void onDeath() override {
        cout << "Bee Bot destroyed!" << endl;
        projectileManager->clear();
    }

    void render(RenderWindow& window, float cameraOffsetX) {
        Enemy::render(window, cameraOffsetX);
        projectileManager->render(window, cameraOffsetX);
    }

    void checkProjectilesHitPlayer(Player& player) {
        projectileManager->checkCollisionWithPlayer(player);
    }

    float getMinX() const { return minX; }
    float getMaxX() const { return maxX; }
    void setMinX(float minX) { this->minX = minX; }
    void setMaxX(float maxX) { this->maxX = maxX; }
};
