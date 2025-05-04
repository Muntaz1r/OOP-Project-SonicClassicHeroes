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
        if (!idleRightTexture.loadFromFile("Data/0right_still.png")) {
            cout << "Failed to load 0right_still.png\n";
        }
        if (!runRightTexture.loadFromFile("Data/0right.png")) {
            cout << "Failed to load 0right.png\n";
        }

        if (!idleLeftTexture.loadFromFile("Data/0left_still.png")) {
            cout << "Failed to load 0left_still.png" << endl; 
        }

        if (!runLeftTexture.loadFromFile("Data/0left.png")) {
            cout << "Failed to load 0left.png" << endl;
        }
            
        if (!jumpRightTexture.loadFromFile("Data/0upR.png")) {
            cout << "Failed to load 0upR.png" << endl;
        }

        if (!jumpLeftTexture.loadFromFile("Data/0upL.png")) {
            cout << "Failed to load 0upL.png" << endl;
        }
        
        sprite.setTexture(idleRightTexture); // start with idle right texture
        sprite.setTextureRect(IntRect(0, 0, 40, 40));
        sprite.setScale(2.0f, 2.0f);
        
        // Setup animation for running
        runRightAnimation.initialize(&sprite, &runRightTexture, 40, 40, 12, 0.07f);
        runLeftAnimation.initialize(&sprite, &runLeftTexture, 40, 40, 12, 0.07f);
        
        // Setup animation for jumping
        jumpRightAnimation.initialize(&sprite, &jumpLeftTexture, 40, 40, 8, 0.1f);
        jumpLeftAnimation.initialize(&sprite, &jumpLeftTexture, 40, 40, 8, 0.1f);
    }
    
    virtual void specialAbility() override {
        cout<<"Breaking unbreakable wall..\n";
        boostClock.restart(); // remove on basis of special abillity
        boosting = true;
    }

    virtual void update(float deltaTime) override {
        Player::update(deltaTime);

        // Stop boosting if boost time is done
        if (boosting && boostClock.getElapsedTime().asSeconds() >= 7) {
            cout << "Knuckles boost expired.\n";
            boosting = false;
        }
    }
    virtual ~Knuckles() {}
};