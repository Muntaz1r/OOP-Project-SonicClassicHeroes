#pragma once
#include "Enemy.h"
#include <iostream>

class BatBrain : public Enemy {
private:


public:
    BatBrain(float px = 0, float py = 0, int h = 0, int w = 0, sf::Texture* texture = nullptr,
    float vx = 0, float vy = 0, float terminal = 0, int hp = 3, float maxSpeed = 0, bool alive = true, bool movinRight = true);

    virtual void update(float deltaTime, float playerX, float playerY, int &score, int volume);

    void onDeath();
};
