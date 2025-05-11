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
    virtual Player* createPlayer(float acc_x, float acc_y, float friction, float gravity) const = 0;
    virtual ~PlayerFactory();
};

// Sonic Factory
class SonicFactory : public PlayerFactory {
public:
    Player* createPlayer(float acc_x, float acc_y, float friction, float gravity) const override;
};

// Tails Factory
class TailsFactory : public PlayerFactory {
    public:
        Player* createPlayer(float acc_x, float acc_y, float friction, float gravity) const override;
};
    
// Knuckles Factory
class KnucklesFactory : public PlayerFactory {
public:
    Player* createPlayer(float acc_x, float acc_y, float friction, float gravity) const;
};
