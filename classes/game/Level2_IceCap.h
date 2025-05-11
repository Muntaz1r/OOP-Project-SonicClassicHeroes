#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>

using namespace sf;
using namespace std;

#include "Level.h"

class Level2_IceCap : public Level {
private:
    void initializeGrid();

public:
    Level2_IceCap();
    ~Level2_IceCap();

    void render(RenderWindow& window, float cameraOffsetX) override;//For exit portal
    void loadAssets(int volume) override;
};