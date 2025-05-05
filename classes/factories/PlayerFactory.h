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
            850.0f,   // py
            64,      // height
            64,      // width
            texture,  // Texture pointer
            0.0f,    // velocity_x
            0.0f,    // velocity_y
            4.5f,    // terminal_velocity
            5.0f,    // maxSpeed
            true,    // onGround
            false,   // invincible
            true,    // movingRight
            0.5f,    // acc_x
            12.0f,     // acc_y
            0.2f,    // friction
            0.4f,   //gravity
            7,  //special abillity duration
            false // leader
        );
    }
};
class TailsFactory : public PlayerFactory {
    public:
        Player* createPlayer() const override {
            sf::Texture *texture = new Texture;
            if (!texture->loadFromFile("Data/0right.png")) {
                cout << "Failed to load player texture\n";
            }
            return new Tails(
                10.0f,   // px
                800.0f,   // py
                64,      // height
                64,      // width
                texture,  // Texture pointer
                0.0f,    // velocity_x
                0.0f,    // velocity_y
                4.5f,    // terminal_velocity
                5.0f,    // maxSpeed
                true,    // onGround
                false,   // invincible
                true,    // movingRight
                0.47f,    // acc_x
                12.0f,     // acc_y
                0.2f,    // friction
                0.4f,   //gravity
                7, // specail abillity time
                false //leader
            );
        }
};
    
// Knuckles Factory
class KnucklesFactory : public PlayerFactory {
public:
    Player* createPlayer() const override {
        sf::Texture *texture = new Texture;
        if (!texture->loadFromFile("Data/0right.png")) {
            cout << "Failed to load player texture\n";
        }
        return new Knuckles(
            20.0f,   // px
            600.0f,   // py
            64,      // height
            64,      // width
            texture,  // Texture pointer
            0.0f,    // velocity_x
            0.0f,    // velocity_y
            4.5f,    // terminal_velocity
            5.0f,    // maxSpeed
            true,    // onGround
            false,   // invincible
            true,    // movingRight
            0.42f,    // acc_x
            12.0f,     // acc_y
            0.2f,    // friction
            0.4f,   //gravity
            7, // specail abillity time
            false //leader
        );
    }
};
