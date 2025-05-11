#include "Projectile.h"

Projectile::Projectile(float px, float py, int h, int w, Texture* texture, float vx, float vy, float terminal, int dmg, bool active, bool down, bool right)
: DynamicEntity(px, py, h, w, texture, vx, vy, terminal),
damage(damage), active(active), movingDown(down), movingRight(right) {
    if (!projectileTextureD.loadFromFile("Data/projectileD.png")) {
        cout << "Unable to load projectileD.png" << endl;
    }

    if (!projectileTextureR.loadFromFile("Data/projectileR.png")) {
        cout << "Unable to load projectileR.png" << endl;
    }

    if (!projectileTextureL.loadFromFile("Data/projectileL.png")) {
        cout << "Unable to load projectileL.png" << endl;
    }

    sprite.setTexture(this->texture);
    sprite.setScale(2.0f, 2.0f);
    sprite.setTextureRect(IntRect(0, 0, width, height));
}

Projectile::~Projectile() {
}

void Projectile::update(float deltaTime, float minX, float maxX, int volume) {
    projectileVolume = volume;

    if (!isActive()) {
        return;
    }

    pos_x += velocity_x * deltaTime;
    pos_y += velocity_y * deltaTime;

    if (pos_x < minX || pos_x > maxX || pos_y > 960.0f) { // off the map
        active = false;
    }

    if (isMovingDown()) {
        if (sprite.getTexture() != &projectileTextureD) {
            sprite.setTexture(projectileTextureD);
        }
    }
    else {
        if (isMovingRight()) {
            if (sprite.getTexture() != &projectileTextureR) {
                sprite.setTexture(projectileTextureR);
            }
        }
        else {
            if (sprite.getTexture() != &projectileTextureL) {
                sprite.setTexture(projectileTextureL);
            }
        }
    }
}

void Projectile::render(RenderWindow& window, float cameraOffsetX) {
    if (isActive()) {
        Entity::render(window, cameraOffsetX);
    }
}

void Projectile::checkCollision(Player& player) {
    float playerLeft = player.getLeft();
    float playerRight = player.getRight();
    float playerTop = player.getTop();
    float playerBottom = player.getBottom();

    float projectileLeft = getLeft();
    float projectileRight = getRight();
    float projectileTop = getTop();
    float projectileBottom = getBottom();

    bool isOverlapping = playerRight > projectileLeft && playerLeft < projectileRight && playerBottom > projectileTop && playerTop < projectileBottom;
    
    if (isOverlapping) {
        player.takeDamage();
        deactivate();
        cout << "Player took damage from bullet." << endl;
    }
}

bool Projectile::isActive() const { return active; }
bool Projectile::isMovingDown() const { return movingDown; }
bool Projectile::isMovingRight() const { return movingRight; }

void Projectile::deactivate() { active = false; }
