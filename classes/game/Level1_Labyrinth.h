
#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>

using namespace sf;
using namespace std;

#include "Level.h"

class Level1_Labyrinth : public Level {
private:
    void initializeGrid();
public:
    Level1_Labyrinth();
    ~Level1_Labyrinth();

    void loadAssets(int volume) override;
};