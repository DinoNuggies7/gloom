#ifndef DRAW_H
#define DRAW_H

#define WIELD_SCALE 2
#define SHADE_STRENGTH 20

#include "vector.h"
#include "object.h"
#include "player.h"
#include "map.h"

enum TextureID {
	TEXTURE_BACKROOM,
	TEXTURE_CARPET,
	TEXTURE_CEILING,

	TEXTURES,
};

void drawBackground(SDL_Surface* screen, SDL_Surface** texture, SDL_Rect view, Player player);
void drawForeground(SDL_Surface* screen, SDL_Surface** texture, SDL_Rect view, Player player, Object* object, int objects, Map map);
void drawHUD(SDL_Surface* screen, SDL_Rect view, Player player);

#endif