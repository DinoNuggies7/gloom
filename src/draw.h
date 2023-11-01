#ifndef DRAW_H
#define DRAW_H

#include "global.h"
#include "object.h"
#include "player.h"
#include "map.h"

void drawBackground(SDL_Renderer* renderer, SDL_Rect view);
void drawObjects(SDL_Renderer* renderer, SDL_Rect view, int objects, Object object[objects], Player player);
void drawWalls(SDL_Renderer* renderer, SDL_Rect view, int objects, Object object[objects], Player player, Map map);
void drawHUD(SDL_Renderer* renderer, Player* player);

#endif