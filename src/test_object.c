#include "test_object.h"

// Test Object
void Object__Test__INIT(Object* this, ...) {
	printf("Based Nuggets\n");
	this->speed = 0;
	this->hitbox = 0.3f;
	this->texture = IMG_Load("res/image.png");
}

void Object__Test__UPDATE(Object* this, ...) {
	// if (this->pos.x < 0)
	// 	this->vel.x = this->speed;
	// else if (this->pos.x > 1166)
	// 	this->vel.x = -this->speed;
	// if (this->pos.y < 0)
	// 	this->vel.y = this->speed;
	// else if (this->pos.y > 568)
	// 	this->vel.y = -this->speed;
}