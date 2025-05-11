#pragma once

#include <iostream>
#include <cmath>
#include "Entity.h"
#include "Animation.h"

class DynamicEntity : public Entity {
protected:
    float velocity_x;
    float velocity_y;
    float terminal_velocity;

    Animation walkRightAnimation;
    Animation walkLeftAnimation;
    Animation jumpRightAnimation;
    Animation jumpLeftAnimation;
    
public:
    DynamicEntity(float px=0, float py=0, int h=0, int w = 0, sf::Texture* texture = nullptr, 
        float vx = 0, float vy = 0, float terminal = 0);
        
    // Getters
    float getVelocityX() const;
    float getVelocityY() const;
    float getTerminalVelocity() const;
    //Animation& getAnimation();
    sf::Sprite& getSprite();

    // Setters
    void setVelocityX(float vx);
    void setVelocityY(float vy);    
    void setTerminalVelocity(float tv);

    virtual void update(float deltaTime, int &score, int volume);
    virtual ~DynamicEntity();

    void updateAnimation(float deltaTime);

    void resetAnimation();
};


    