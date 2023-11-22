#include "object.h"

void Chair_Object_INIT(Object* this, ...) {
	this->hitbox = 0.3;
	this->hp = this->maxHealth = 2;
	this->texture = IMG_Load("res/table.png");
}

void Chair_Object_UPDATE(Object* this, ...) {

}

void __attribute__((constructor)) InitChair() {
	objFunc.INIT_[OBJECT_CHAIR] = &Chair_Object_INIT;
	objFunc.UPDATE_[OBJECT_CHAIR] = &Chair_Object_UPDATE;
}