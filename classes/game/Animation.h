#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>

using namespace sf;
using namespace std;

class Animation {
private:
    Sprite* targetSprite;
    Texture* texture;

    int frameHeight;
    int frameWidth;
    int frameCount;
    int currentFrame;
    float frameDuration;
    float elapsed;

public:
    Animation();
    void initialize(Sprite* sprite, Texture* texture,
        int frameWidth, int frameHeight, int frameCount, float frameDuration);
    void update(float deltaTime);
    void reset();
};