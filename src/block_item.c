#include "block_item.h"
#include "player.h"

void Item__Red__INIT(Item* this, ...) {
	this->tile = TILE_RED;
	this->offset.y = 0.15;
	this->texture = this->itemTexture = IMG_Load("res/block_red.png");
	this->frameRect.w = this->texture->w; this->frameRect.h = this->texture->h;
}

void Item__Green__INIT(Item* this, ...) {
	this->tile = TILE_GREEN;
	this->offset.y = 0.15;
	this->texture = this->itemTexture = IMG_Load("res/block_green.png");
	this->frameRect.w = this->texture->w; this->frameRect.h = this->texture->h;
}

void Item__Purple__INIT(Item* this, ...) {
	this->tile = TILE_PURPLE;
	this->texture = this->itemTexture = IMG_Load("res/block_purple.png");
	this->frameRect.w = this->texture->w; this->frameRect.h = this->texture->h;
	this->offset.y = 0.15;
}

void Item__Brick__INIT(Item* this, ...) {
	this->tile = TILE_BRICK;
	this->texture = this->itemTexture = IMG_Load("res/block_brick.png");
	this->frameRect.w = this->texture->w; this->frameRect.h = this->texture->h;
	this->offset.y = 0.15;
}

void Item__Stone__INIT(Item* this, ...) {
	this->tile = TILE_STONE;
	this->texture = this->itemTexture = IMG_Load("res/block_stone.png");
	this->frameRect.w = this->texture->w; this->frameRect.h = this->texture->h;
	this->offset.y = 0.15;
}

void Item__Dark__INIT(Item* this, ...) {
	this->tile = TILE_DARK;
	this->texture = this->itemTexture = IMG_Load("res/darkbrick.png");
	this->frameRect.w = this->texture->w; this->frameRect.h = this->texture->h;
	this->offset.y = 0.15;
}

void Item__Block__USE(Item* this, ...) {
	va_list list;
	va_start(list, this);
	Player* player = va_arg(list, Player*);
	Map* map = va_arg(list, Map*);

	int x = player->pos.x + player->dir.x * 1.5;
	int y = player->pos.y + player->dir.y * 1.5;

	int tile = getTile(*map, x, y);
	if (tile == TILE_NONE) {
		setTile(map, x, y, this->tile);
	}
}