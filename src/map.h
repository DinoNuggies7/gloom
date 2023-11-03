#ifndef MAP_H
#define MAP_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

typedef struct {
	int w, h;
	int* tile;
} Map;

enum MapTile {
	TILE_NONE,
	TILE_SPAWN,

	TILE_COLLISION_START,
	TILE_GLITCHED,
	TILE_RED,
	TILE_GREEN,
	TILE_PURPLE,
	TILE_BRICK,
	TILE_COLLISION_END,
};

void parseMap(Map* map, const char* mapFile);
int getTile(Map map, int x, int y);
void setTile(Map* map, int x, int y, int tile);

#endif