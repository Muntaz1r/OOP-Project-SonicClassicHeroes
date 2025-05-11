#include "PlayerFactory.h"

Player* SonicFactory::createPlayer(float acc_x, float acc_y, float friction, float gravity) const {
    sf::Texture *texture = new Texture;
    if (!texture->loadFromFile("Data/0right.png")) {
        cout << "Failed to load player texture\n";
    }
    return new Sonic(
        50.0f,   // px
        200.0f,   // py
        80,      // height
        80,      // width
        texture,  // Texture pointer
        0.0f,    // velocity_x
        0.0f,    // velocity_y
        4.0f,    // terminal_velocity
        4.5f,    // maxSpeed
        true,    // onGround
        false,   // invincible
        true,    // movingRight
        acc_x,    // acc_x
        acc_y,     // acc_y
        friction,    // friction
        gravity,   //gravity
        7,  //special abillity duration
        false // leader
    );
}

Player* TailsFactory::createPlayer(float acc_x, float acc_y, float friction, float gravity) const {
    sf::Texture *texture = new Texture;
    if (!texture->loadFromFile("Data/0right.png")) {
        cout << "Failed to load player texture\n";
    }
    return new Tails(
        10.0f,   // px
        200.0f,   // py
        80,      // height
        80,      // width
        texture,  // Texture pointer
        0.0f,    // velocity_x
        0.0f,    // velocity_y
        4.0f,    // terminal_velocity
        4.5f,    // maxSpeed
        true,    // onGround
        false,   // invincible
        true,    // movingRight
        acc_x,    // acc_x
        acc_y,     // acc_y
        friction,    // friction
        gravity,   //gravity
        7, // specail abillity time
        false //leader
    );
}

Player* KnucklesFactory::createPlayer(float acc_x, float acc_y, float friction, float gravity) const  {
    sf::Texture *texture = new Texture;
    if (!texture->loadFromFile("Data/0right.png")) {
        cout << "Failed to load player texture\n";
    }
    return new Knuckles(
        20.0f,   // px
        200.0f,   // py
        80,      // height
        80,      // width
        texture,  // Texture pointer
        0.0f,    // velocity_x
        0.0f,    // velocity_y
        4.0f,    // terminal_velocity
        4.5f,    // maxSpeed
        true,    // onGround
        false,   // invincible
        true,    // movingRight
        acc_x,    // acc_x
        acc_y,     // acc_y
        friction,    // friction
        gravity,   //gravity
        7, // power up abillity time
        false //leader
    );
}

PlayerFactory::~PlayerFactory(){}