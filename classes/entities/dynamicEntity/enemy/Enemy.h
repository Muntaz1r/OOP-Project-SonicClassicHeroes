#pragma once

#include <iostream>
#include <SFML/System/Clock.hpp>
#include <cmath>

#include "DynamicEntity.h"
#include "SoundManager.h"
#include "Player.h"

class Enemy : public DynamicEntity {
protected:
    int hp;
    float maxSpeed;
    bool alive;
    bool movingRight;

    int enemyVolume;
    SoundManager* enemySounds;

    Texture walkRightTexture;
    Texture walkLeftTexture;

public:
    Enemy(float px = 0, float py = 0, int h = 0, int w = 0, sf::Texture* texture = nullptr,
    float vx = 0, float vy = 0, float terminal = 0, int hp = 3, float maxSpeed = 0, bool alive = true, bool movinRight = true);

    virtual void takeDamage(int amount = 1);

    virtual void onDeath();

    bool isAlive() const;
    bool isMovingRight() const;
    int getHP() const;

    void setHP(int hp);
    void setIsAlive(bool alive);
    void setIsMovingRight(bool movingRight);

    virtual void update(float deltaTime, int &score, int volume) override;

    virtual void checkCollisionWithPlayer(Player& player);

    virtual void render(sf::RenderWindow& window, float cameraOffsetX);
};