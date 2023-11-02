#ifndef PLAYER_H
#define PLAYER_H

#include "global.h"
#include "map.h"

#include <stdbool.h>
#include <string.h>
#include <math.h>
#include <cjson/cJSON.h>

typedef struct {
	bool quit;
	char* map;
	bool forward, backword, left, right, lookleft, lookright, dig, build;
	float speed, friction, maxSpeed, minSpeed, hitbox, xrel, lookspeed, sensitivity;
	int equip;
	Vec2F pos, vel, dir, plane;
} Player;

void Player__INIT(Player* player, const char* configPath);
void Player__UPDATE(Player* player, Map* map, float dt);

#endif