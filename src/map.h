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
	NONE = -1,
	SPAWN,
	GLITCHED,
	RED,
	GREEN,
	BLUE,
	PURPLE,
};

void parseMap(Map* map, const char* mapFile);
int getTile(Map map, int x, int y);

#endif