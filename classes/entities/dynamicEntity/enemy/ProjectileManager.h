#pragma once

#include "Projectile.h"
#include "Player.h"

class ProjectileManager {
private:
    static const int MAX_PROJECTILES = 100;
    Projectile* projectiles[MAX_PROJECTILES];
    int numProjectiles;
    Texture projectileTexture;

public:
    ProjectileManager();
    ~ProjectileManager();

    void spawn(float px, float py, bool down = false, bool right = true);
    
    void update(float deltaTime, float minX, float maxX, int volume);

    void render(sf::RenderWindow& window, float cameraOffsetX);

    void checkCollisionWithPlayer(Player& player);

    void clear();
};