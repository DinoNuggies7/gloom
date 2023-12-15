#ifndef DRAW_H
#define DRAW_H

#define WIELD_SCALE 2
#define SHADE_STRENGTH 16

#include "vector.h"
#include "object.h"
#include "player.h"
#include "map.h"

enum TextureID {
	TEXTURE_BACKROOM,

	TEXTURES,
};

void drawBackground(SDL_Renderer* renderer, SDL_Surface** texture, SDL_Rect view, Player player);
void drawForeground(SDL_Renderer* renderer, SDL_Surface** texture, SDL_Rect view, int objects, Object object[objects], Player player, Map map);
void drawHUD(SDL_Renderer* renderer, SDL_Surface** texture, SDL_Rect view, Player player);

#endif