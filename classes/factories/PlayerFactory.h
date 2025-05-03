#pragma once

#include <iostream>
#include <string>
#include "Sonic.h"
#include "Tails.h"
#include "Knuckles.h"
#include <iostream>

using namespace std;

class PlayerFactory {
public:
    virtual Player* createPlayer() const = 0;
    virtual ~PlayerFactory(){};
};

class SonicFactory : public PlayerFactory {
public:
    Player* createPlayer() const override {
        return new Sonic(
            50.0f,   // px
            700.0f,   // py
            64,      // height
            64,      // width
            0.0f,    // velocity_x
            0.0f,    // velocity_y
            9.8f,    // terminal_velocity
            5.0f,    // maxSpeed
            true,    // onGround
            false,   // invincible
            true,    // movingRight
            0.5f,    // acc_x
            7.5f,     // acc_y
            0.1f,    // friction
            0.1f    //gravity
        );
    }
};
/*class TailsFactory : public PlayerFactory {
public:
    Player* createPlayer() const override {
        return new Tails();
    }
};

// Knuckles Factory
class KnucklesFactory : public PlayerFactory {
public:
    Player* createPlayer() const override {
        return new Knuckles();
    }
};
*/
