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
    Entity(float x=0, float y=0, int h=0, int w=0,sf::Texture* texture = nullptr);
    // Getters
    float getPosX() const;
    float getPosY() const;
    int getHeight() const ;
    int getWidth() const;
    // Hitbox for collision
    float getLeft() const;
    float getRight() const;
    float getTop() const;
    float getBottom() const;


   // Setters
    void setPosX(float x);
    void setPosY(float y);
    void setHeight(int h); 
    void setWidth(int w);

    virtual void update(float deltaTime, int &score, int volume) = 0;
    //virtual void update(float deltaTime, float playerX, float playerY) = 0;

    virtual void render(sf::RenderWindow& window, float cameraOffsetX);

    virtual ~Entity();
};

