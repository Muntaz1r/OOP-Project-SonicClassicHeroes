#pragma once

#include "SFML/System/Clock.hpp"
#include <iostream>
#include <cmath>

#include "Projectile.h"
#include "ProjectileManager.h"
#include "Enemy.h"

class CrabMeat : public Enemy {
private:
    float maxX; // to confine it to a region of the screen
    float minX;
    Clock shootTimer;
    Clock movementClock;
    
    ProjectileManager* projectileManager;

public:
    CrabMeat(float px = 0, float py = 0, int h = 64, int w = 64, sf::Texture* texture = nullptr,
    float vx = 0, float vy = 0, float terminal = 0, float maxX = 0, float minX = 0, int hp = 4, float maxSpeed = 1.0f, bool alive = true, bool movinRight = true);

    void update(float deltaTime, int &score, int volume) override;

    void onDeath() override;

    void render(RenderWindow& window, float cameraOffsetX);

    void checkProjectilesHitPlayer(Player& player);

    float getMinX() const;
    float getMaxX() const;
    void setMinX(float minX);
    void setMaxX(float maxX);
};
