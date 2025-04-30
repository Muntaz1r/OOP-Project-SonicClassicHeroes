#include "Menu.h"

Menu::Menu(RenderWindow& window, Music& music) : window(window), music(music), currentState(STATE_MAIN), volume(40), mainSelected(0) {
    font.loadFromFile();
}
