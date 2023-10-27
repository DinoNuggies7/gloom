#ifndef DRAW_H
#define DRAW_H

#include "global.h"
#include "object.h"
#include "player.h"
#include "map.h"

void drawBackground(SDL_Renderer* renderer, SDL_DisplayMode display);
void drawWalls(SDL_Renderer* renderer, SDL_DisplayMode display, int objects, Object object[objects], Player player, Map map);
void drawObjects(SDL_Renderer* renderer, SDL_DisplayMode display, int objects, Object object[objects], Player player);
void drawHUD(SDL_Renderer* renderer, Player* player);

#endif