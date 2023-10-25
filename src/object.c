#include "object.h"

// Function for Spawning Objects
Object CreateObject(const char* type) {
	Object object;
	if (!strcmp(type, "Test")) {
		object.init = &Object__Test__INIT;
		object.update = &Object__Test__UPDATE;
	}
	else if (!strcmp(type, "Derk")) {
		object.init = &Object__Derk__INIT;
		object.update = &Object__Derk__UPDATE;
	}
	return object;
}

// Global Object Init
void Object__INIT(Object* this) {
	printf("Spawned Object\n");
}

// Test Object
void Object__Test__INIT(Object* this, SDL_Renderer* renderer) {
	printf("Based Nuggets\n");
	this->type = "Test";
	this->speed = 30;
	this->texture = IMG_LoadTexture(renderer, "res/image.png");
	this->vel.x = this->speed;
	this->vel.y = this->speed;
	this->rect.w = 200;
	this->rect.h = 200;
}
void Object__Test__UPDATE(Object* this, ...) {
	if (this->pos.x < 0)
		this->vel.x = this->speed;
	else if (this->pos.x > 1166)
		this->vel.x = -this->speed;
	if (this->pos.y < 0)
		this->vel.y = this->speed;
	else if (this->pos.y > 568)
		this->vel.y = -this->speed;
}

// Derk Object
void Object__Derk__INIT(Object* this, SDL_Renderer* renderer) {
	printf("<Gaylord has joined the game>\n");
	this->type = "Derk";
	this->speed = 0.0f;
	this->texture = IMG_LoadTexture(renderer, "res/derkfull.png");
	if (this->texture == NULL)
		printf("no image\n");
}
void Object__Derk__UPDATE(Object* this, ...) {
	va_list list;
	va_start(list, this);
	double dt = va_arg(list, double);
	Player* player = va_arg(list, Player*);
	Map map = va_arg(list, Map);
	SDL_DisplayMode display = va_arg(list, SDL_DisplayMode);

	this->dir.x = player->pos.x - this->pos.x;
	this->dir.y = player->pos.y - this->pos.y;

	float magnitude = this->dir.x * this->dir.x + this->dir.y * this->dir.y;
	this->dir.x /= magnitude;
	this->dir.y /= magnitude;
	// if (magnitude != 0)  {
		this->vel.x = this->dir.x * this->speed;
		this->vel.y = this->dir.y * this->speed;
	// }

	Vec2I tile;
	tile.x = getTile(map, this->pos.x + this->vel.x * dt, this->pos.y);
	tile.y = getTile(map, this->pos.x, this->pos.y + this->vel.y * dt);
	if (tile.x > 0 && tile.x < 6)
		this->vel.x = 0;
	if (tile.y > 0 && tile.y < 6)
		this->vel.y = 0;

	// int shake = display.h / 16;
	if (this->pos.x > player->pos.x - player->hitbox
	 && this->pos.y > player->pos.y - player->hitbox
	 && this->pos.x < player->pos.x + player->hitbox
	 && this->pos.y < player->pos.y + player->hitbox) {
		// this->rect.w = display.w + shake;
		// this->rect.h = display.h + shake;
		printf("damn you actually gay asf\n");
		player->quit = true;
	}
	// if (this->rect.w > 2) {
	// 	this->rect.x = rand() % shake - shake;
	// 	this->rect.y = rand() % shake - shake;
	// }
}

// Global Object Update
void Object__UPDATE(Object* this, float dt) {
	this->pos.x += this->vel.x * dt;
	this->pos.y += this->vel.y * dt;
}

// Global Object Draw
void Object__DRAW(Object* this, SDL_Renderer* renderer) {
	SDL_Rect rect;
	rect.x = round(this->rect.x);
	rect.y = round(this->rect.y);
	rect.w = round(this->rect.w);
	rect.h = round(this->rect.h);
	SDL_RenderCopy(renderer, this->texture, NULL, &rect);
}