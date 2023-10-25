#ifndef OBJECT_H
#define OBJECT_H

#include "player.h"

#include <SDL2/SDL_image.h>

typedef struct Object {
	void (*init)(struct Object*, SDL_Renderer*);
	void (*update)(struct Object*, ...);
	const char* type;
	float speed;
	Vec2F pos, vel, dir;
	SDL_Texture* texture;
	SDL_FRect rect;
} Object;
void Object__INIT(Object* object);
void Object__UPDATE(Object* object, float dt);
void Object__DRAW(Object* object, SDL_Renderer* renderer);
Object CreateObject(const char* type);

void Object__Test__INIT(Object* object, SDL_Renderer* renderer);
void Object__Test__UPDATE(Object* object, ...);

void Object__Derk__INIT(Object* object, SDL_Renderer* renderer);
void Object__Derk__UPDATE(Object* object, ...);

#endif