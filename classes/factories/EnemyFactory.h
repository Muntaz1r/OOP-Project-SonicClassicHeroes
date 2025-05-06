#pragma once

#include <iostream>
#include <SFML/Graphics.hpp>
#include <string>

#include "flyers/BatBrain.h"

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