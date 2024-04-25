#include "object.h"

void Chair_Object_INIT(Object* this, ...) {
	this->solid = true;
	this->hitbox = 0.3;
	this->hp = this->maxHealth = 2;
	this->texture = IMG_Load("res/object/chair.png");
}

void Chair_Object_UPDATE(Object* this, ...) {

}

void __attribute__((constructor)) SetRegistry_Object_Chair() {
	ObjectVtableRegistry.INIT_[OBJECT_CHAIR] = &Chair_Object_INIT;
	ObjectVtableRegistry.UPDATE_[OBJECT_CHAIR] = &Chair_Object_UPDATE;
}
