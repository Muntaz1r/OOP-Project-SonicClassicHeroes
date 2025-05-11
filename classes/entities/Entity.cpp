#include "Entity.h"

Entity::Entity(float x, float y, int h, int w, sf::Texture* texture): 
    pos_x(x), pos_y(y), height(h), width(w), texture(*texture){
        sprite.setTexture(this->texture);
        // sprite.setScale(
        //     static_cast<float>(width) / this->texture.getSize().x,
        //     static_cast<float>(height) / this->texture.getSize().y
        // );
        sprite.setScale(2.0f, 2.0f);
        sprite.setTextureRect(sf::IntRect(0, 0, 40, 40));
}

// Getters
float Entity::getPosX() const { return pos_x; }
float Entity::getPosY() const { return pos_y; }
int Entity::getHeight() const { return height; }
int Entity::getWidth() const { return width; }

// HitBox for collison
float Entity::getLeft() const { return pos_x; }
float Entity::getRight() const { return pos_x + width; }
float Entity::getTop() const { return pos_y; }
float Entity::getBottom() const { return pos_y + height; }


// Setters
void Entity::setPosX(float x) { pos_x = x; }
void Entity::setPosY(float y) { pos_y = y; }
void Entity::setHeight(int h) { height = h; }
void Entity::setWidth(int w) { width = w; }


void Entity::render(sf::RenderWindow& window, float cameraOffsetX) { //render
    sprite.setPosition(pos_x - cameraOffsetX, pos_y); // pos_x, pos_y inherited from Entity
    window.draw(sprite);
}

Entity::~Entity(){};