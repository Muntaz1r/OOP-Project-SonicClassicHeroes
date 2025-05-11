#pragma once
#include "Enemy.h"
#include <iostream>

class EggStinger : public Enemy {
private:
    float maxX; // to confine it to a region of the screen
    float minX;

    bool shouldDrop;
    bool dropping;
    bool waitingAtBottom;
    bool goingUp;

    Clock dropClock;
    Clock waitClock;

    float targetX;
    int targetIndexX;
    float originalY;

    Texture droppingRightTexture;
    Texture droppingLeftTexture;

    Animation droppingRightAnimation;
    Animation droppingLeftAnimation;

public:
    EggStinger(float px = 0, float py = 0, int h = 96, int w = 96, sf::Texture* texture = nullptr,
    float vx = 0, float vy = 0, float terminal = 0, float maxX = 0, float minX = 0, int hp = 15, float maxSpeed = 1.0f, bool alive = true, bool movinRight = true);

    void update(float deltaTime, float playerX, int& score, int volume);
    void render(RenderWindow& window, float cameraOffsetX = 0);
    void onDeath() override;

    bool isDropping() const;
    float getTargetX() const;
    int getTargetIndexX() const; // gives the x-cord of the grid 

};