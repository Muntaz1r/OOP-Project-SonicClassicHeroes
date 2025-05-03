#include "Animation.h"

Animation::Animation() {
    targetSprite = nullptr;
    texture = nullptr;
    frameWidth = frameHeight = frameCount = currentFrame = 0;
    frameDuration = elapsed = 0.0f;
}

void Animation::initialize(Sprite* sprite, Texture* texture,
    int frameWidth, int frameHeight, int frameCount, float frameDuration) {
        this->targetSprite = sprite;
    this->texture = texture;
    this->frameWidth = frameWidth;
    this->frameHeight = frameHeight;
    this->frameCount = frameCount;
    this->frameDuration = frameDuration;
    this->currentFrame = 0;
    this->elapsed = 0.0f;

    sprite->setTexture(*texture);
    sprite->setTextureRect(IntRect(0, 0, frameWidth, frameHeight));
}

void Animation::update(float deltaTime) {
    elapsed += deltaTime;
    if (elapsed >= frameDuration) {
        elapsed = 0.0f;
        currentFrame = (currentFrame + 1) % frameCount;

        targetSprite->setTextureRect(IntRect(currentFrame * frameWidth, 0, frameWidth, frameHeight));
    }
}

void Animation::reset() {
    currentFrame = 0;
    elapsed = 0.0f;
    targetSprite->setTextureRect(IntRect(0, 0, frameWidth, frameHeight));
}