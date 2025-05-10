#pragma once

#include "Player.h"
#include <iostream>
using namespace std;

class Knuckles : public Player {
protected:
    sf::Clock powerUpClock;
    int powerUpTime;
public:
    Knuckles(float px = 0, float py = 0, int h = 0, int w = 0, sf::Texture* texture = nullptr,
        float vx = 0, float vy = 0, float terminal = 0,
        float ms = 0, bool onGround = true, bool invincible = false, bool moving_right = true,
        float acc_x = 0, float acc_y = 0,float friction = 0, float gravity = 0,
        int powerUpTime = 15
        ,bool leader = false)
    : Player(px, py, h, w, texture, vx, vy, terminal, ms, onGround, invincible, 
        moving_right, acc_x, acc_y, friction, gravity, leader), powerUpTime(powerUpTime)
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

        if (!boostRightTexture.loadFromFile("Data/knuckles/boostR.png")) {
            cout << "Failed to load knuckles/boostR.png" << endl;
        }

        if (!boostLeftTexture.loadFromFile("Data/knuckles/boostL.png")) {
            cout << "Failed to load knuckles/boostL.png" << endl;
        }

        if (!hangingRightTexture.loadFromFile("Data/knuckles/hangR.png")) {
            cout << "Failed to load knuckles/hangR.png" << endl;
        }

        if (!hangingLeftTexture.loadFromFile("Data/knuckles/hangL.png")) {
            cout << "Failed to load knuckles/hangL.png" << endl;
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

        // Setup animation for boosting
        boostRightAnimation.initialize(&sprite, &boostRightTexture, 40, 40, 8, 0.1f);
        boostLeftAnimation.initialize(&sprite, &boostLeftTexture, 40, 40, 8, 0.1f);

        // Setup animation for hanging
        hangingRightAnimation.initialize(&sprite, &hangingRightTexture, 40, 40, 8, 0.1f);
        hangingLeftAnimation.initialize(&sprite, &hangingLeftTexture, 40, 40, 8, 0.1f);
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
        
        if (collidesLeft('b')) {
            // Collect left coins if they are 'R'
            if (*collidingTiles.leftTop == 'b') {
                *collidingTiles.leftTop = 's';  // Set to 's' if it's a coin
            }
            if (*collidingTiles.leftBottom == 'b') {
                *collidingTiles.leftBottom = 's';  // Set to 's' if it's a coin
            }
        }

        if (collidesBelow('b')) {
            // Collect left coins if they are 'R'
            if (*collidingTiles.belowLeft == 'b') {
                *collidingTiles.belowLeft = 's';  // Set to 's' if it's a coin
            }
            if (*collidingTiles.belowRight == 'b') {
                *collidingTiles.belowRight = 's';  // Set to 's' if it's a coin
            }
        }
        if (collidesBelow('b')) {
            // Collect left coins if they are 'R'
            if (*collidingTiles.belowLeft == 'b') {
                *collidingTiles.belowLeft = 's';  // Set to 's' if it's a coin
            }
            if (*collidingTiles.belowRight == 'b') {
                *collidingTiles.belowRight = 's';  // Set to 's' if it's a coin
            }
        }
        if(numPowerUps > 0){
            this->powerUp();
        }
    }

    virtual void update(float deltaTime, int &score, int volume) override {
        Player::update(deltaTime, score, volume);
        if (boosting && powerUpClock.getElapsedTime().asSeconds() >= powerUpTime) {
            cout << "Knuckles power up expired.\n";
            boosting = false;
            invincible = false;
        }
    }

    void powerUp() override {
        if (!boosting) {
            cout<<"knuckles power up\n";
            boosting = true;
            powerUpClock.restart();
            invincible = true;
        }
    }
    virtual ~Knuckles() {}
};