#include "object.h"

// Derk Object
void Derk_Object_INIT(Object* this, ...) {
	printf("<Gaylord has joined the game>\n");
	this->mode = 2;
	this->damage = 1;
	this->hurtable = true;
	this->speed = 2.5;
	this->hitbox = 0.3;
	this->hp = this->maxHealth = 8;
	this->texture = IMG_Load("res/object/derk.png");
}

void Derk_Object_UPDATE(Object* this, ...) {
	va_list list;
	va_start(list, this);
	double dt = va_arg(list, double);
	SDL_Rect view = va_arg(list, SDL_Rect);
	Player* player = va_arg(list, Player*);
	Map map = va_arg(list, Map);

	// Move at a constant speed toward the player
	Vec2F dir;
	dir.x = player->pos.x - this->pos.x; 
	dir.y = player->pos.y - this->pos.y;
	dir = NormalizeVec2F(dir);
	this->dir.x = dir.x;
	this->dir.y = dir.y;

	this->vel.x = this->dir.x * this->speed;
	this->vel.y = this->dir.y * this->speed;

	// Wall Collision
	Vec2I tile[4];
	tile[0].x = getTile(map, (this->pos.x - this->hitbox) + this->vel.x * dt, this->pos.y - this->hitbox);
	tile[1].x = getTile(map, (this->pos.x - this->hitbox) + this->vel.x * dt, this->pos.y + this->hitbox);
	tile[2].x = getTile(map, (this->pos.x + this->hitbox) + this->vel.x * dt, this->pos.y - this->hitbox);
	tile[3].x = getTile(map, (this->pos.x + this->hitbox) + this->vel.x * dt, this->pos.y + this->hitbox);
	tile[0].y = getTile(map, this->pos.x - this->hitbox, (this->pos.y - this->hitbox) + this->vel.y * dt);
	tile[1].y = getTile(map, this->pos.x - this->hitbox, (this->pos.y + this->hitbox) + this->vel.y * dt);
	tile[2].y = getTile(map, this->pos.x + this->hitbox, (this->pos.y - this->hitbox) + this->vel.y * dt);
	tile[3].y = getTile(map, this->pos.x + this->hitbox, (this->pos.y + this->hitbox) + this->vel.y * dt);
	for (int i = 0; i < 4; i++) {
		if (tile[i].x >= TILE__COLLISION_OBJECT)
			this->vel.x = 0;
		if (tile[i].y >= TILE__COLLISION_OBJECT)
			this->vel.y = 0;
	}

	// Jumpscare player when they be gotted
	if (this->pos.x + this->hitbox > player->pos.x - player->hitbox
	 && this->pos.y + this->hitbox > player->pos.y - player->hitbox
	 && this->pos.x - this->hitbox < player->pos.x + player->hitbox
	 && this->pos.y - this->hitbox < player->pos.y + player->hitbox) {
		player->jumpscare = this->type;
	}
}

void __attribute__((constructor)) SetRegistry_Object_Derk() {
	ObjectVtableRegistry.INIT_[OBJECT_DERK] = &Derk_Object_INIT;
	ObjectVtableRegistry.UPDATE_[OBJECT_DERK] = &Derk_Object_UPDATE;
}