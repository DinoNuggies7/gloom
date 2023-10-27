#ifndef OBJECT_H
#define OBJECT_H

#include "player.h"

#include <SDL2/SDL_image.h>

typedef struct Object {
	void (*init)(struct Object*, ...);
	void (*update)(struct Object*, ...);
	int type;
	float speed, hitbox;
	Vec2F pos, vel, dir;
	SDL_Surface* texture;
	SDL_FRect rect;
} Object;
enum ObjectType {
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

void ObjectGlobalINIT(Object* object);
void ObjectGlobalUPDATE(Object* object, float dt);

#endif