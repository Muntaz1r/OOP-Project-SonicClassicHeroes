#include "SoundManager.h"

SoundManager::SoundManager() : volume(40) {}

void SoundManager::loadSound(int index, const string& filepath) {
    if (index < 0 || index >= MAX_SOUNDS) {
        return;
    }

    if (!buffers[index].loadFromFile(filepath)) {
        cout << "Failed to load sound file" << filepath << endl;
        return;
    }

    sounds[index].setBuffer(buffers[index]);
    sounds[index].setVolume(volume);
}

void SoundManager::play(int index) {
    if (index < 0 || index >= MAX_SOUNDS) {
        return;
    }
    sounds[index].play();
}

void SoundManager::setVolume(int volume) {
    this->volume = volume;
    for (int i = 0; i < MAX_SOUNDS; i++) {
        sounds[i].setVolume(volume);
    }
}