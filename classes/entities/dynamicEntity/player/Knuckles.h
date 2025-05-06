#pragma once

#include "Player.h"
#include <iostream>
using namespace std;

class Knuckles : public Player {
protected:
    sf::Clock boostClock;
    bool boosting = false;
    int specialAbillityTime;
public:
    Knuckles(float px = 0, float py = 0, int h = 0, int w = 0, sf::Texture* texture = nullptr,
        float vx = 0, float vy = 0, float terminal = 0,
        float ms = 0, bool onGround = true, bool invincible = false, bool moving_right = true,
        float acc_x = 0, float acc_y = 0,float friction = 0, float gravity = 0,
        int specialAbillityTime = 7/*think this may be removed*/
        ,bool leader = false)
    : Player(px, py, h, w, texture, vx, vy, terminal, ms, onGround, invincible, 
        moving_right, acc_x, acc_y, friction, gravity, leader), specialAbillityTime(specialAbillityTime)
    {
        if (!idleRightTexture.loadFromFile("Data/knuckles/0right_still.png")) {
            cout << "Failed to load knuckles/0right_still.png\n";
        }
        if (!walkRightTexture.loadFromFile("Data/knuckles/0right.png")) {
            cout << "Failed to load knuckles/0right.png\n";
        }

        if (!idleLeftTexture.loadFromFile("Data/knuckles/0left_still.png")) {
            cout << "Failed to load knuckles/0left_still.png" << endl; 
        }

        if (!walkLeftTexture.loadFromFile("Data/knuckles/0left.png")) {
            cout << "Failed to load knuckles/0left.png" << endl;
        }
            
        if (!jumpRightTexture.loadFromFile("Data/knuckles/0upR.png")) {
            cout << "Failed to load knuckles/0upR.png" << endl;
        }

        if (!jumpLeftTexture.loadFromFile("Data/knuckles/0upL.png")) {
            cout << "Failed to load knuckles/0upL.png" << endl;
        }
        
        sprite.setTexture(idleRightTexture); // start with idle right texture
        sprite.setTextureRect(IntRect(0, 0, 40, 40));
        sprite.setScale(2.0f, 2.0f);
        
        // Setup animation for walking
        walkRightAnimation.initialize(&sprite, &walkRightTexture, 40, 40, 12, 0.07f);
        walkLeftAnimation.initialize(&sprite, &walkLeftTexture, 40, 40, 12, 0.07f);
        
        // Setup animation for jumping
        jumpRightAnimation.initialize(&sprite, &jumpLeftTexture, 40, 40, 8, 0.1f);
        jumpLeftAnimation.initialize(&sprite, &jumpLeftTexture, 40, 40, 8, 0.1f);
    }
    
    virtual void specialAbility() override {
        cout<<"Break\n";
        if(collidesRight('b')) {
            // Collect right coins if they are 'R'
            if (*collidingTiles.rightTop == 'b') {
                *collidingTiles.rightTop = 's';  // Set to 's' if it's a coin
            }
            if (*collidingTiles.rightBottom == 'b') {
                *collidingTiles.rightBottom = 's';  // Set to 's' if it's a coin
            }
        }
        
        if (collidesLeft('R')) {
            // Collect left coins if they are 'R'
            if (*collidingTiles.leftTop == 'b') {
                *collidingTiles.leftTop = 's';  // Set to 's' if it's a coin
            }
            if (*collidingTiles.leftBottom == 'b') {
                *collidingTiles.leftBottom = 's';  // Set to 's' if it's a coin
            }
        }
    }

    // virtual void update(float deltaTime) override {
    //     Player::update(deltaTime);
    // }
    virtual ~Knuckles() {}
};