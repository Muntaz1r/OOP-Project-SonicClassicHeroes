#pragma once


#include "Player.h"
#include <iostream>
using namespace std;

class Tails : public Player {
protected:
    sf::Clock boostClock;
    int specialAbilityTime;
    float prevGravity;

public:
    Tails(float px = 0, float py = 0, int h = 0, int w = 0, sf::Texture* texture = nullptr,
        float vx = 0, float vy = 0, float terminal = 0,
        float ms = 0, bool onGround = true, bool invincible = false, bool moving_right = true,
        float acc_x = 0, float acc_y = 0,float friction = 0, float gravity = 0,
        int specialAbilityTime = 7, bool leader = false)
    : Player(px, py, h, w, texture, vx, vy, terminal, ms, onGround, invincible, 
        moving_right, acc_x, acc_y, friction, gravity, leader) , specialAbilityTime(specialAbilityTime)
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

        if (!boostRightTexture.loadFromFile("Data/tails/boostR.png")) {
            cout << "Failed to load tails/boostR.png" << endl;
        }

        if (!boostLeftTexture.loadFromFile("Data/tails/boostL.png")) {
            cout << "Failed to load tails/boostL.png" << endl;
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

        // Setup animation for boosting
        boostRightAnimation.initialize(&sprite, &boostRightTexture, 40, 40, 8, 0.07f);
        boostLeftAnimation.initialize(&sprite, &boostLeftTexture, 40, 40, 8, 0.07f);
    }
    sf::Clock getBoostClock(){return boostClock;}
    virtual void specialAbility() override {
        if(leader){
            if (!boosting) {
                cout << "Tails boost activated!\n";
                if(numPowerUps > 0){
                    this->powerUp();
                }
                pos_y = pos_y - 128;
                prevGravity = gravity;
                velocity_y = 0;
                // acc_y= 0;
                gravity = 0;
                
                boostClock.restart();
                boosting = true;
            }
        }
    }
    virtual void update(float deltaTime, int &score, int volume) override {
        if(boosting){
            onGround = true;
            for (int i=0; i<2; ++i) followers[i]->setOnGround(true); // Align followers
        }
        Player::update(deltaTime, score, volume);

        if(boosting){
            for (int i=0; i<2; ++i) followers[i]->setPosX(pos_x); // Align followers y
            
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
            if(specialAbilityTime > 7){//If power up just used up
                specialAbilityTime = 7;
            }
        }
    }

    void collisionHandle(float previousX, float previousY, int &score) override {
        Player::collisionHandle(previousX, previousY, score);

        if(boosting){

            if (pos_y <= previousY && (collidesAbove('\0') || collidesAbove('w') || 
                collidesAbove('x') || collidesAbove('b') || pos_y < 0 )){
                pos_y = collidingTiles.yAbove + 64 + 5;//tile size
                velocity_y = 0;
            }
            if (!fallingIntoVoid && ((followers[1]->collidesBelow('\0') || followers[1]->collidesBelow('w') 
            || followers[1]->collidesBelow('q') || followers[1]->collidesBelow('b') || followers[1]->collidesBelow('x')))) {
                pos_y = followers[1]->collidingTiles.yBelow - height - 128 - 7.25;
                velocity_y = 0;
            }
            // X collision
            if (((followers[1]->collidesRight('\0') || followers[1]->collidesRight('w') 
                || followers[1]->collidesRight('x') || followers[1]->collidesRight('b')) && movingRight)) {
                pos_x = followers[1]->collidingTiles.xRight - 80;
                velocity_x = -0;
                movingRight = false;
            }else if ((followers[1]->collidesLeft('\0') || followers[1]->collidesLeft('w') || 
            followers[1]->collidesLeft('x') || followers[1]->collidesLeft('b')) && !movingRight) {
                pos_x = followers[1]->collidingTiles.xLeft + 60;  // Prevent movement if collided
                velocity_x = -0;
                movingRight = true;
            }
            if (((followers[0]->collidesRight('\0') || followers[0]->collidesRight('w') 
            || followers[0]->collidesRight('x') || followers[0]->collidesRight('b')) && movingRight)) {
                pos_x = followers[0]->collidingTiles.xRight - 80;
                velocity_x = -0;
                movingRight = false;
            }else if ((followers[0]->collidesLeft('\0') || followers[0]->collidesLeft('w') || 
            followers[0]->collidesLeft('x') || followers[0]->collidesLeft('b')) && !movingRight) {
                pos_x = followers[0]->collidingTiles.xLeft + 60;  // Prevent movement if collided
                velocity_x = -0;
                movingRight = true;
            }
            
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
    void powerUp() override{
        cout<<"tails power up\n";
        specialAbilityTime +=4;
        numPowerUps--;
    }

    virtual ~Tails() {}
};