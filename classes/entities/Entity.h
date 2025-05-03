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
    Entity(float x=0, float y=0, int h=0, int w=0): pos_x(x), pos_y(y), height(h), width(w){
        if (!texture.loadFromFile("Data/0right.png")) {
            std::cerr << "Failed to load player texture\n";
        }
        sprite.setTexture(texture);
        sprite.setScale(
            static_cast<float>(width) / texture.getSize().x,
            static_cast<float>(height) / texture.getSize().y
        );
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

    virtual void update() = 0;

    void render(sf::RenderWindow& window) { //render
        sprite.setPosition(pos_x, pos_y); // pos_x, pos_y inherited from Entity
        window.draw(sprite);
    }

    virtual ~Entity(){};
};

