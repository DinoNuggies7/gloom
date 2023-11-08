#ifndef OBJECT_H
#define OBJECT_H

#include "player.h"

#include <SDL2/SDL_image.h>

typedef struct Object {
	void (*init)(struct Object*, ...);
	void (*update)(struct Object*, ...);
	bool destroy;
	int type, hp, maxHealth;
	float speed, hitbox;
	Vec2F pos, vel, dir;
	SDL_Surface* texture;
	SDL_FRect rect;
} Object;
enum ObjectType {
	OBJECT_NULL = -1,

	OBJECT_NONE,
	OBJECT_DERK,
	OBJECT_CHAIR,

	OBJECT_TYPES,
};
typedef struct ObjectFunction {
void (*INIT_[OBJECT_TYPES])(Object* object, ...);
void (*UPDATE_[OBJECT_TYPES])(Object* object, ...);
} ObjectFunction;

void SetObjectFunctions(ObjectFunction* objFunc);
Object CreateObject(int type, ObjectFunction* objFunc);
void DestroyObject(Object* object);

void ObjectGlobalINIT(Object* object);
void ObjectGlobalUPDATE(Object* object, Player player, Map map, float dt);

#endif