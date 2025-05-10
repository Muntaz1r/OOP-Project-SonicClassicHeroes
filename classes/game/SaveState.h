#pragma once
#include <fstream>
#include <iostream>
#include <SFML/Graphics.hpp>
#include "flyers/BatBrain.h"
#include "flyers/BeeBot.h"
#include "crawlers/MotoBug.h"
#include "crawlers/CrabMeat.h"

using namespace std;
using namespace sf;

class SaveState {
public:
    static void saveLevel(
        string& filepath, int levelID, float timer, int score,
        float playerX, float playerY, float velX, float velY,
        int hp, char currentChar, int width, int height, char** grid,
        BatBrain** batBrains, int numBatBrains,
        BeeBot** beeBots, int numBeeBots,
        MotoBug** motoBugs, int numMotoBugs,
        CrabMeat** crabMeats, int numCrabMeats
    ) {
        ofstream myFile(filepath);
        if (!myFile.is_open()) {
            return;
        }

        myFile << "LEVEL_ID " << levelID << endl;
        myFile << "TIMER " << timer << endl;
        myFile << "SCORE " << score << endl;
        myFile << "PLAYER_POS " << playerX << " " << playerY << endl;
        myFile << "PLAYER_VEL " << velX << " " << velY << endl;
        myFile << "PLAYER_HP " << hp << endl;
        myFile << "CURRENT_CHAR " << currentChar << endl;

        myFile << "RING_GRID " << width << " " << height << endl;
        for (int i = 0; i < height; i++) {
            for (int j = 0; j < width; j++) {
                myFile << grid[i][j] << " ";
            }
            myFile << endl;
        }

        cout << "GRID SAVED" << endl;

        myFile << "BAT_BRAIN_COUNT " << numBatBrains << endl;
        for (int i = 0; i < numBatBrains; i++) {
            myFile << "BATBRAIN " << batBrains[i]->getPosX() << " "
                 << batBrains[i]->getPosY() << " "
                 << batBrains[i]->getHP() << " "
                 << (batBrains[i]->isAlive() ? 1 : 0) << " "
                 << (batBrains[i]->isMovingRight() ? 1 : 0) << endl;
        }

        myFile << "BEE_BOT_COUNT " << numBeeBots << endl;
        for (int i = 0; i < numBeeBots; i++) {
            myFile << "BEEBOT " << beeBots[i]->getPosX() << " "
                 << beeBots[i]->getPosY() << " "
                 << beeBots[i]->getHP() << " "
                 << beeBots[i]->getMinX() << " "
                 << beeBots[i]->getMaxX() << " "
                 << (beeBots[i]->isAlive() ? 1 : 0) << " "
                 << (beeBots[i]->isMovingRight() ? 1 : 0) << endl;
        }

        myFile << "MOTO_BUG_COUNT " << numMotoBugs << endl;
        for (int i = 0; i < numMotoBugs; i++) {
            myFile << "MOTOBUG " << motoBugs[i]->getPosX() << " "
                 << motoBugs[i]->getPosY() << " "
                 << motoBugs[i]->getHP() << " "
                 << motoBugs[i]->getMinX() << " "
                 << motoBugs[i]->getMaxX() << " "
                 << (motoBugs[i]->isAlive() ? 1 : 0) << " "
                 << (motoBugs[i]->isMovingRight() ? 1 : 0) << endl;
        }

        myFile << "CRAB_MEAT_COUNT " << numCrabMeats << "\n";
        for (int i = 0; i < numCrabMeats; i++) {
            myFile << "CRABMEAT " << crabMeats[i]->getPosX() << " "
                 << crabMeats[i]->getPosY() << " "
                 << crabMeats[i]->getHP() << " "
                 << crabMeats[i]->getMinX() << " "
                 << crabMeats[i]->getMaxX() << " "
                 << (crabMeats[i]->isAlive() ? 1 : 0) << " "
                 << (crabMeats[i]->isMovingRight() ? 1 : 0) << endl;
        }

        myFile.close();
    }

    static bool loadLevel(
        string& filepath,
        int& levelID, float& timer, int& score,
        float& playerX, float& playerY, float& velX, float& velY,
        int& hp, char& currentChar, int& width, int& height, char** grid,
        BatBrain**& batBrains, int& numBatBrain,
        BeeBot**& beeBots, int& numBeeBot,
        MotoBug**& motoBugs, int& numMotoBug,
        CrabMeat**& crabMeats, int& numCrabMeat
    ) {
        ifstream myFile(filepath);
        if (!myFile.is_open()) {
            return false;
        }

        string label;
        myFile >> label >> levelID;
        myFile >> label >> timer;
        myFile >> label >> score;
        myFile >> label >> playerX >> playerY;
        myFile >> label >> velX >> velY;
        myFile >> label >> hp;
        myFile >> label >> currentChar;

        myFile >> label >> width >> height; // RING_GRID
        for (int i = 0; i < height; i++) {
            for (int j = 0; j < width; j++) {
                myFile >> grid[i][j];
            }
        }

        float ex, ey, minX, maxX;
        int ehp, alive, right;

        Texture* texture = new Texture; // placeholder

        if (!texture->loadFromFile("Data/bee_botR.png")) {
            cout << "Failed to load enemy texture\n";
        }

        myFile >> label >> numBatBrain;
        if (numBatBrain > 0) { batBrains = new BatBrain*[numBatBrain]; }
        for (int i = 0; i < numBatBrain; i++) {
            myFile >> label >> ex >> ey >> ehp >> alive >> right;
            batBrains[i] = new BatBrain(ex, ey, 32, 32, texture, 1.5f, 1.5f, 0.0f, ehp, 1.0f, alive == 1, right == 1);
        }

        myFile >> label >> numBeeBot;
        if (numBeeBot > 0) { beeBots = new BeeBot*[numBeeBot]; }
        for (int i = 0; i < numBeeBot; i++) {
            myFile >> label >> ex >> ey >> ehp >> minX >> maxX >> alive >> right;
            beeBots[i] = new BeeBot(ex, ey, 64, 64, texture, 1.5f, 0.0f, 0.0f, maxX, minX, ehp, 1.0f, alive == 1, right == 1);
        }

        myFile >> label >> numMotoBug;
        if (numMotoBug > 0) { motoBugs = new MotoBug*[numMotoBug]; }
        for (int i = 0; i < numMotoBug; i++) {
            myFile >> label >> ex >> ey >> ehp >> minX >> maxX >> alive >> right;
            motoBugs[i] = new MotoBug(ex, ey, 32, 32, texture, 1.5f, 0.0f, 0.0f, maxX, minX, ehp, 1.0f, alive == 1, right == 1);
        }

        myFile >> label >> numCrabMeat;
        if (numCrabMeat > 0) { crabMeats = new CrabMeat*[numCrabMeat]; }
        for (int i = 0; i < numCrabMeat; i++) {
            myFile >> label >> ex >> ey >> ehp >> minX >> maxX >> alive >> right;
            crabMeats[i] = new CrabMeat(ex, ey, 32, 32, texture, 1.5f, 0.0f, 0.0f, maxX, minX, ehp, 1.0f, alive == 1, right == 1);
        }

        myFile.close();
        return true;
    }
};
