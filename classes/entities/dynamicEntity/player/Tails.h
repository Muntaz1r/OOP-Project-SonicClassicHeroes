#pragma once

#include "Player.h"
#include <iostream>
using namespace std;

class Tails : public Player {
protected:
    sf::Clock boostClock;
    int specialAbilityTime;
    float prevGravity;
public:
    Tails(float px = 0, float py = 0, int h = 0, int w = 0, sf::Texture* texture = nullptr,
        float vx = 0, float vy = 0, float terminal = 0,
        float ms = 0, bool onGround = true, bool invincible = false, bool moving_right = true,
        float acc_x = 0, float acc_y = 0,float friction = 0, float gravity = 0,
        int specialAbilityTime = 7, bool leader = false);

    sf::Clock getBoostClock();
   
    virtual void specialAbility() override;
    virtual void update(float deltaTime, int &score, int volume) override;
    void collisionHandle(float previousX, float previousY, int &score) override;
    
    void jump()override;
    void flyDown()override;
    void moveRight()override;
    void moveLeft()override;
    void powerUp() override;

    virtual ~Tails();
};