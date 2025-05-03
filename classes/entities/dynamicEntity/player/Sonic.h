#pragma once

#include "Player.h"
#include <iostream>

class Sonic : public Player {
protected:
    const float boostedAcc = 10;
    const float boostedMaxSpeed = 10;
    float originalAcc;
    float originalMaxSpeed;
    sf::Clock boostClock;
    bool boosting = false;
public:
    Sonic(float px = 0, float py = 0, int h = 0, int w = 0, sf::Texture* texture = nullptr,
          float vx = 0, float vy = 0, float terminal = 0,
          float ms = 0, bool onGround = true, bool invincible = false, bool moving_right = true,
          float acc_x = 0, float acc_y = 0,float friction = 0, float gravity = 0)
        : Player(px, py, h, w, texture, vx, vy, terminal, ms, onGround, invincible, 
            moving_right, acc_x, acc_y, friction, gravity) 
    {
        originalAcc = acc_x;
        originalMaxSpeed = ms;
    }

    float getBoostedAcc() const { return boostedAcc; }
    float getBoostedMaxSpeed() const { return boostedMaxSpeed; }

    virtual void specialAbility() override {
        if (!boosting) {
            cout << "Sonic boost activated!\n";
            acc_x = boostedAcc;
            maxSpeed = boostedMaxSpeed;
            if(movingRight){
                moveRight();
            }else{
                moveLeft();
            }
            boostClock.restart();
            boosting = true;
        }
    }
    virtual void update() override {
        Player::update();

        // Stop boosting if boost time is done
        if (boosting && boostClock.getElapsedTime().asSeconds() >= 7) {
            std::cout << "Sonic boost expired.\n";
            acc_x = originalAcc;
            maxSpeed = originalMaxSpeed;
            boosting = false;
        }
    }
    virtual ~Sonic() {}
};
