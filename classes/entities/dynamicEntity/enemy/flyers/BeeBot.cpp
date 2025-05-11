#include "BeeBot.h"

BeeBot::BeeBot(float px, float py, int h, int w, sf::Texture* texture,
float vx, float vy, float terminal, float maxX, float minX, int hp, float maxSpeed, bool alive, bool movinRight)
: Enemy(px, py, h, w, texture, vx, vy, terminal, hp, maxSpeed, alive, movingRight),
    maxX(maxX), minX(minX), zigzagAmp(50.0f), zigzagSpeed(3.0f), originalY(py) {

    if (!walkRightTexture.loadFromFile("Data/bee_botR.png")) {
        cout << "Failed to load bat_brainR.png\n";
    }
    if (!walkLeftTexture.loadFromFile("Data/bee_botL.png")) {
        cout << "Failed to load bat_brainL.png\n";
    }

    sprite.setTexture(walkRightTexture);
    sprite.setTextureRect(sf::IntRect(0, 0, 64, 64)); // single frame
    //sprite.setScale(2.0f, 2.0f);

    // Setup animation for walking
    walkRightAnimation.initialize(&sprite, &walkRightTexture, 64, 64, 6, 0.07f);
    walkLeftAnimation.initialize(&sprite, &walkLeftTexture, 64, 64, 6, 0.07f);

    if (!texture->loadFromFile("Data/projectileD.png")) {
        cout << "Unable to load projectileD.png" << endl;
    }

    projectileManager = new ProjectileManager();
}

void BeeBot::update(float deltaTime, int &score, int volume) {
    Enemy::update(deltaTime, score, volume);

    if (!isAlive()) {
        return;
    }

    float time = movementClock.getElapsedTime().asSeconds();
    
    // using the shm formula y = Asin(wt)
    pos_y = originalY + zigzagAmp * sin(zigzagSpeed * time);

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

    if (shootTimer.getElapsedTime().asSeconds() > 5.0f) {
        projectileManager->spawn(pos_x, pos_y, true, false); // BeeBot only shoots downwards so down = true
        shootTimer.restart();
    }
    projectileManager->update(deltaTime, minX, maxX, volume); // display the projectile over the confined region
}

void BeeBot::onDeath() {
    cout << "Bee Bot destroyed!" << endl;
    projectileManager->clear();
}

void BeeBot::render(RenderWindow& window, float cameraOffsetX) {
    Enemy::render(window, cameraOffsetX);
    projectileManager->render(window, cameraOffsetX);
}

void BeeBot::checkProjectilesHitPlayer(Player& player) {
    projectileManager->checkCollisionWithPlayer(player);
}

float BeeBot::getMinX() const { return minX; }
float BeeBot::getMaxX() const { return maxX; }
void BeeBot::setMinX(float minX) { this->minX = minX; }
void BeeBot::setMaxX(float maxX) { this->maxX = maxX; }