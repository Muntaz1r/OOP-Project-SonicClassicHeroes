#include "Boss.h"

EggStinger::EggStinger(float px, float py, int h, int w, sf::Texture* texture,
float vx, float vy, float terminal, float maxX, float minX, int hp, float maxSpeed, bool alive, bool movinRight)
: Enemy(px, py, h, w, texture, vx, vy, terminal, hp, maxSpeed, alive, movingRight),
    maxX(maxX), minX(minX), shouldDrop(false), dropping(false), waitingAtBottom(false), goingUp(false), originalY(py) {

    if (!walkRightTexture.loadFromFile("Data/eggstinger/flyingR.png")) {
        cout << "Failed to load eggstinger/flyingR.png\n";
    }
    if (!walkLeftTexture.loadFromFile("Data/eggstinger/flyingL.png")) {
        cout << "Failed to load eggstinger/flyingL.png\n";
    }

    if (!droppingRightTexture.loadFromFile("Data/eggstinger/droppingR.png")) {
        cout << "Failed to load eggstinger/droppingR.png\n";
    }
    if (!droppingLeftTexture.loadFromFile("Data/eggstinger/droppingL.png")) {
        cout << "Failed to load eggstinger/droppingL.png\n";
    }

    sprite.setTexture(walkRightTexture);
    sprite.setTextureRect(sf::IntRect(0, 0, h, w)); // single frame
    //sprite.setScale(2.0f, 2.0f);

    // Setup animation for walking
    walkRightAnimation.initialize(&sprite, &walkRightTexture, 96, 96, 12, 0.05f);
    walkLeftAnimation.initialize(&sprite, &walkLeftTexture, 96, 96, 12, 0.05f);

    droppingRightAnimation.initialize(&sprite, &droppingRightTexture, 96, 96, 12, 0.05f);
    droppingLeftAnimation.initialize(&sprite, &droppingLeftTexture, 96, 96, 12, 0.05f);

    if (!texture->loadFromFile("Data/eggstinger/idle.png")) {
        cout << "Unable to load eggstinger/idle.png" << endl;
    }
}

void EggStinger::update(float deltaTime, float playerX, int& score, int volume) {
    Enemy::update(deltaTime, score, volume);

    if (!isAlive()) {
        return;
    }

    if (!dropping && !waitingAtBottom && !goingUp) { // move left and right across the top of the screen
        pos_x += (movingRight ? velocity_x : -velocity_x);

        if (pos_x > maxX) {
            pos_x = maxX;
            movingRight = false;
        }
        else if (pos_x < minX) {
            pos_x = minX;
            movingRight = true;
        }

        if (dropClock.getElapsedTime().asSeconds() > 10.0f && !shouldDrop) {
            shouldDrop = true;
            targetX = playerX;
            targetIndexX = static_cast<int>(targetX) / (int)64 + 1;
        }

        if (shouldDrop) {
            float dx = targetX - pos_x;

            // For animation
            if (dx < 0) {
                movingRight = false;
            }
            else {
                movingRight = true;
            }

            float distance = sqrt(dx * dx);
            if (distance > 2.0f) { // small distance to avoid jitteryness
                pos_x += (dx / distance) * velocity_x;
            }
            else {
                pos_x -= 55;
                shouldDrop = true;
                dropping = true;
            }
        }
    }
    else if (dropping) {

        if (isMovingRight()) {
            if (sprite.getTexture() != &droppingRightTexture) {
                sprite.setTexture(droppingRightTexture);
            }
            droppingRightAnimation.update(deltaTime);
        }
        else {
            if (sprite.getTexture() != &droppingLeftTexture) {
                sprite.setTexture(droppingLeftTexture);
            }
            droppingLeftAnimation.update(deltaTime);
        }

        if (velocity_y < 0) {
            velocity_y *= -1;
        }

        pos_y += velocity_y;

        if (pos_y >= 672.0f) {
            pos_y = 672.0f;
            shouldDrop = false;
            dropping = false;
            waitingAtBottom = true;
            waitClock.restart();
        }
    }
    else if (waitingAtBottom) {
        // pos_y += (rand() % 5) - 2; // random number between -2 and 2 to emulate shake
        // pos_x += (rand() % 5) - 2;
        if (waitClock.getElapsedTime().asSeconds() > 1.5f) {
            waitingAtBottom = false;
            goingUp = true;
            velocity_y *= -1;
        }
    }
    else if (goingUp) {
        pos_y += velocity_y;

        if (pos_y <= originalY) {
            pos_y = originalY;
            goingUp = false;
            dropClock.restart();
        }
    }
}

void EggStinger::render(RenderWindow& window, float cameraOffsetX) {
    Enemy::render(window, cameraOffsetX);
}

void EggStinger::onDeath() {
}


bool EggStinger::isDropping() const { return dropping; }
float EggStinger::getTargetX() const { return targetX; }
int EggStinger::getTargetIndexX() const { return targetIndexX; } // gives the x-cord of the grid 
