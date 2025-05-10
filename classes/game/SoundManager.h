#pragma once
#include <SFML/Audio.hpp>
#include <iostream>

using namespace std;
using namespace sf;

class SoundManager {
private:
    static const int MAX_SOUNDS = 6;

    SoundBuffer buffers[MAX_SOUNDS];
    Sound sounds[MAX_SOUNDS];
    int volume;

public:
    SoundManager() : volume(40) {}

    void loadSound(int index, const string& filepath) {
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

    void play(int index) {
        if (index < 0 || index >= MAX_SOUNDS) {
            return;
        }
        sounds[index].play();
    }

    void setVolume(int volume) {
        this->volume = volume;
        for (int i = 0; i < MAX_SOUNDS; i++) {
            sounds[i].setVolume(volume);
        }
    }
};
