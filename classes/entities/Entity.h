#pragma once

#include <iostream>
#include <cmath>
#include <SFML/Graphics.hpp>


class Entity {
protected:
    float pos_x;
    float pos_y;
    int height;
    int width;
    sf::Texture texture;
    sf::Sprite sprite;
public:
    Entity(float x=0, float y=0, int h=0, int w=0,sf::Texture* texture = nullptr): 
    pos_x(x), pos_y(y), height(h), width(w), texture(*texture){
        sprite.setTexture(this->texture);
        // sprite.setScale(
        //     static_cast<float>(width) / this->texture.getSize().x,
        //     static_cast<float>(height) / this->texture.getSize().y
        // );
        sprite.setScale(2.0f, 2.0f);
        sprite.setTextureRect(IntRect(0, 0, 40, 40));
    }
    // Getters
    float getPosX() const { return pos_x; }
    float getPosY() const { return pos_y; }
    int getHeight() const { return height; }
    int getWidth() const { return width; }

    // Setters
    void setPosX(float x) { pos_x = x; }
    void setPosY(float y) { pos_y = y; }
    void setHeight(int h) { height = h; }
    void setWidth(int w) { width = w; }

    virtual void update(float deltaTime) = 0;

    virtual void render(sf::RenderWindow& window, float cameraOffsetX) { //render
        sprite.setPosition(pos_x - cameraOffsetX, pos_y - 19); // pos_x, pos_y inherited from Entity
        window.draw(sprite);
    }

    virtual ~Entity(){};
};

