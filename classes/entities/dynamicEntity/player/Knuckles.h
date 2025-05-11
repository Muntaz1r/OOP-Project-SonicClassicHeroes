#pragma once

#include "Player.h"
#include <iostream>
using namespace std;

class Knuckles : public Player {
protected:
    sf::Clock powerUpClock;
    int powerUpTime;
public:
    Knuckles(float px = 0, float py = 0, int h = 0, int w = 0, sf::Texture* texture = nullptr,
        float vx = 0, float vy = 0, float terminal = 0,
        float ms = 0, bool onGround = true, bool invincible = false, bool moving_right = true,
        float acc_x = 0, float acc_y = 0,float friction = 0, float gravity = 0,
        int powerUpTime = 15 ,bool leader = false);
    
    virtual void specialAbility() override;

    virtual void update(float deltaTime, int &score, int volume);

    void powerUp() override;
    virtual ~Knuckles();
};