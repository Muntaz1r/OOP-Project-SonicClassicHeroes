#include "Enemy.h"

Enemy::Enemy(float px, float py, int h, int w, sf::Texture* texture,
float vx, float vy, float terminal, int hp, float maxSpeed, bool alive, bool movinRight)
: DynamicEntity(px, py, h, w, texture, vx, vy, terminal),
    hp(hp), maxSpeed(maxSpeed), alive(alive), movingRight(movingRight) {
    enemySounds = new SoundManager();

    // Index 0: Hit
    // Index 1: Enemy Hit
    enemySounds->loadSound(0, "Data/sfx/hit.wav");
    enemySounds->loadSound(1, "Data/sfx/enemy_hit.wav");
}

Enemy::~Enemy() {
    if (enemySounds) {delete enemySounds; enemySounds = nullptr;}
}

void Enemy::takeDamage(int amount) {
    if (!alive) {
        return;
    }
    hp -= amount;
    if (hp <= 0) {
        alive = false;
        onDeath();
    }
}

void Enemy::onDeath() {
    
}

bool Enemy::isAlive() const { return alive; }
bool Enemy::isMovingRight() const { return movingRight; }
int Enemy::getHP() const { return hp; }
void Enemy::setHP(int hp) { this->hp = hp; }
void Enemy::setIsAlive(bool alive) { this->alive = alive; }
void Enemy::setIsMovingRight(bool movingRight) { this->movingRight = movingRight; }

void Enemy::update(float deltaTime, int &score, int volume) {
    enemyVolume = volume;
    enemySounds->setVolume(enemyVolume);
    if (isMovingRight()) {
        if (sprite.getTexture() != &walkRightTexture) {
            sprite.setTexture(walkRightTexture);
        }
        walkRightAnimation.update(deltaTime);
    }
    else {
        if (sprite.getTexture() != &walkLeftTexture) {
            sprite.setTexture(walkLeftTexture);
        }
        walkLeftAnimation.update(deltaTime);
    }
}

void Enemy::checkCollisionWithPlayer(Player& player) {
    if (!isAlive()) return;

    float playerLeft = player.getLeft();
    float playerRight = player.getRight();
    float playerTop = player.getTop();
    float playerBottom = player.getBottom();

    float enemyLeft = getLeft();
    float enemyRight = getRight();
    float enemyTop = getTop();
    float enemyBottom = getBottom();

    bool isOverlapping = playerRight > enemyLeft && playerLeft < enemyRight && playerBottom > enemyTop && playerTop < enemyBottom;

    if (isOverlapping) {
        if (playerBottom <= enemyTop + 10.0f && player.getVelocityY() > 0) {
            enemySounds->play(1);
            takeDamage(1);
            player.setVelocityY(-5.0f); // Bounce effect
            cout << "Enemy took damage." << endl;
        } else if (!player.isInvincible() && player.isOnGround()) {
            enemySounds->play(0);
            player.takeDamage();
            cout << "Player took damage." << endl;
        }
    }
}

void Enemy::render(sf::RenderWindow& window, float cameraOffsetX) {
    if (isAlive()) {
        Entity::render(window, cameraOffsetX);
    }
}