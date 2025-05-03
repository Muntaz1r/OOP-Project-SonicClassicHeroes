#include "Scoreboard.h"
#include <fstream>

Scoreboard::Scoreboard(int screen_x, int screen_y) : screen_x(screen_x), screen_y(screen_y) {
    filePath = "Data/leaderboard.txt";
    numScores = 0;

    font.loadFromFile("Data/Retro Gaming.ttf");

    title.setFont(font);
    title.setString("Top 10 Scores: ");
    title.setFillColor(Color::Cyan);
    title.setCharacterSize(36);
    title.setPosition(screen_x / 2.0 - 200, 180);
    
    for (int i = 0; i < 10; i++) {
        scoreTexts[i].setFont(font);
        scoreTexts[i].setCharacterSize(26);
        scoreTexts[i].setFillColor(Color::White);
    }

    loadScores();
}

void Scoreboard::loadScores() {
    ifstream myFile(filePath);
    numScores = 0;

    while (numScores < 10 && myFile >> scores[numScores].name >> scores[numScores].score) {
        numScores++;        
    }

    myFile.close();
}

void Scoreboard::saveScores(const string name, int score) {
    scores[numScores].name = name;
    scores[numScores].score = score;
    numScores++;

    // sort descending
    for (int i = 0; i < numScores; i++) {
        for (int j = 0; j < numScores - i - 1; j++) {
            if (scores[j].score < scores[j + 1].score) {
                ScoreRecord tempScore = scores[j + 1];
                scores[j + 1] = scores[j];
                scores[j] = tempScore;
            }
        }
    }

    if (numScores > 10) { // only use the top 10
        numScores = 10;
    }

    // write to file
    ofstream myFile(filePath);

    for (int i = 0; i < numScores; i++) {
        myFile << scores[i].name << " " << scores[i].score << endl;
    }

    myFile.close();
}

void Scoreboard::draw(RenderWindow& window) {
    window.draw(title);

    if (numScores == 0) {
        Text noScores;
        noScores.setFont(font);
        noScores.setString("No scores recorded!");
        noScores.setCharacterSize(26);
        noScores.setFillColor(Color::White);
        noScores.setPosition(screen_x / 2.0 - 200, 250);
        window.draw(noScores);
    }
    else {
        for (int i = 0; i < numScores; i++) {
            scoreTexts[i].setString(to_string(i + 1) + ", " + scores[i].name + " - " + to_string(scores[i].score));
            scoreTexts[i].setPosition(screen_x / 2.0 - 200, 250 + i * 40);
            window.draw(scoreTexts[i]);
        }
    }
}

