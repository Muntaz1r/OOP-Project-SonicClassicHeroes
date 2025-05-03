#pragma once

#include <iostream>
#include <SFML/System/Clock.hpp>
#include <cmath>

#include "DynamicEntity.h"

class Player : public DynamicEntity
{
protected:
    //static int hp;
    float maxSpeed;
    bool onGround;
    bool invincible;
    bool movingRight;
    float acc_x;
    float acc_y;
    sf::Clock invincibilityClock;
    sf::Time invincibilityDuration = sf::seconds(1);
    const float friction;
    const float gravity;

    Texture idleRightTexture;
    Texture runRightTexture;
    Texture idleLeftTexture;
    Texture runLeftTexture;
    Texture jumpRightTexture;
    Texture jumpLeftTexture;
    
public:
    Player(float px=0, float py=0, int h=0, int w = 0, sf::Texture* texture = nullptr,
        float vx = 0, float vy = 0, float terminal = 0, 
        float ms=0, bool onGround =true, bool invicible = false, bool movingRight = true,
        float acc_x =0, float acc_y=0, float friction = 0, float gravity = 0)
        : DynamicEntity(px, py, h, w, texture, vx, vy, terminal), 
        maxSpeed(ms), onGround(onGround), invincible(invicible), movingRight(movingRight)
        , acc_x(acc_x), acc_y(acc_y), friction(friction), gravity(gravity) {}

    // Getters
    float getMaxSpeed() const { return maxSpeed; }
    bool isOnGround() const { return onGround; }
    bool isInvincible() const { return invincible; }
    //static int getHP() { return hp; }
    bool isMovingRight() const { return movingRight; }
    float getAccnX() const { return acc_x; }
    float getAccY() const { return acc_y; }

    // Setters
    void setMaxSpeed(float speed) { maxSpeed = speed; }
    void setOnGround(bool value) { onGround = value; }
    void setInvincible(bool value) { invincible = value; }
    //static void setHP(int value) { hp = value; }
    void setMovingRight(bool value) { movingRight = value; }
    void setAccX(float value) { acc_x = value; }
    void setAccY(float value) { acc_y = value; }
    
    // Movement
    void jump() {
        if(onGround){
            velocity_y -= acc_y;
        }
        onGround = false;
    }
    void moveLeft() {
        if(onGround){
            velocity_x -= acc_x;
            if (velocity_x < -maxSpeed) velocity_x = -maxSpeed;
            movingRight = false;
        }
    }
    
    void moveRight() {
        if(onGround){
            velocity_x += acc_x;
            if (velocity_x > maxSpeed) velocity_x = maxSpeed;
            movingRight = true;
        }
    }

    //Damage mechanics
    void takeDamage() {
        if (!invincible /*&& hp > 0*/) {
            //--hp;
            invincible = true;
            invincibilityClock.restart();
        }
    }
   
    //Updating everything relevant 
    virtual void update(float deltaTime) override {
        
        // Updating position and velocities
        DynamicEntity::update();
        if(onGround){
            if(velocity_x>0){ // friction
                velocity_x -= friction;
                if (velocity_x<0){
                    velocity_x = 0;
                }
            }else if(velocity_x<0){
                velocity_x += friction;
                if (velocity_x>0){
                    velocity_x = 0;
                }
            }
        }
        
        if(abs(velocity_y) > 0){// going  up or down
            velocity_y += gravity; 
        }
        // Updating invinciillity
        if (invincible && invincibilityClock.getElapsedTime() >= invincibilityDuration) {
            invincible = false;
        }
        if (pos_y >=  635) {
            pos_y = 635; // Clamp to ground
            velocity_y = 0;
            onGround = true;
        }
        
        // Animation logic... maybe changing texture
        if (!onGround) { // Jumping
            if (isMovingRight()) {
                if (sprite.getTexture() != &jumpRightTexture) {
                    sprite.setTexture(jumpRightTexture);
                }
                jumpRightAnimation.update(deltaTime);
            }
            else {
                if (sprite.getTexture() != &jumpLeftTexture) {
                    sprite.setTexture(jumpLeftTexture);
                }
                jumpLeftAnimation.update(deltaTime);
            }
        }
        else if (velocity_x != 0) { // check if stationary
            if (isMovingRight()) {
                // Switch to running texture if not already
                if (sprite.getTexture() != &runRightTexture) {
                    sprite.setTexture(runRightTexture);
                }
                runRightAnimation.update(deltaTime);
            }
            else {
                if (sprite.getTexture() != &runLeftTexture) {
                    sprite.setTexture(runLeftTexture);
                }
                runLeftAnimation.update(deltaTime);
            }
            
        }
        else { // for idle situations
            if (isMovingRight()) {
                // Switch to idle texture if not already once
                if (sprite.getTexture() != &idleRightTexture) {
                    sprite.setTexture(idleRightTexture);
                    sprite.setTextureRect(sf::IntRect(0, 0, 40, 40));
                }
            }
            else {
                if (sprite.getTexture() != &idleLeftTexture) {
                    sprite.setTexture(idleLeftTexture);
                    sprite.setTextureRect(sf::IntRect(0, 0, 40, 40));
                }
            }
            
            // Reset to avoid overlap
            runRightAnimation.reset();
            runLeftAnimation.reset();
            jumpRightAnimation.reset();
            jumpLeftAnimation.reset();
        }
    }
    
    // Will be defined in children classes
    virtual void specialAbility() = 0;

    virtual ~Player(){};
};
// Initialize static member
//int Player::hp = 3;

