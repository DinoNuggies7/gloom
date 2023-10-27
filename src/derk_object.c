#include "derk_object.h"

// Derk Object
void Object__Derk__INIT(Object* this, ...) {
	printf("<Gaylord has joined the game>\n");
	this->speed = 0.f;
	this->hitbox = 0.3f;
	this->texture = IMG_Load("res/derk.png");
}

void Object__Derk__UPDATE(Object* this, ...) {
	va_list list;
	va_start(list, this);
	double dt = va_arg(list, double);
	Player* player = va_arg(list, Player*);
	Map map = va_arg(list, Map);
	SDL_DisplayMode display = va_arg(list, SDL_DisplayMode);

	// dir vector is straight line towards the player
	this->dir.x = player->pos.x - this->pos.x;
	this->dir.y = player->pos.y - this->pos.y;

	// Normalize dir vector
	float magnitude = this->dir.x * this->dir.x + this->dir.y * this->dir.y;
	this->dir.x /= magnitude;
	this->dir.y /= magnitude;
	if (magnitude != 0)  {
		this->vel.x = this->dir.x * this->speed;
		this->vel.y = this->dir.y * this->speed;
	}

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