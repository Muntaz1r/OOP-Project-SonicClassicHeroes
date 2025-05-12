#include <iostream>
#include <fstream>
#include <cmath>
#include <ctime>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Window.hpp>

#include "game/Game.h"

using namespace sf;
using namespace std;

int screen_x = 1200;
int screen_y = 900;

int main() {
	srand(time(0));

	Game game(screen_x, screen_y);
	game.runGame();

	return 0;
}
