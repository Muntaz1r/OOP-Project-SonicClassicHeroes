#pragma once

#include "Player.h"
#include <iostream>

class Sonic : public Player {
protected:
    const float boostedAcc = 10;
    const float boostedMaxSpeed = 10;
    float originalAcc;
    float originalMaxSpeed;
    

public:
    Sonic(float px = 0, float py = 0, int h = 0, int w = 0, sf::Texture* texture = nullptr,
          float vx = 0, float vy = 0, float terminal = 0,
          float ms = 0, bool onGround = true, bool invincible = false, bool moving_right = true,
          float acc_x = 0, float acc_y = 0,float friction = 0, float gravity = 0,
          int boostTime = 7, bool leader = false);

    float getBoostedAcc() const;
    float getBoostedMaxSpeed() const;

    virtual void specialAbility() override;
    
    virtual void update(float deltaTime, int &score, int volume) override;
    virtual void powerUp() override;
    virtual ~Sonic();
};
