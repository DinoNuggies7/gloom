#ifndef OBJECT_H
#define OBJECT_H

#include "player.h"

#include <SDL2/SDL_image.h>

typedef struct Object {
	void (*init)(struct Object*, ...);
	void (*update)(struct Object*, ...);
	bool destroy, solid, hurtable;
	int type, mode, damage, hp, maxHealth;
	float speed, hitbox, scale;
	Vec2F pos, vel, dir;
	Vec2I collisionBox[8];
	SDL_Surface* texture;
	SDL_FRect rect;
} Object;
enum ObjectType {
	OBJECT_NULL = -1,

	OBJECT_NONE,
	OBJECT_ITEM,
	OBJECT_DERK,
	OBJECT_CHAIR,

	OBJECT_TYPES,
};
extern struct ObjectVtableRegistry {
	void (*INIT_[OBJECT_TYPES])(Object* obj, ...);
	void (*UPDATE_[OBJECT_TYPES])(Object* obj, ...);
} ObjectVtableRegistry;

Object CreateObject(int type, ...);
void DestroyObject(Object* obj);

void ObjectGlobalINIT(Object* obj);
void ObjectGlobalUPDATE(Object* obj, float dt, Object* object, int objects, Player* player, Map map);

#endif