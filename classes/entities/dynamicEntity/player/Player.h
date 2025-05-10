#pragma once

#include <iostream>
#include <SFML/System/Clock.hpp>
#include <cmath>

#include "DynamicEntity.h"
#include "SoundManager.h"

struct CollidingTiles {
    char *aboveLeft;
    char *aboveRight;
    char *belowLeft;
    char *belowRight;
    char *leftTop;
    char *leftBottom;
    char *rightTop;
    char *rightBottom;
    float yBelow;
    float yAbove;
    float xRight;
    float xLeft;  
};


class Player : public DynamicEntity
{
protected:
    static int hp;
    float maxSpeed;
    bool onGround;
    bool invincible;
    bool movingRight;
    float acc_x;
    float acc_y;
    sf::Clock invincibilityClock;
    sf::Time invincibilityDuration = sf::seconds(1);
    float friction;
    float gravity;
    bool leader;
    bool boosting;
    Player* followers[2];
    char invalid;
    bool fallingIntoVoid;
    bool hanging;
    bool hangingRight;

    Texture idleRightTexture;
    Texture idleLeftTexture;
    Texture walkRightTexture;
    Texture walkLeftTexture;
    Texture jumpRightTexture;
    Texture jumpLeftTexture;
    Texture boostRightTexture;
    Texture boostLeftTexture;
    Texture hangingRightTexture;
    Texture hangingLeftTexture;

    Animation boostRightAnimation;
    Animation boostLeftAnimation;
    Animation hangingRightAnimation;
    Animation hangingLeftAnimation;

    SoundManager* playerSounds;
    
public:
    CollidingTiles collidingTiles;
    Player(float px=0, float py=0, int h=0, int w = 0, sf::Texture* texture = nullptr,
        float vx = 0, float vy = 0, float terminal = 0, 
        float ms=0, bool onGround =true, bool invicible = false, bool movingRight = true,
        float acc_x =0, float acc_y=0, float friction = 0, float gravity = 0, 
        bool leader = false)
        : DynamicEntity(px, py, h, w, texture, vx, vy, terminal), 
        maxSpeed(ms), onGround(onGround), invincible(invicible), movingRight(movingRight)
        , acc_x(acc_x), acc_y(acc_y), friction(friction), gravity(gravity), leader(leader) {
            boosting = false;
            invalid = '\0';
            fallingIntoVoid = false;
            hanging = false;

            playerSounds = new SoundManager();
        
            // Index 0: Ring
            // Index 1: Spike
            playerSounds->loadSound(0, "Data/sfx/ring.wav");
            playerSounds->loadSound(1, "Data/sfx/spike.wav");
        }

    // Getters
    float getMaxSpeed() const { return maxSpeed; }
    bool isOnGround() const { return onGround; }
    bool isInvincible() const { return invincible; }
    static int getHP() { return hp; }
    bool isMovingRight() const { return movingRight; }
    float getAccnX() const { return acc_x; }
    float getAccY() const { return acc_y; }
    bool getLeader() const {return leader;}
    Player* getFollower1() const{return followers[0];}
    Player* getFollower2() const{return followers[1];}
    bool getBoosting() const{return boosting;}
    bool getFallingIntoVoid() const{return fallingIntoVoid;}
    bool getHanging() const { return hanging; }
    bool getHangingRight() const { return hangingRight; }
    virtual sf::Clock getBoostClock() {return sf::Clock();}

    // Setters
    void setMaxSpeed(float speed) { maxSpeed = speed; }
    void setOnGround(bool value) { onGround = value; }
    void setInvincible(bool value) { invincible = value; }
    static void setHP(int value) { hp = value; }
    void setMovingRight(bool value) { movingRight = value; }
    void setAccX(float value) { acc_x = value; }
    void setAccY(float value) { acc_y = value; }
    void setLeader(bool value) {leader = value;}
    void setFollowers(Player* follower1, Player* follower2){
        followers[0]=follower1;
        followers[1]=follower2;
    }
    void setGravity(float value){ gravity = value;}
    void setFriction(float value){ friction= value;}
    void setBoosting(bool value){ boosting = value;}
    void setFallingIntoVoid(bool value){fallingIntoVoid = value;}
    void setHanging(bool value) { hanging = value; }
    void setHangingRight(bool value) { hangingRight = value; }

    
    
    
    
    char* sampleTile(float x, float y, int tileSize, int levelHeight, int levelWidth, char** grid, int* outRow = nullptr, int* outCol = nullptr) {
    int col = static_cast<int>(x / tileSize);
    int row = static_cast<int>(y / tileSize);

    if (col < 0 || col >= levelWidth || row < 0 || row >= levelHeight) {
        *outRow = -1;
        *outCol = -1;
        return &invalid; // assuming 'invalid' is defined somewhere
    }

    if (outRow) *outRow = row;
    if (outCol) *outCol = col;

    return &grid[row][col];
}

    

    
    void setCollidingTiles(int tileSize, int levelHeight, int levelWidth, char** grid) {
        
        float left   = pos_x;
        float right  = pos_x + width;
        float top    = pos_y;
        float bottom = pos_y + height;
    
        // Buffer helps avoid jitter or premature collision detection
        float bufferY = tileSize / 10.0f;
        float bufferX = tileSize / 10.0f;
    
        int row, col;
    
        // BELOW
        collidingTiles.belowLeft  = sampleTile(left + bufferX, bottom + bufferY, tileSize, levelHeight, levelWidth, grid, &row, &col);
        collidingTiles.yBelow = row * tileSize;
        collidingTiles.belowRight = sampleTile(right - bufferX, bottom + bufferY, tileSize, levelHeight, levelWidth, grid);
    
        // ABOVE
        collidingTiles.aboveLeft  = sampleTile(left + bufferX, top - bufferY, tileSize, levelHeight, levelWidth, grid, &row, &col);
        collidingTiles.yAbove = row * tileSize;
        collidingTiles.aboveRight = sampleTile(right - bufferX, top - bufferY, tileSize, levelHeight, levelWidth, grid);
    
        // LEFT
        collidingTiles.leftTop    = sampleTile(left - bufferX, top + bufferY, tileSize, levelHeight, levelWidth, grid, &row, &col);
        collidingTiles.xLeft = col * tileSize;
        collidingTiles.leftBottom = sampleTile(left - bufferX, bottom - bufferY, tileSize, levelHeight, levelWidth, grid);
    
        // RIGHT
        collidingTiles.rightTop   = sampleTile(right + bufferX, top + bufferY, tileSize, levelHeight, levelWidth, grid, &row, &col);
        collidingTiles.xRight = col * tileSize;
        collidingTiles.rightBottom = sampleTile(right + bufferX, bottom - bufferY, tileSize, levelHeight, levelWidth, grid);
        // If this object has followers, update their collision tiles too
        if (leader) {
            for (int i = 0; i < 2; ++i)
                followers[i]->setCollidingTiles(tileSize, levelHeight, levelWidth, grid);
        }
        
    }
    
    // Movement
    virtual void jump() {
        if(onGround){
            velocity_y -= acc_y;
        }
        onGround = false;
        if(leader){
            for(int i=0; i<2; ++i) followers[i]->jump();
        }
    }

    virtual void moveLeft() {
        if(onGround){
            velocity_x -= acc_x;
            if (velocity_x < -maxSpeed) velocity_x = -maxSpeed;
            movingRight = false;
        }
        if(leader){
            for(int i=0; i<2; ++i){
                if(!(pos_x - followers[i]->getPosX() > 100)){
                    followers[i]->moveLeft();
                }
            }
        }
    }
    
    virtual void moveRight() {
        if(onGround){
            velocity_x += acc_x;
            if (velocity_x > maxSpeed) velocity_x = maxSpeed;
            movingRight = true;
        }
        if(leader){
            for(int i=0; i<2; ++i){
                if(!(pos_x - followers[i]->getPosX() < -100)){
                    followers[i]->moveRight();
                }
            }
        }
    }

    virtual void flyDown(){} // Tails specific ftn

    //Damage mechanics
    void takeDamage() {
        if (!invincible && hp > 0) {
            --hp;
            invincible = true;
            invincibilityClock.restart();
        }
    }
   
    //Updating everything relevant 
    virtual void update(float deltaTime, int &score, int volume) override{
        playerSounds->setVolume(volume);
        float previousX = pos_x;
        float previousY = pos_y;
        // Updating position and velocities
        DynamicEntity::update(deltaTime, score, volume);
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
        // if (pos_y >=  635) {
        //     pos_y = 635; // Clamp to ground
        //     velocity_y = 0;
        //     onGround = true;
        // }
        
        
        // Animation logic
        if (hanging) {
            if (getHangingRight()) {
                if (sprite.getTexture() != &hangingRightTexture) {
                    sprite.setTexture(hangingRightTexture);
                }
                hangingRightAnimation.update(deltaTime);
            }
            else {
                if (sprite.getTexture() != &hangingLeftTexture) {
                    sprite.setTexture(hangingLeftTexture);
                }
                hangingLeftAnimation.update(deltaTime);
            }
        }
        else if (!onGround) { // Jumping
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
        else if (boosting && (velocity_x != 0 || velocity_y != 0)) {
            if (isMovingRight()) {
                // Switch to running texture if not already
                if (sprite.getTexture() != &boostRightTexture) {
                    sprite.setTexture(boostRightTexture);
                }
                boostRightAnimation.update(deltaTime);
            }
            else {
                if (sprite.getTexture() != &boostLeftTexture) {
                    sprite.setTexture(boostLeftTexture);
                }
                boostLeftAnimation.update(deltaTime);
            }
        }
        else if (velocity_x != 0) { // check if not stationary
            if (isMovingRight()) {
                // Switch to running texture if not already
                if (sprite.getTexture() != &walkRightTexture) {
                    sprite.setTexture(walkRightTexture);
                }
                walkRightAnimation.update(deltaTime);
            }
            else {
                if (sprite.getTexture() != &walkLeftTexture) {
                    sprite.setTexture(walkLeftTexture);
                }
                walkLeftAnimation.update(deltaTime);
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
            walkRightAnimation.reset();
            walkLeftAnimation.reset();
            jumpRightAnimation.reset();
            jumpLeftAnimation.reset();
        }
        
        collisionHandle(previousX, previousY, score);
        
        if(leader){
            if(fallingIntoVoid){
                fallingIntoVoid = false;
                velocity_x = 0;
                pos_x = 20;
                this->takeDamage();
                pos_y = 50;
                velocity_y = terminal_velocity;
            }
            for(int i=0; i<2; ++i) {
                followers[i]->update(deltaTime, score, volume);
                if(abs(pos_x - followers[i]->getPosX()) > 960){
                    followers[i]->setPosY(0);
                    followers[i]->setVelocityY(followers[i]->getTerminalVelocity());
                    followers[i]->setPosX(pos_x + 20*i);
                }else if(pos_x - followers[i]->getPosX() > 100){
                    followers[i]->setVelocityX(followers[i]->getMaxSpeed());
                }else if(pos_x - followers[i]->getPosX() < -100){
                    followers[i]->setVelocityX(-followers[i]->getMaxSpeed());
                }
            }
        }else if(fallingIntoVoid){
           fallingIntoVoid = false;
           pos_x = 1000000; // initiates teleport to leader
        }

    }

    //Tile Checkers
    bool collidesBelow(char target) {
        return *collidingTiles.belowLeft  == target ||
               *collidingTiles.belowRight == target;
    }
    
    bool collidesAbove(char target) {
        return *collidingTiles.aboveLeft  == target ||
               *collidingTiles.aboveRight == target;
    }
    
    bool collidesLeft(char target) {
        return *collidingTiles.leftTop    == target ||
               *collidingTiles.leftBottom == target;
    }
    
    bool collidesRight(char target) {
        return *collidingTiles.rightTop    == target ||
               *collidingTiles.rightBottom == target;
    }

    //Tile setters
    void setBelow(char target) {
        *collidingTiles.belowLeft  = target;
        *collidingTiles.belowRight = target;
    }
    
    void setAbove(char target) {
        *collidingTiles.aboveLeft  = target;
        *collidingTiles.aboveRight = target;
    }
    
    void setLeft(char target) {
        *collidingTiles.leftTop    = target;
        *collidingTiles.leftBottom = target;
    }
    
    void setRight(char target) {
        *collidingTiles.rightTop    = target;
        *collidingTiles.rightBottom = target;
    }
    
    

    virtual void collisionHandle(float previousX, float previousY, int &score){

        if(collidesBelow('p') && !(collidesBelow('\0') || collidesBelow('w') 
        || collidesBelow('q') || collidesBelow('b') || collidesBelow('x'))){
            if(leader){
                cout<<"Player fell into the void\n";
                velocity_y = terminal_velocity;
                fallingIntoVoid = true;
                for (int i =0; i<2; ++i){
                    followers[i]->setVelocityY(terminal_velocity);
                    followers[i]->setFallingIntoVoid(true);
                }
            }
        }
        //Below ther is : out-of-bounds, wall or platform
        if (!fallingIntoVoid && (pos_y >= previousY && (collidesBelow('\0') || collidesBelow('w') 
        || collidesBelow('q') || collidesBelow('b') || collidesBelow('x')))) {
            // if (pos_y - previousY > 64/5.0f) // when hit ground really hard
            //     pos_y = previousY - velocity_y;
            // else   
            //     pos_y = previousY;
            // onGround = true;
            // velocity_y = 0;
            pos_y = collidingTiles.yBelow - height - 2;
            velocity_y = 0;
            onGround = true;
        }

        if ((pos_x> previousX && (collidesRight('\0') || collidesRight('w') 
        || collidesRight('x') || collidesRight('b')) && movingRight)) {
            pos_x = collidingTiles.xRight - width;  // Prevent movement if collided
            //velocity_x = velocity_x/1.5f;
        }

        // Left collision check (only when moving left)
        if (pos_x < previousX && (collidesLeft('\0') || collidesLeft('w') || 
        collidesLeft('x') || collidesLeft('b')) && !movingRight) {
            pos_x = collidingTiles.xLeft + 64;  // Prevent movement if collided
            velocity_x = velocity_x/1.5f;
        }
        if (pos_x < previousX && pos_x < 0){
            pos_x = previousX;
            velocity_x = velocity_x/1.5f;
        }
        
        if (pos_y <= previousY && (collidesAbove('\0') || collidesAbove('w') || 
        collidesAbove('x') || collidesAbove('b') || pos_y < 0 )){
            pos_y = collidingTiles.yAbove + 64;//tile size
            velocity_y = -velocity_y*0.5;
        }
        
        //Handling falling off
        if (!(collidesBelow('\0')||collidesBelow('q') || collidesBelow('w')
        || collidesBelow('b') || collidesBelow('x'))) {
            if (onGround) {
                velocity_y += gravity;
                onGround = false;
            }
        }
        
        // For some reason leader has above definded while followers don't idk y
        if((leader ? collidesAbove('p') : collidesBelow('p') )&& !fallingIntoVoid){ 
            fallingIntoVoid = true;
        }

        
        //Ring collection
        if (collidesBelow('R')) {
            playerSounds->play(0);
            // Check both tiles (belowLeft and belowRight) and set to 's' only if they are 'R'
            if (*collidingTiles.belowLeft == 'R') {
                *collidingTiles.belowLeft = 's';  // Set only belowLeft to 's' if it's 'R'
                score += 100;
            }
            if (*collidingTiles.belowRight == 'R') {
                score += 100;
                *collidingTiles.belowRight = 's';  // Set only belowRight to 's' if it's 'R'
            }
        }
        
        if (collidesAbove('R')) {
            playerSounds->play(0);
            // Check both tiles (aboveLeft and aboveRight) and set to 's' only if they are 'R'
            if (*collidingTiles.aboveLeft == 'R') {
                score += 100;
                *collidingTiles.aboveLeft = 's';  // Set only aboveLeft to 's' if it's 'R'
            }
            if (*collidingTiles.aboveRight == 'R') {
                score += 100;
                *collidingTiles.aboveRight = 's';  // Set only aboveRight to 's' if it's 'R'
            }
        }

        if (collidesRight('R')) {
            playerSounds->play(0);
            // Collect right coins if they are 'R'
            if (*collidingTiles.rightTop == 'R') {
                score += 100;
                *collidingTiles.rightTop = 's';  // Set to 's' if it's a coin
            }
            if (*collidingTiles.rightBottom == 'R') {
                score += 100;
                *collidingTiles.rightBottom = 's';  // Set to 's' if it's a coin
            }
        }
        
        if (collidesLeft('R')) {
            playerSounds->play(0);
            // Collect left coins if they are 'R'
            if (*collidingTiles.leftTop == 'R') {
                score += 100;
                *collidingTiles.leftTop = 's';  // Set to 's' if it's a coin
            }
            if (*collidingTiles.leftBottom == 'R') {
                score += 100;
                *collidingTiles.leftBottom = 's';  // Set to 's' if it's a coin
            }
        }
        
    
        
        
        
        if (collidesBelow('C') || collidesAbove('C') || 
            collidesLeft('C') || collidesRight('C')) {
            cout << "Collect crystal\n";
        }
        
        if (pos_y >= previousY && collidesBelow('x')) {
            playerSounds->play(1);
            cout << "Spike hurt" << endl;
                takeDamage();
        }
    }

    virtual void render(sf::RenderWindow& window, float cameraOffsetX) override {
        float time = invincibilityClock.getElapsedTime().asSeconds();
        static const float flickerRate = 0.000001f;

        int cycle = static_cast<int>(time / flickerRate);
        bool shouldRender = true;

        if (invincible) {
            shouldRender = (cycle % 2 == 0);
        }
    
        if (shouldRender) {
            if (leader) {
                for (int i = 0; i < 2; ++i) {
                    followers[i]->render(window, cameraOffsetX);
                }
            }
            Entity::render(window, cameraOffsetX);
        }
    }
    
    // Will be defined in children classes
    virtual void specialAbility() = 0;

    virtual ~Player(){};
};

