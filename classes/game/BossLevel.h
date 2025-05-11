#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>

using namespace sf;
using namespace std;

#include "Level.h"

class BossLevel : public Level {
private:
    bool completed;
    bool failed;
    bool waitingAtBottom;

    EggStingerFactory eggStingerMaker;
    EggStinger* eggStinger;

    Clock waitClock;

    void initializeGrid();

public:
    BossLevel();
    ~BossLevel();

    void loadAssets(int volume) override;
    void update(float deltaTime) override;
    void render(RenderWindow& window, float cameraOffsetX) override;
    bool isLevelComplete() const override;
    bool getFailed();
    bool exitCheck(float cameraOffSetX) override;

};