#pragma once

#include <iostream>
#include <SFML/System/Clock.hpp>
#include <cmath>

#include "DynamicEntity.h"

class Enemy : public DynamicEntity {
protected:
    int hp;
    float maxSpeed;
    bool alive;
    bool movingRight;

    Texture walkRightTexture;
    Texture walkLeftTexture;

public:
    Enemy(float px = 0, float py = 0, int h = 0, int w = 0, sf::Texture* texture = nullptr,
        float vx = 0, float vy = 0, float terminal = 0, int hp = 3, float maxSpeed = 0, bool alive = true, bool movinRight = true)
         : DynamicEntity(px, py, h, w, texture, vx, vy, terminal),
         hp(hp), maxSpeed(maxSpeed), alive(alive), movingRight(movingRight) {}

    virtual void takeDamage(int amount = 1) {
        if (!alive) {
            return;
        }
        hp -= amount;
        if (hp <= 0) {
            alive = false;
            onDeath();
        }
    }

    virtual void onDeath() {
        
    }

    bool isAlive() const { return alive; }
    bool isMovingRight() const { return movingRight; }
    int getHP() const { return hp; }
    void setHP(int hp) { this->hp = hp; }
    void setIsAlive(bool alive) { this->alive = alive; }
    void setIsMovingRight(bool movingRight) { this->movingRight = movingRight; }

    virtual void update(float deltaTime, int &score, int volume) override {
        if (isMovingRight()) {
            if (sprite.getTexture() != &walkRightTexture) {
                sprite.setTexture(walkRightTexture);
            }
            walkRightAnimation.update(deltaTime);
        }
        else {
            if (sprite.getTexture() != &walkLeftTexture) {
                sprite.setTexture(walkLeftTexture);
            }
            walkLeftAnimation.update(deltaTime);
        }
    }

    virtual void checkCollisionWithPlayer(Player& player) {
        if (!isAlive()) return;

        float playerLeft = player.getLeft();
        float playerRight = player.getRight();
        float playerTop = player.getTop();
        float playerBottom = player.getBottom();

        float enemyLeft = getLeft();
        float enemyRight = getRight();
        float enemyTop = getTop();
        float enemyBottom = getBottom();

        bool isOverlapping = playerRight > enemyLeft && playerLeft < enemyRight && playerBottom > enemyTop && playerTop < enemyBottom;

        if (isOverlapping) {
            if (playerBottom <= enemyTop + 10.0f && player.getVelocityY() > 0) {
                takeDamage(1);
                player.setVelocityY(-5.0f); // Bounce effect
                cout << "Enemy took damage." << endl;
            } else if (!player.isInvincible() && player.isOnGround()) {
                player.takeDamage();
                cout << "Player took damage." << endl;
            }
        }
    }

    virtual void render(sf::RenderWindow& window, float cameraOffsetX) override {
        if (isAlive()) {
            Entity::render(window, cameraOffsetX);
        }
    }


};