#include "Menu.h"

Menu::Menu(RenderWindow& window, Music& music, int screen_x, int screen_y) : window(window), music(music), screen_x(screen_x), screen_y(screen_y) {
    currentState = STATE_MAIN;
    volume = 40; 
    mainSelected = 0;
    selectedLevel = 0;
    isMuted = false;
    prevVolume = volume;
    leaderboardRequested = false;
    gameStarted = false;
    wantsToContinue = false;
    
    font.loadFromFile("Data/Retro Gaming.ttf");

    title.setFont(font);
    title.setString("WELCOME TO SONIC CLASSIC HEROES");
    title.setCharacterSize(40);
    title.setPosition(80, 30);
    title.setFillColor(Color::Cyan);

    for (int i = 0; i < menuItemCount; i++) {
        menuText[i].setFont(font);
        menuText[i].setString(mainOptions[i]);
        menuText[i].setCharacterSize(28);
        menuText[i].setPosition(100, 120 + i * 60);
    }

    for (int i = 0; i < 4; i++) {
        levelText[i].setFont(font);
        levelText[i].setCharacterSize(28);
        levelText[i].setPosition(100, 120 + i * 60);
    }

    volumeText.setFont(font);
    volumeText.setCharacterSize(26);
    volumeText.setPosition(100, 120);

    namePrompt.setFont(font);
    namePrompt.setString("Enter your name: ");
    namePrompt.setCharacterSize(26);
    namePrompt.setPosition(100, 120);
    namePrompt.setFillColor(Color::White);

    nameText.setFont(font);
    nameText.setCharacterSize(26);
    nameText.setPosition(100, 170);
    nameText.setFillColor(Color::Yellow);

    if (!bgTexture.loadFromFile("Data/sonic-menu-bg.jpg")) {
        cout << "Failed to load background image" << endl;
    }

    bgWidth = 3400;
    bgHeight = 2276;

    bgSprite.setTexture(bgTexture);
    bgSprite.setScale((float)screen_x / bgWidth, (float)screen_y / bgHeight);
    bgSprite.setColor(Color(255, 255, 255, 150)); // lower obapacity

    music.setVolume(volume);
    music.setLoop(true);
    
    menuSounds = new SoundManager();
    // Index 0: Navigation
    // Index 1: Selection
    menuSounds->loadSound(0, "Data/sfx/menu_nav.wav");
    menuSounds->loadSound(1, "Data/sfx/menu_select.wav");

    if (isMuted) {
        menuSounds->setVolume(0);
    } else {
        menuSounds->setVolume(volume);
    }
}

Menu::~Menu() {
    delete menuSounds;
}

void Menu::handleInput(Event& event) {
    if (currentState == STATE_MAIN) {
        if (event.type == Event::KeyPressed) {
            if (event.key.code == Keyboard::Up) {
                mainSelected = (mainSelected - 1 + menuItemCount) % menuItemCount; // implement wrap around feature
                menuSounds->play(0);
            }
            
            if (event.key.code == Keyboard::Down) {
                mainSelected = (mainSelected + 1) % menuItemCount;
                menuSounds->play(0);
            }

            if (event.key.code == Keyboard::Enter) {
                menuSounds->play(1);
                if (mainSelected == 0) {
                    currentState = STATE_NAME_INPUT;
                    playerName.clear();
                }
                else if (mainSelected == 1) {
                    currentState = STATE_OPTIONS;
                }
                else if (mainSelected == 2) {
                    cout << "CONTINUE SAVED GAME" << endl;
                    wantsToContinue = true;
                }
                else if (mainSelected == 3) {
                    currentState = STATE_LEADERBOARD;
                    leaderboardRequested = true;
                    cout << "Menu: Leaderboard selected" << endl;
                }
                else if (mainSelected == 4) {
                    currentState = STATE_SELECT_LEVEL;
                }
                else if(mainSelected == 5) {
                    currentState = STATE_EXIT;
                }
            }
        }
    }
    else if (currentState == STATE_OPTIONS) {
        if (event.type == Event::KeyPressed) {
            menuSounds->setVolume(isMuted ? 0 : volume);
            if (event.key.code == Keyboard::Up) {
                menuSounds->play(0);
                if (isMuted) {
                    volume = prevVolume;
                    isMuted = false;
                }
                volume += 5;
                if (volume > 100) volume = 100;
                music.setVolume(volume);
            }
            
            if (event.key.code == Keyboard::Down) {
                menuSounds->play(0);
                if (isMuted) {
                    volume = prevVolume;
                    isMuted = false;
                }
                volume -= 5;
                if (volume < 0) volume = 0;
                music.setVolume(volume);
            }

            if (event.key.code == Keyboard::M) {
                menuSounds->play(0);
                if (!isMuted) { // Mute Volume
                    prevVolume = volume;
                    music.setVolume(0);
                    isMuted = true;
                }
                else { // UnMute Volume
                    music.setVolume(prevVolume);
                    isMuted = false;
                }
            }

            if (event.key.code == Keyboard::Escape) {
                menuSounds->play(1);
                currentState = STATE_MAIN;
            }
        }
    }
    else if (currentState == STATE_NAME_INPUT) {
        if (event.type == Event::TextEntered) {
            if (event.text.unicode == '\b' && !playerName.empty()) { // backspace functionality
                playerName.pop_back();
            }
            else if (event.text.unicode >= 32 && event.text.unicode <= 126) { // accept other characters between space and tilda
                playerName += static_cast<char>(event.text.unicode);
            }
        }

        if (event.type == Event::KeyPressed && event.key.code == Keyboard::Enter) {
            menuSounds->play(1);
            if (!playerName.empty()) {
                cout << "Starting game as: " << playerName << endl;
                gameStarted = true;
            }
        }

        if (event.key.code == Keyboard::Escape) {
            menuSounds->play(1);
            currentState = STATE_MAIN;
        }
    }
    else if (currentState == STATE_LEADERBOARD) {
        if (event.type == Event::KeyPressed && event.key.code == Keyboard::Escape) {
            menuSounds->play(1);
            currentState = STATE_MAIN;
            leaderboardRequested = false;
            cout << "Menu: Return to main from leaderboard" << endl;
        }
    }
    else if (currentState == STATE_SELECT_LEVEL) {
        cout << "Menu: Level menu selected" << endl;
        if (event.type == Event::KeyPressed) {
            if (event.key.code == Keyboard::Up) {
                menuSounds->play(0);
                selectedLevel = (selectedLevel - 1 + 4) % 4;
            }
            
            if (event.key.code == Keyboard::Down) {
                menuSounds->play(0);
                selectedLevel = (selectedLevel + 1) % 4;
            }

            if (event.key.code == Keyboard::Escape) {
                menuSounds->play(1);
                currentState = STATE_MAIN;
            }

            if (event.key.code == Keyboard::Enter) {
                menuSounds->play(1);
                cout << "Level " << selectedLevel + 1 << " selected" << endl; 
                currentState = STATE_NAME_INPUT;
                playerName.clear();
            }
        }
        
    }
    else if (currentState == STATE_EXIT) {
        window.close();
    }
    
}

void Menu::update() {
    if (isMuted) {
        volumeText.setString("Volume: Currently MUTED! (Press M to unmute)");
    } 
    else {
        volumeText.setString("Volume: " + to_string(volume) + " (+/-) \n\nUse Up/Down arrow key or Press M to mute");
    }
    nameText.setString(playerName);
}

void Menu::draw() {
    window.clear(Color::Black);
    window.draw(bgSprite);
    window.draw(title);

    if (currentState == STATE_MAIN) {
        for (int i = 0; i < menuItemCount; i++) {
            menuText[i].setFillColor(i == mainSelected ? Color::Yellow : Color:: White);
            window.draw(menuText[i]);
        }
    }
    else if (currentState == STATE_OPTIONS) {
        window.draw(volumeText);
    }
    else if (currentState == STATE_NAME_INPUT) {
        window.draw(namePrompt);
        window.draw(nameText);
    }
    else if (currentState == STATE_SELECT_LEVEL) {
        string levelOptions[4] = {"Level 1", "Level 2", "Level 3", "Boss Level"};
        for (int i = 0; i < 4; i++) {
            levelText[i].setString(levelOptions[i]);
            levelText[i].setPosition(100, 120 + i * 60);
            levelText[i].setFillColor(i == selectedLevel ? Color::Yellow : Color::White);
            window.draw(levelText[i]);
        }
    }
    
    //window.display();
}

string Menu::getPlayerName() const {
    return playerName;
}

bool Menu::isLeaderboardRequested() const {
    return leaderboardRequested;
}

void Menu::resetLeaderboardRequest() {
    leaderboardRequested = false;
}

bool Menu::hasGameStarted() const {
    return gameStarted;
}

void Menu::resetGameStarted() {
    gameStarted = false;
}

int Menu::getSelectedLevel() const { 
    return selectedLevel; 
}

int Menu::getVolume() const {
    return volume;
}

bool Menu::isMutedStatus() const {
    return isMuted;
}

bool Menu::getWantsToContinue() const {
    return wantsToContinue;
}

void Menu::resetWantsToContinue() {
    wantsToContinue = false;
}