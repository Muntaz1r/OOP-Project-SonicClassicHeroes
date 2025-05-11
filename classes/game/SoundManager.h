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
    SoundManager();

    void loadSound(int index, const string& filepath);
    void play(int index);
    void setVolume(int volume);
};
