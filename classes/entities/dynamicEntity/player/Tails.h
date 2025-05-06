#pragma once


#include "Player.h"
#include <iostream>
using namespace std;

class Tails : public Player {
protected:
    sf::Clock boostClock;
    bool boosting = false;
    int specialAbillityTime;
    float prevGravity;

public:
    Tails(float px = 0, float py = 0, int h = 0, int w = 0, sf::Texture* texture = nullptr,
        float vx = 0, float vy = 0, float terminal = 0,
        float ms = 0, bool onGround = true, bool invincible = false, bool moving_right = true,
        float acc_x = 0, float acc_y = 0,float friction = 0, float gravity = 0,
        int specialAbillityTime = 7, bool leader = false)
    : Player(px, py, h, w, texture, vx, vy, terminal, ms, onGround, invincible, 
        moving_right, acc_x, acc_y, friction, gravity, leader) , specialAbillityTime(specialAbillityTime)
    {
        if (!idleRightTexture.loadFromFile("Data/tails/0right_still.png")) {
            cout << "Failed to load tails/0right_still.png\n";
        }
        if (!walkRightTexture.loadFromFile("Data/tails/0right.png")) {
            cout << "Failed to load tails/0right.png\n";
        }

        if (!idleLeftTexture.loadFromFile("Data/tails/0left_still.png")) {
            cout << "Failed to load tails/0left_still.png" << endl; 
        }

        if (!walkLeftTexture.loadFromFile("Data/tails/0left.png")) {
            cout << "Failed to load tails/0left.png" << endl;
        }
            
        if (!jumpRightTexture.loadFromFile("Data/tails/0upR.png")) {
            cout << "Failed to load tails/0upR.png" << endl;
        }

        if (!jumpLeftTexture.loadFromFile("Data/tails/0upL.png")) {
            cout << "Failed to load tails/0upL.png" << endl;
        }
        
        sprite.setTexture(idleRightTexture); // start with idle right texture
        sprite.setTextureRect(IntRect(0, 0, 40, 40));
        sprite.setScale(2.0f, 2.0f);
        
        // Setup animation for walking
        walkRightAnimation.initialize(&sprite, &walkRightTexture, 40, 40, 12, 0.07f);
        walkLeftAnimation.initialize(&sprite, &walkLeftTexture, 40, 40, 12, 0.07f);
        
        // Setup animation for jumping
        jumpRightAnimation.initialize(&sprite, &jumpLeftTexture, 40, 40, 6, 0.1f);
        jumpLeftAnimation.initialize(&sprite, &jumpLeftTexture, 40, 40, 6, 0.1f);
    }
    virtual void specialAbility() override {
        if(leader){
            if (!boosting) {
                cout << "Tails boost activated!\n";
                pos_y = pos_y - 128 - 64;
                prevGravity = gravity;
                velocity_y = 0;
                // acc_y= 0;
                gravity = 0;
                
                boostClock.restart();
                boosting = true;
            }
        }
    }
    virtual void update(float deltaTime) override {
        if(boosting){
            onGround = true;
            for (int i=0; i<2; ++i) followers[i]->setOnGround(true); // Align followers
        }
        Player::update(deltaTime);

        if(boosting){
            for (int i=0; i<2; ++i) followers[i]->setPosX(pos_x); // Align followers
            for (int i=0; i<2; ++i) followers[i]->setPosY(pos_y + 64*(i+1)); // Align followers
        }
        
        // Stop boosting if boost time is done
        if (boosting && boostClock.getElapsedTime().asSeconds() >= 10) {
            cout << "Tails boost expired.\n";
            velocity_y = terminal_velocity;
            gravity = prevGravity;
            for (int i=0; i<2; ++i) followers[i]->setVelocityY(followers[i]->getTerminalVelocity());
            boosting = false;
            onGround = false;
        }
    }
    void jump()override{
        if(boosting){
            pos_y -= acc_y/5.0f;
        }else
            Player::jump();
    }
    void flyDown()override{
        if(boosting){
            pos_y += acc_y/5.0f;
        }
    }
    void moveRight()override{
        if(boosting) onGround = true;
        Player::moveRight();
    }
    void moveLeft()override{
        if(boosting) onGround = true;
        Player::moveLeft();
    }

    virtual ~Tails() {}
};