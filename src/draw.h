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
	TEXTURE_DOOR,
	TEXTURE_CARPET,
	TEXTURE_TILED,
	TEXTURE_YAY,
	TEXTURE_END,
	TEXTURE_DERK,

	TEXTURES
};
#define TEXTURE__WALL 		TEXTURE_BACKROOM
#define TEXTURE__FLOOR 		TEXTURE_CARPET
#define TEXTURE__CEILING 	TEXTURE_TILED
#define TEXTURE__JUMPSCARE 	TEXTURE_YAY

void loadTextures(SDL_Surface** texture);
void drawBackground(SDL_Surface* screen, SDL_Surface** texture, SDL_Rect view, Player player);
void drawForeground(SDL_Surface* screen, SDL_Surface** texture, SDL_Rect view, Player player, Object* object, int objects, Map map);
void drawHUD(SDL_Surface* screen, SDL_Surface** texture, SDL_Renderer* renderer, SDL_Rect windowSize, SDL_Rect view, Player player, float dt);

#endif
