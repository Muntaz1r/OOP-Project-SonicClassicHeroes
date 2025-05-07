#pragma once

#include <iostream>
#include <SFML/Graphics.hpp>

#include "DynamicEntity.h"
#include "Player.h"

using namespace std;
using namespace sf;

class Projectile : public DynamicEntity {
private:
    int damage;

    bool active;
    bool movingDown;
    bool movingRight;

    Texture projectileTextureD;
    Texture projectileTextureR;
    Texture projectileTextureL;

public:
    Projectile(float px, float py, int h = 0, int w = 0, Texture* texture = nullptr, float vx = 0, float vy = 0, float terminal = 0, int dmg = 1, bool active = true, bool down = false, bool right = true)
    : DynamicEntity(px, py, h, w, texture, vx, vy, terminal),
    damage(damage), active(active), movingDown(down), movingRight(right) {
        if (!projectileTextureD.loadFromFile("Data/projectileD.png")) {
            cout << "Unable to load projectileD.png" << endl;
        }

        if (!projectileTextureR.loadFromFile("Data/projectileR.png")) {
            cout << "Unable to load projectileR.png" << endl;
        }

        if (!projectileTextureL.loadFromFile("Data/projectileL.png")) {
            cout << "Unable to load projectileL.png" << endl;
        }

        sprite.setTexture(this->texture);
        sprite.setScale(2.0f, 2.0f);
        sprite.setTextureRect(IntRect(0, 0, width, height));
    }

    void update(float deltaTime) {}
    void update(float deltaTime, float minX, float maxX) {
        if (!isActive()) {
            return;
        }

        pos_x += velocity_x * deltaTime;
        pos_y += velocity_y * deltaTime;

        if (pos_x < minX || pos_x > maxX || pos_y > 960.0f) { // off the map
            active = false;
        }

        if (isMovingDown()) {
            if (sprite.getTexture() != &projectileTextureD) {
                sprite.setTexture(projectileTextureD);
            }
        }
        else {
            if (isMovingRight()) {
                if (sprite.getTexture() != &projectileTextureR) {
                    sprite.setTexture(projectileTextureR);
                }
            }
            else {
                if (sprite.getTexture() != &projectileTextureL) {
                    sprite.setTexture(projectileTextureL);
                }
            }
        }
    }

    void render(RenderWindow& window, float cameraOffsetX) {
        if (isActive()) {
            Entity::render(window, cameraOffsetX);
        }
    } 

    // Getters
    bool isActive() const { return active; }
    bool isMovingDown() const { return movingDown; }
    bool isMovingRight() const { return movingRight; }

    void deactivate() { active = false; }


    void checkCollision(Player& player) {
        float playerLeft = player.getLeft();
        float playerRight = player.getRight();
        float playerTop = player.getTop();
        float playerBottom = player.getBottom();

        float projectileLeft = getLeft();
        float projectileRight = getRight();
        float projectileTop = getTop();
        float projectileBottom = getBottom();

        bool isOverlapping = playerRight > projectileLeft && playerLeft < projectileRight && playerBottom > projectileTop && playerTop < projectileBottom;
        
        if (isOverlapping) {
            player.takeDamage();
            deactivate();
            cout << "Player took damage from bullet." << endl;
        }
    }
};