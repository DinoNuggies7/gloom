#include "object.h"

void Table_Object_INIT(Object* this, ...) {
	this->solid = true;
	this->hitbox = 0.3;
	this->speed = 2;
	this->hp = this->maxHealth = 2;
	this->texture = IMG_Load("res/object/table.png");
}

void Table_Object_UPDATE(Object* this, ...) {
	va_list list;
	va_start(list, this);
	double dt = va_arg(list, double);
	SDL_Rect view = va_arg(list, SDL_Rect);
	Player* player = va_arg(list, Player*);
	Map map = va_arg(list, Map);

	static int dir;
	static float moveDistance;
	if (moveDistance <= 0 || getTile(map, this->pos.x + this->dir.x, this->pos.y + this->dir.y) >= TILE__COLLISION_OBJECT) {
		moveDistance = rand() % 6 + 2;
		dir = rand() % 4 + 1;
	}
	moveDistance -= 1 * dt;

	switch (dir) {
		case 1:
			this->dir.x = 0;
			this->dir.y = -0.5;
			break;
		case 2:
			this->dir.x = 0.5;
			this->dir.y = 0;
			break;
		case 3:
			this->dir.x = 0;
			this->dir.y = 0.5;
			break;
		case 4:
			this->dir.x = -0.5;
			this->dir.y = 0;
			break;
	}

	this->vel.x = this->dir.x * this->speed;
	this->vel.y = this->dir.y * this->speed;
}

void __attribute__((constructor)) SetRegistry_Object_Table() {
	ObjectVtableRegistry.INIT_[OBJECT_TABLE] = &Table_Object_INIT;
	ObjectVtableRegistry.UPDATE_[OBJECT_TABLE] = &Table_Object_UPDATE;
}
