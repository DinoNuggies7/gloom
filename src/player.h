#ifndef PLAYER_H
#define PLAYER_H

#include "global.h"
#include "map.h"

#include <stdbool.h>
#include <math.h>

typedef struct {
	bool quit;
	bool forward, backword, left, right, dig, lookleft, lookright;
	float speed, friction, maxSpeed, minSpeed, hitbox, xrel, lookspeed;
	Vec2F pos, vel, dir, plane;
} Player;

void Player__INIT(Player* player);
void Player__UPDATE(Player* player, Map map, float dt);

#endif