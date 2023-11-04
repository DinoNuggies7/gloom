#include "object.h"
#include "objects.h"

void SetObjectFunctions(ObjectFunction* objFunc) {
	objFunc->INIT_[OBJECT_DERK] = &Object__Derk__INIT;
	objFunc->UPDATE_[OBJECT_DERK] = &Object__Derk__UPDATE;
	objFunc->INIT_[OBJECT_CHAIR] = &Object__Chair__INIT;
	objFunc->UPDATE_[OBJECT_CHAIR] = &Object__Chair__UPDATE;
}

// Function for Spawning Objects
Object CreateObject(int type, ObjectFunction* objFunc) {
	Object object;
	ObjectGlobalINIT(&object);
	if (type > OBJECT_NONE && type < OBJECT_TYPES) {
		object.init = objFunc->INIT_[type];
		object.update = objFunc->UPDATE_[type];
		object.type = type;
		object.init(&object);
	}
	return object;
}

// Global Object Init
void ObjectGlobalINIT(Object* this) {
	printf("Spawned Object\n");
	this->destroy = false;
	this->type = OBJECT_NONE;
	this->hitbox = 0.5f;
	this->speed = 0;
	this->pos.x = this->pos.y = 0;
	this->vel.x = this->vel.y = 0;
	this->dir.x = this->dir.y = 0;
	this->texture = IMG_Load("res/null.png");
}

// Global Object Update
void ObjectGlobalUPDATE(Object* this, float dt) {
	this->pos.x += this->vel.x * dt;
	this->pos.y += this->vel.y * dt;
}