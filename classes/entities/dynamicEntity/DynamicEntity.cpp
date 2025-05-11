#include "DynamicEntity.h"

DynamicEntity::DynamicEntity(float px=0, float py=0, int h=0, int w = 0, sf::Texture* texture = nullptr, 
    float vx = 0, float vy = 0, float terminal = 0)
    : Entity(px, py, h, w, texture), velocity_x(vx), velocity_y(vy), 
    terminal_velocity(terminal){
        //Constructor
    }

// Getters
float DynamicEntity::getVelocityX() const { return velocity_x; }
float DynamicEntity::getVelocityY() const { return velocity_y; }
float DynamicEntity::getTerminalVelocity() const { return terminal_velocity; }
//Animation& getAnimation() { return runAnimation; }
sf::Sprite& DynamicEntity::getSprite() { return sprite; }

// Setters
void DynamicEntity::setVelocityX(float vx) { velocity_x = vx; }
void DynamicEntity::setVelocityY(float vy) { velocity_y = vy; }    
void DynamicEntity::setTerminalVelocity(float tv) { terminal_velocity = tv; }

void DynamicEntity::update(float deltaTime, int &score, int volume){
    pos_x += velocity_x;
    pos_y += velocity_y; 
} 
DynamicEntity::~DynamicEntity(){};

void DynamicEntity::updateAnimation(float deltaTime) {
    walkRightAnimation.update(deltaTime);
}

void DynamicEntity::resetAnimation() {
    walkRightAnimation.reset();
}