#include "chair_object.h"

void Object__Chair__INIT(Object* this, ...) {
	this->hitbox = 0.3;
	this->texture = IMG_Load("res/table.png");
}

void Object__Chair__UPDATE(Object* this, ...) {

}