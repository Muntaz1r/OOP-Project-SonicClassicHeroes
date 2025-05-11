#include "CrabMeat.h"

CrabMeat::CrabMeat(float px, float py, int h, int w, sf::Texture* texture,
float vx, float vy, float terminal, float maxX, float minX, int hp, float maxSpeed, bool alive, bool movinRight)
: Enemy(px, py, h, w, texture, vx, vy, terminal, hp, maxSpeed, alive, movingRight),
    maxX(maxX), minX(minX) {

    if (!walkRightTexture.loadFromFile("Data/crab_meat.png")) {
        cout << "Failed to load crab_meat.png\n";
    }
    if (!walkLeftTexture.loadFromFile("Data/crab_meat.png")) {
        cout << "Failed to load crab_meat.png\n";
    }

    sprite.setTexture(walkRightTexture);
    sprite.setTextureRect(sf::IntRect(0, 0, 64, 64)); // single frame
    //sprite.setScale(2.0f, 2.0f);

    // Setup animation for walking
    walkRightAnimation.initialize(&sprite, &walkRightTexture, 32, 32, 10, 0.05f);
    walkLeftAnimation.initialize(&sprite, &walkLeftTexture, 32, 32, 10, 0.05f);

    if (!texture->loadFromFile("Data/projectileR.png")) {
        cout << "Unable to load projectile.png" << endl;
    }

    projectileManager = new ProjectileManager();
}

void CrabMeat::update(float deltaTime, int &score, int volume) {
    Enemy::update(deltaTime, score, volume);

    if (!isAlive()) {
        return;
    }

    pos_x += movingRight ? velocity_x : -velocity_x;

    if (isMovingRight()) {
        pos_x += velocity_x;
    }
    else {
        pos_x += -velocity_x;
    }

    if (pos_x > maxX) {
        pos_x = maxX;
        movingRight = false;
    }
    else if (pos_x < minX) {
        pos_x = minX;
        movingRight = true;
    }

    if (shootTimer.getElapsedTime().asSeconds() > 3.0f) {
        if (isMovingRight()) { // shoot right when moving left and vice versa
            projectileManager->spawn(pos_x, pos_y, false, false);
        }
        else {
            projectileManager->spawn(pos_x, pos_y, false, true);
        }
        
        shootTimer.restart();
    }
    projectileManager->update(deltaTime, minX, maxX, volume); // display the projectile over the entire screen
}

void CrabMeat::onDeath() {
    cout << "Crab Meat destroyed!" << endl;
    projectileManager->clear();
}

void CrabMeat::render(RenderWindow& window, float cameraOffsetX) {
    Enemy::render(window, cameraOffsetX);
    projectileManager->render(window, cameraOffsetX);
}

void CrabMeat::checkProjectilesHitPlayer(Player& player) {
    projectileManager->checkCollisionWithPlayer(player);
}

float CrabMeat::getMinX() const { return minX; }
float CrabMeat::getMaxX() const { return maxX; }
void CrabMeat::setMinX(float minX) { this->minX = minX; }
void CrabMeat::setMaxX(float maxX) { this->maxX = maxX; }
