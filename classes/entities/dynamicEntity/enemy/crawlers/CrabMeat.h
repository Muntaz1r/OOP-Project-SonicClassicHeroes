#pragma once

#include "SFML/System/Clock.hpp"
#include <iostream>
#include <cmath>

#include "Projectile.h"
#include "ProjectileManager.h"
#include "Enemy.h"

class CrabMeat : public Enemy {
private:
    float maxX; // to confine it to a region of the screen
    float minX;
    Clock shootTimer;
    Clock movementClock;
    
    ProjectileManager* projectileManager;

public:
    CrabMeat(float px = 0, float py = 0, int h = 64, int w = 64, sf::Texture* texture = nullptr,
    float vx = 0, float vy = 0, float terminal = 0, float maxX = 0, float minX = 0, int hp = 4, float maxSpeed = 1.0f, bool alive = true, bool movinRight = true)
    : Enemy(px, py, h, w, texture, vx, vy, terminal, hp, maxSpeed, alive, movingRight),
      maxX(maxX), minX(minX) {

        if (!walkRightTexture.loadFromFile("Data/crab_meat.png")) {
            cout << "Failed to load crab_meat.png\n";
        }
        if (!walkLeftTexture.loadFromFile("Data/crab_meat.png")) {
            cout << "Failed to load crab_meat.png\n";
        }

        sprite.setTexture(walkRightTexture);
        sprite.setTextureRect(sf::IntRect(0, 0, 64, 64)); // single frame
        //sprite.setScale(2.0f, 2.0f);

        // Setup animation for walking
        walkRightAnimation.initialize(&sprite, &walkRightTexture, 32, 32, 10, 0.05f);
        walkLeftAnimation.initialize(&sprite, &walkLeftTexture, 32, 32, 10, 0.05f);

        if (!texture->loadFromFile("Data/projectileR.png")) {
            cout << "Unable to load projectile.png" << endl;
        }

        projectileManager = new ProjectileManager();
    }

    void update(float deltaTime, int &score) override {
        Enemy::update(deltaTime, score);

        if (!isAlive()) {
            return;
        }

        pos_x += movingRight ? velocity_x : -velocity_x;

        if (isMovingRight()) {
            pos_x += velocity_x;
        }
        else {
            pos_x += -velocity_x;
        }

        // if (pos_x > maxX) {
        //     pos_x = maxX;
        //     movingRight = false;
        // }
        // else if (pos_x < minX) {
        //     pos_x = minX;
        //     movingRight = true;
        // }

        if (shootTimer.getElapsedTime().asSeconds() > 3.0f) {
            if (isMovingRight()) { // shoot right when moving left and vice versa
                projectileManager->spawn(pos_x, pos_y, false, false);
            }
            else {
                projectileManager->spawn(pos_x, pos_y, false, true);
            }
            
            shootTimer.restart();
        }
        projectileManager->update(deltaTime, minX, maxX); // display the projectile over the entire screen
    }

    void onDeath() override {
        cout << "Crab Meat destroyed!" << endl;
        projectileManager->clear();
    }

    void render(RenderWindow& window, float cameraOffsetX) {
        Enemy::render(window, cameraOffsetX);
        projectileManager->render(window, cameraOffsetX);
    }

    void checkProjectilesHitPlayer(Player& player) {
        projectileManager->checkCollisionWithPlayer(player);
    }
};
