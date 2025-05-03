#pragma once

#include <iostream>
#include <SFML/Graphics.hpp>
#include <string>

#include "Sonic.h"
#include "Tails.h"
#include "Knuckles.h"

using namespace std;

class PlayerFactory {
public:
    virtual Player* createPlayer() const = 0;
    virtual ~PlayerFactory(){};
};

class SonicFactory : public PlayerFactory {
public:
    
    Player* createPlayer() const override {
        sf::Texture *texture = new Texture;
        if (!texture->loadFromFile("Data/0right.png")) {
            cout << "Failed to load player texture\n";
        }
        return new Sonic(
            50.0f,   // px
            750.0f,   // py
            64,      // height
            64,      // width
            texture,  // Texture pointer
            0.0f,    // velocity_x
            0.0f,    // velocity_y
            9.8f,    // terminal_velocity
            5.0f,    // maxSpeed
            true,    // onGround
            false,   // invincible
            true,    // movingRight
            0.5f,    // acc_x
            12.0f,     // acc_y
            0.2f,    // friction
            0.4f   //gravity
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
