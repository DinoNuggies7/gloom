#include "object.h"

// Derk Object
void Derk_Object_INIT(Object* this, ...) {
	printf("<Gaylord has joined the game>\n");
	this->speed = 1.5;
	this->hitbox = 0.3;
	this->hp = this->maxHealth = 8;
	this->texture = IMG_Load("res/derk.png");
}

void Derk_Object_UPDATE(Object* this, ...) {
	va_list list;
	va_start(list, this);
	double dt = va_arg(list, double);
	Player* player = va_arg(list, Player*);
	Map map = va_arg(list, Map);
	SDL_Rect view = va_arg(list, SDL_Rect);

	// dir vector is straight line towards the player
	this->dir.x = player->pos.x - this->pos.x;
	this->dir.y = player->pos.y - this->pos.y;

	// Normalize dir vector
	this->dir = NormalizeVec2F(this->dir);
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
		if (tile[i].x > TILE_COLLISION_START && tile[i].x < TILE_COLLISION_END)
			this->vel.x = 0;
		if (tile[i].y > TILE_COLLISION_START && tile[i].y < TILE_COLLISION_END)
			this->vel.y = 0;
	}

	// Quit game when player is gotted (should be jumpscare later)
	if (this->pos.x + this->hitbox > player->pos.x - player->hitbox
	 && this->pos.y + this->hitbox > player->pos.y - player->hitbox
	 && this->pos.x - this->hitbox < player->pos.x + player->hitbox
	 && this->pos.y - this->hitbox < player->pos.y + player->hitbox) {
		printf("damn you actually gay asf\n");
		player->quit = true;
	}
}

void __attribute__((constructor)) SetRegistry() {
	ObjectVtableRegistry.INIT_[OBJECT_DERK] = &Derk_Object_INIT;
	ObjectVtableRegistry.UPDATE_[OBJECT_DERK] = &Derk_Object_UPDATE;
}