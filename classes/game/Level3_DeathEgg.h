#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>

using namespace sf;
using namespace std;

#include "Level.h"

class Level3_DeathEgg : public Level {
private:
    void initializeGrid();

public:
    Level3_DeathEgg();
    ~Level3_DeathEgg();

    void loadAssets(int volume) override;
};