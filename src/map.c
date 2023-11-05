#include "map.h"

void parseMap(Map* map, const char* mapFile) {
	FILE* file = fopen(mapFile, "r");
	char ch;
	char buffer[2];
	int i = 0;
	bool done;
	while (!done) { // Find the size of the map
		ch = fgetc(file);
		switch (ch) {
		case ',':
			map->w = atoi(buffer);
			buffer[0] = '\0';
			i = 0;
			break;
		case '\n':
			map->h = atoi(buffer);
			buffer[0] = '\0';
			i = 0;
			done = true;
			break;
		default:
			buffer[i] = ch;
			i++;
			break;
		}
	}
	map->tile = malloc(sizeof(int) * (map->w * map->h));
	while (i < map->w * map->h) { // Load the map data
		ch = fgetc(file);
		switch (ch) {
		case '.':
			map->tile[i] = TILE_NONE;
			i++;
			break;
		case '@':
			map->tile[i] = TILE_SPAWN;
			i++;
			break;
		case '?':
			map->tile[i] = TILE_GLITCHED;
			i++;
			break;
		case 'R':
			map->tile[i] = TILE_RED;
			i++;
			break;
		case 'G':
			map->tile[i] = TILE_GREEN;
			i++;
			break;
		case 'P':
			map->tile[i] = TILE_PURPLE;
			i++;
			break;
		case 'B':
			map->tile[i] = TILE_BRICK;
			i++;
			break;
		case 'S':
			map->tile[i] = TILE_STONE;
			i++;
			break;
		case 'D':
			map->tile[i] = TILE_DARK;
			i++;
			break;
		case '\n':
			break;
		}
	}
}

int getTile(Map map, int x, int y) {
	return map.tile[x+(y*map.w)];
}

void setTile(Map* map, int x, int y, int tile) {
	map->tile[x+(y*map->w)] = tile;
}