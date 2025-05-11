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
    static int numPowerUps;
    bool knucklesInvinc;
    sf::Clock boostClock;
    sf::Time boostTime;

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
        bool leader = false, int boostTime=0);

    // Getters
    float getMaxSpeed() const;
    bool isOnGround() const;
    bool isInvincible() const;
    bool isMovingRight() const;
    float getAccnX() const;
    float getAccY() const;
    bool getLeader() const;
    Player* getFollower1() const;
    Player* getFollower2() const;
    bool getBoosting() const;
    bool getFallingIntoVoid() const;
    bool getHanging() const;
    bool getHangingRight() const;
    virtual sf::Clock getBoostClock() const;
    virtual sf::Time getBoostTime() const;
    static int getNumPowerUps();
    static int getHP();

    // Setters
    void setMaxSpeed(float speed);
    void setOnGround(bool value);
    void setInvincible(bool value);
    void setMovingRight(bool value);
    void setAccX(float value);
    void setAccY(float value);
    void setLeader(bool value);
    void setFollowers(Player* follower1, Player* follower2);
    void setGravity(float value);
    void setFriction(float value);
    void setBoosting(bool value);
    void setFallingIntoVoid(bool value);
    void setHanging(bool value);
    void setHangingRight(bool value);
    static void setHP(int value);
    static void setNumPowerUps(int value);
    
    char* sampleTile(float x, float y, int tileSize, int levelHeight, int levelWidth, char** grid, int* outRow = nullptr, int* outCol = nullptr);
    void setCollidingTiles(int tileSize, int levelHeight, int levelWidth, char** grid);
    
    // Movement
    virtual void jump();

    virtual void moveLeft();
    
    virtual void moveRight();

    virtual void flyDown(); // Tails specific ftn

    //Damage mechanics
    void takeDamage();
   
    //Updating everything relevant 
    virtual void update(float deltaTime, int &score, int volume) override;

    //Tile Checkers
    bool collidesBelow(char target);
    
    bool collidesAbove(char target);
    bool collidesLeft(char target);
    
    bool collidesRight(char target);

    //Tile setters
    void setBelow(char target);
    
    void setAbove(char target);
    
    void setLeft(char target);
    
    void setRight(char target);

    virtual void collisionHandle(float previousX, float previousY, int &score);

    virtual void render(sf::RenderWindow& window, float cameraOffsetX) override;
    
    // Will be defined in children classes
    virtual void specialAbility() = 0;
    virtual void powerUp()=0;

    virtual ~Player();
};

