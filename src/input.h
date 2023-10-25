#ifndef INPUT_H
#define INPUT_H

#include "player.h"

#include <SDL2/SDL_events.h>

void handleInput(SDL_Event* event, Player* player);

#endif