#pragma once

#include <iostream>
#include <SFML/Graphics.hpp>
#include <string>

#include "flyers/BatBrain.h"
#include "flyers/BeeBot.h"
#include "crawlers/MotoBug.h"
#include "crawlers/CrabMeat.h"

using namespace std;

class EnemyFactory {
public:
    virtual Enemy* createEnemy() const = 0;
    virtual ~EnemyFactory(){};
};

class BatBrainFactory {
public:
    BatBrain* createBatBrain() const {
        sf::Texture *texture = new Texture;
        if (!texture->loadFromFile("Data/0right_still.png")) {
            cout << "Failed to load enemy texture\n";
        }
        return new BatBrain(
            800.0f,   // pos_x
            100.0f,   // pos_y
            32,       // height
            32,       // width
            texture, // texture pointer
            1.5f,     // velocity_x
            1.5f,     // velocity_y
            0.0f,     // terminal velocity (not used here)
            3,        // HP
            1.0f,     // max speed (used for following speed)
            true,     // alive
            true      // movingRight
        );
    }
};


class BeeBotFactory {
public:
    BeeBot* createBeeBot() const {
        sf::Texture *texture = new Texture;
        if (!texture->loadFromFile("Data/bee_botR.png")) {
            cout << "Failed to load enemy texture\n";
        }
        return new BeeBot(
            500.0f,   // pos_x
            50.0f,   // pos_y
            64,       // height
            64,       // width
            texture,  // texture pointer
            1.5f,     // velocity_x (used for horizontal movement)
            0.0f,       // velocity_y (not used)
            0.0f,       // terminal velocity (not used here)
            4864.0f,     // maxX (maximum region in x-direction)
            3584.0f,     // minX (minimum region in x-direction)
            5,        // HP
            1.0f,     // max speed (if you use it for movement cap)
            true,     // alive
            true      // movingRight
        );
    }
};


class MotoBugFactory {
public:
    MotoBug* createMotoBug() const {
        sf::Texture *texture = new Texture;
        if (!texture->loadFromFile("Data/moto_bugR.png")) {
            cout << "Failed to load enemy texture\n";
        }
        return new MotoBug(
            7500.0f,   // pos_x
            768.0f,   // pos_y
            32,       // height
            32,       // width
            texture, // texture pointer
            1.5f,     // velocity_x
            0.0f,       // velovity_y (not used)
            0.0f,     // terminal velocity (not used here)
            8511.0f,     // maxX (maximum region in x-direction)
            6600.0f,     // minX (minimum region in x-direction)
            2,        // HP
            1.0f,     // max speed (used for following speed)
            true,     // alive
            true      // movingRight
        );
    }
};


class CrabMeatFactory {
public:
    CrabMeat* createCrabMeat() const {
        sf::Texture *texture = new Texture;
        if (!texture->loadFromFile("Data/bee_botR.png")) {
            cout << "Failed to load enemy texture\n";
        }
        return new CrabMeat(
            10800.0f,   // pos_x
            768.0f,   // pos_y
            32,       // height
            32,       // width
            texture,  // texture pointer
            1.5f,     // velocity_x (used for horizontal movement)
            0.0f,       // velocity_y (not used)
            0.0f,       // terminal velocity (not used here)
            11392.0f,     // maxX (maximum region in x-direction)
            10688.0f,     // minX (minimum region in x-direction)
            4,        // HP
            1.0f,     // max speed (if you use it for movement cap)
            true,     // alive
            true      // movingRight
        );
    }
};

