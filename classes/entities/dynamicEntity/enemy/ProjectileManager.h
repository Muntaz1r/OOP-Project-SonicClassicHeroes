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
    ProjectileManager() : numProjectiles(0) {
        if (!projectileTexture.loadFromFile("Data/projectileD.png")) {
            cout << "Failed to load projectile.png" << endl;
        }
    
        for (int i = 0; i < MAX_PROJECTILES; ++i) {
            projectiles[i] = nullptr;
        }
    }
    ~ProjectileManager() {
        clear();
    }

    void spawn(float px, float py, bool down = false, bool right = true) {
        for (int i = 0; i < MAX_PROJECTILES; i++) {
            if (projectiles[i] == nullptr || !projectiles[i]->isActive()) {
                if (projectiles[i]) {
                    delete projectiles[i];
                }

                float vx;
                float vy;

                if (down) { // for shooting downwards
                    vx = -50.0f + static_cast<float>(rand() % 101); // between -50 and 50
                    vy = 180.0f + static_cast<float>(rand() % 41);  // speed between 180 and 220
                }
                else { // for horizontal shooting only
                    vx = 180.0f + static_cast<float>(rand() % 41);  // speed between 180 and 220
                    vy = 0.0f;

                    vx = (right ? vx : -vx);
                }
                
                projectiles[i] = new Projectile(px, py, 32, 32, &projectileTexture, vx, vy, 0.0f, 1, true, down, right);
                return;
            }
        }
    }
    
    void update(float deltaTime, float minX, float maxX, int volume) {
        for (int i = 0; i < MAX_PROJECTILES; ++i) {
            if (projectiles[i] && projectiles[i]->isActive()) {
                projectiles[i]->update(deltaTime, minX, maxX, volume);
            }
        }
    }

    void render(sf::RenderWindow& window, float cameraOffsetX) {
        for (int i = 0; i < MAX_PROJECTILES; ++i) {
            if (projectiles[i] && projectiles[i]->isActive()) {
                projectiles[i]->render(window, cameraOffsetX);
            }
        }
    }

    void checkCollisionWithPlayer(Player& player) {
        for (int i = 0; i < MAX_PROJECTILES; ++i) {
            if (projectiles[i] && projectiles[i]->isActive()) {
                projectiles[i]->checkCollision(player);
            }
        }
    }

    void clear() {
        for (int i = 0; i < MAX_PROJECTILES; ++i) {
            if (projectiles[i]) {
                delete projectiles[i];
                projectiles[i] = nullptr;
            }
        }
    }
};