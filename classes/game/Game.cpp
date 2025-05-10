#include "Game.h"

Game::Game(int screen_x, int screen_y) : screen_x(screen_x), screen_y(screen_y), window(VideoMode(screen_x, screen_y), "Sonic the Hedgehog-OOP", Style::Close) {
    window.setFramerateLimit(60);
    window.setVerticalSyncEnabled(true);

    if (!music.openFromFile("Data/labrynth.ogg")) {
        cout << "Failed to load background music" << endl;
    }
    else {
        music.setVolume(40);
        music.play();
        music.setLoop(true);
    }

    // Initialize menu, scoreboard
    menu = new Menu(window, music, screen_x, screen_y);
    scoreboard = new Scoreboard(screen_x, screen_y);
    level = nullptr;

    currentState = GAME_STATE_MENU;

    cameraOffsetX = 0.0f;
}

Game::~Game() {
    if (menu != nullptr) { delete menu; menu = nullptr;}
    if (scoreboard != nullptr) { delete scoreboard; scoreboard = nullptr;}
    if (level != nullptr) { delete level; level = nullptr;}
}

void Game::runGame() {
    Clock clock;
    float deltaTime = 0.0f;
    bool levelCreated = false;
    
    while (window.isOpen()) {    
        deltaTime = clock.restart().asSeconds();

        Event event;
		while (window.pollEvent(event)) {
			if (event.type == Event::Closed) {
                if (currentState == GAME_STATE_PLAYING && !menu->getPlayerName().empty()) {
                    scoreboard->saveScores(menu->getPlayerName(), 9999999);
                }
				window.close();
			}

            if (currentState == GAME_STATE_MENU) {
                menu->handleInput(event);
                if (menu->isLeaderboardRequested()) {
                    currentState = GAME_STATE_LEADERBOARD;
                    menu->resetLeaderboardRequest();
                    cout << "Game: Switched to GAME_STATE_LEADERBOARD" << endl;
                }
            }
            else if (currentState == GAME_STATE_PLAYING) {
                if (event.type == Event::KeyPressed && event.key.code == Keyboard::Escape) {
                    if (level) {
                        delete level; // level destructor stops level music
                        level = nullptr;
                    }
                
                    if (!menu->isMutedStatus()) {
                        music.setVolume(menu->getVolume());
                    } else {
                        music.setVolume(0);
                    }
                
                    music.play(); 

                    currentState = GAME_STATE_MENU;
                    menu->handleInput(event);
                }
            }
            else if (currentState == GAME_STATE_LEADERBOARD) {
                if (event.type == Event::KeyPressed && event.key.code == Keyboard::Escape) {
                    currentState = GAME_STATE_MENU;
                    menu->handleInput(event);
                }
            }
		}


        if (currentState == GAME_STATE_MENU){
            menu->update();
            if (menu->hasGameStarted()) {
                music.stop();
                string playerName = menu->getPlayerName();
                // to-do
                if (level != nullptr) {
                    delete level;
                    level = nullptr;
                }

                int selectedLevel = menu->getSelectedLevel();

                int menuVolume = menu->getVolume();
                bool isMuted = menu->isMutedStatus();

                if (selectedLevel == 0) {
                    level = new Level1_Labyrinth();
                }
                else if (selectedLevel == 1) {
                    level = new Level2_IceCap();
                }
                else if (selectedLevel == 2) {
                    level = new Level3_DeathEgg();
                }
                else if (selectedLevel == 3) {
                    level = new BossLevel();
                }
                levelCreated = true;

                level->loadAssets(isMuted ? 0 : menuVolume); // load all assests when level is created
                
                currentState = GAME_STATE_PLAYING;
                menu->resetGameStarted();
                cout << "Game: Switched to GAME_STATE_PLAYING for " << playerName << endl;
            }
        }
        else if (currentState == GAME_STATE_PLAYING) {
            if (level != nullptr) {
                level->update(deltaTime);

                if (level->isLevelComplete()) {
                    cout << "Level completed!" << endl;

                    currentState = GAME_STATE_MENU; // go back to menu for now

                    menu->resetGameStarted();
                }
            }
        }
        else if (currentState == GAME_STATE_LEADERBOARD) {
            menu->update();
        }
		

        // Rendering
		window.clear(Color::Black);
        
        if (currentState == GAME_STATE_MENU) {
            menu->draw();
        }
        else if (currentState == GAME_STATE_PLAYING) {
            if (level != nullptr) {
                
                // logic to calculate cameraOffsetX for horizontal scrolling
                float playerCenterX = level->getPlayerX() + level->getPlayerWidth() / 2.0f;
                float halfScreen = screen_x / 2.0f;
                float levelWidthinPixels = level->getLevelWidthinTiles() * level->getCellSize();

                if (playerCenterX <= halfScreen) {
                    cameraOffsetX = 0; // lock camera to the left edge
                }
                else if (playerCenterX >= levelWidthinPixels - halfScreen) {
                    cameraOffsetX = levelWidthinPixels - screen_x; // lock camera to the right edge
                }
                else {
                    cameraOffsetX = playerCenterX - halfScreen; // follow and center the player
                }
                level->render(window, cameraOffsetX);
                level->drawUI(window, cameraOffsetX);
            }
        }
        else if (currentState == GAME_STATE_LEADERBOARD) {
            scoreboard->draw(window);
        }

        window.display();
    }
}