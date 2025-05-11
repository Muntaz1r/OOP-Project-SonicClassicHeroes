#pragma once
#include "Enemy.h"
#include <iostream>

class MotoBug : public Enemy {
private:
    float maxX; // to confine it to a region of the screen
    float minX;

public:
    MotoBug(float px = 0, float py = 0, int h = 0, int w = 0, sf::Texture* texture = nullptr,
    float vx = 0, float vy = 0, float terminal = 0, float maxX = 0, float minX = 0, int hp = 2, float maxSpeed = 0, bool alive = true, bool movinRight = true);

    virtual void update(float deltaTime, float playerX, float playerY, int &score, int volume);

    void onDeath();

    float getMinX() const;
    float getMaxX() const;

    void setMinX(float minX);
    void setMaxX(float maxX);
};
