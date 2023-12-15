#ifndef MAP_H
#define MAP_H

#define MAX_OBJECTS 16

#include "vector.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

typedef struct {
	int w, h;
	int* tile;
	Vec3I spawn[MAX_OBJECTS]; int spawns;
} Map;

enum MapTile {
	TILE_NONE,
	TILE_SPAWN,

	TILE_COLLISION_START,
	TILE_GLITCHED,
	TILE_BACKROOM,
	// TILE_GREEN,
	// TILE_PURPLE,
	// TILE_BRICK,
	// TILE_STONE,
	// TILE_DARK,
	TILE_COLLISION_END,
};

void parseMap(Map* map, const char* mapFile);
int getTile(Map map, int x, int y);
void setTile(Map* map, int x, int y, int tile);

#endif