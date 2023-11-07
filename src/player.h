#ifndef PLAYER_H
#define PLAYER_H

#include "global.h"
#include "item.h"
#include "map.h"

#include <stdbool.h>
#include <string.h>
#include <math.h>
#include <cjson/cJSON.h>

#define CONFIG_PATH "config.json"
#define LEFT  0
#define RIGHT 1
#define SLOTS 8

typedef struct {
	bool quit;
	char* map;
	int equip[2], select;
	Item inventory[SLOTS];
	int scroll;
	bool forward, backword, left, right, lookup, lookdown, lookleft, lookright, leftclick, rightclick, choosing;
	float speed, friction, maxSpeed, minSpeed, hitbox, xrel, lookspeed, sensitivity, selectTimer;
	Vec2F pos, vel, dir, plane;
	SDL_Surface* selectTexture;
} Player;

void save(Player* player);

void Player__INIT(Player* player, ItemFunction* itemFunc);
void Player__UPDATE(Player* player, Map* map, float dt);

#endif