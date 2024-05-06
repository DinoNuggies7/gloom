#ifndef MAP_H
#define MAP_H

#include "vector.h"

#include <stdio.h>
#include <stdlib.h>

typedef struct Map {
	int w, h;
	int* tile;
	Vec3I* spawn; int spawns;
} Map;

enum MapTile {
	TILE_NONE,
	TILE_SPAWN,
	TILE_END,

	TILE_GLITCHED,
	TILE_BACKROOM,
};
#define TILE__COLLISION_PLAYER TILE_GLITCHED
#define TILE__COLLISION_OBJECT TILE_END

void parseMap(Map* map, const char* mapFile);
int getTile(Map map, int x, int y);
void setTile(Map* map, int x, int y, int tile);

#endif
