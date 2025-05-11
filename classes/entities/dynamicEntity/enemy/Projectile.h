#pragma once

#include <iostream>
#include <SFML/Graphics.hpp>

#include "DynamicEntity.h"
#include "Player.h"
#include "SoundManager.h"

using namespace std;
using namespace sf;

class Projectile : public DynamicEntity {
private:
    int damage;

    int projectileVolume;

    bool active;
    bool movingDown;
    bool movingRight;

    Texture projectileTextureD;
    Texture projectileTextureR;
    Texture projectileTextureL;

    SoundManager* projectileSounds;

public:
    Projectile(float px, float py, int h = 0, int w = 0, Texture* texture = nullptr, float vx = 0, float vy = 0, float terminal = 0, int dmg = 1, bool active = true, bool down = false, bool right = true);

    void update(float deltaTime) {}
    void update(float deltaTime, float minX, float maxX, int volume);
    void render(RenderWindow& window, float cameraOffsetX);
    void checkCollision(Player& player);

    // Getters
    bool isActive() const;
    bool isMovingDown() const;
    bool isMovingRight() const;

    // Setters
    void deactivate();
};