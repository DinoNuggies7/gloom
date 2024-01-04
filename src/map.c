#include "map.h"
#include "object.h"

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
	map->spawns = 0;
	int spawns = 0;
	done = false;
	FILE* tmpFile = fopen(mapFile, "r");
	while (!done) { // Find total objects and allocate memory for them
		int cht = fgetc(tmpFile);
		if (cht == '@' || cht == '$')
			spawns++;
		if (feof(tmpFile))
			done = true;
	}
	fclose(tmpFile);
	printf("found objects: %d\n", spawns);
	map->spawn = malloc(sizeof(Vec3I) * spawns);
	while (i < map->w * map->h) { // Load the map data
		ch = fgetc(file);
		switch (ch) {
			case '.':
				map->tile[i] = TILE_NONE;
				i++;
				break;
			case '&':
				map->tile[i] = TILE_SPAWN;
				i++;
				break;
			case '%':
				map->tile[i] = TILE_DOOR;
				i++;
				break;
			case '@':
				map->tile[i] = TILE_NONE;
				char typeChar = fgetc(file);
				map->spawn[map->spawns].x = i % map->w;
				map->spawn[map->spawns].y = i / map->w;
				switch (typeChar) {
					case ' ':
						map->spawn[map->spawns].z = OBJECT_NONE;
						break;
					case 'd':
						map->spawn[map->spawns].z = OBJECT_DERK;
						break;
					case 'c':
						map->spawn[map->spawns].z = OBJECT_CHAIR;
						break;
				}
				map->spawns++;
				i++;
				break;
			case '$':
				map->tile[i] = TILE_NONE;
				char itemChar = fgetc(file);
				map->spawn[map->spawns].x = i % map->w;
				map->spawn[map->spawns].y = i / map->w;
				switch (itemChar) {
					case ' ':
						map->spawn[map->spawns].z = -ITEM_NONE;
						break;
					case 'p':
						map->spawn[map->spawns].z = -ITEM_PICKAXE;
						break;
					case 'n':
						map->spawn[map->spawns].z = -ITEM_NAGANT;
						break;
				}
				map->spawns++;
				i++;
				break;
			case '0':
				map->tile[i] = TILE_GLITCHED;
				i++;
				break;
			case '1':
				map->tile[i] = TILE_BACKROOM;
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