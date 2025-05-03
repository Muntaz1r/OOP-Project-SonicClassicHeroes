#pragma once

#include <iostream>
#include <cmath>
#include "Entity.h"


class DynamicEntity : public Entity {
protected:
    float velocity_x;
    float velocity_y;
    float terminal_velocity;
public:
    DynamicEntity(float px=0, float py=0, int h=0, int w = 0, 
        float vx = 0, float vy = 0, float terminal = 0)
        : Entity(px, py, h, w), velocity_x(vx), velocity_y(vy), terminal_velocity(terminal){}
    // Getters
    float getVelocityX() const { return velocity_x; }
    float getVelocityY() const { return velocity_y; }
    float getTerminalVelocity() const { return terminal_velocity; }

    // Setters
    void setVelocityX(float vx) { velocity_x = vx; }
    void setVelocityY(float vy) { velocity_y = vy; }    
    void setTerminalVelocity(float tv) { terminal_velocity = tv; }

    virtual void update(){
        pos_x += velocity_x;
        pos_y += velocity_y; 
    } 
    virtual ~DynamicEntity(){};
};
    