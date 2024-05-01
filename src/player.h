#ifndef PLAYER_H
#define PLAYER_H

#include "cJSON.h"
#include "vector.h"
#include "item.h"
#include "map.h"

#include <string.h>
#include <math.h>

#define CONFIG_PATH "settings.json"
#define LEFT  0
#define RIGHT 1
#define SLOTS 8

typedef struct Player {
	bool quit;
	char* map;
	int equip[2], select, scroll, jumpscare;
	Item inventory[SLOTS];
	bool sprint, tired, choosing, finish;
	bool forward, backword, left, right, lookup, lookdown, lookleft, lookright, leftclick, rightclick;
	float walkSpeed, runSpeed, stamina, maxStamina, hitbox, xrel, lookspeed, sensitivity, selectTimer;
	Vec2F pos, vel, dir, plane;
	Vec2F collisionBox[8]; Vec2B collided;
	SDL_Surface* hudbarTexture;
	SDL_Surface* selectTexture;
} Player;

void Player__INIT(Player* player);
void Player__UPDATE(Player* player, Map* map, float dt);

void save(Player* player);
void load(Player* player, FILE* file);

#endif
