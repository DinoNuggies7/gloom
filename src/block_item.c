#include "block_item.h"
#include "player.h"

void Item__Red__INIT(Item* this, ...) {
	this->tile = TILE_RED;
	this->texture = this->itemTexture = IMG_Load("res/block_red.png");
}

void Item__Green__INIT(Item* this, ...) {
	this->tile = TILE_GREEN;
	this->texture = this->itemTexture = IMG_Load("res/block_green.png");
}

void Item__Purple__INIT(Item* this, ...) {
	this->tile = TILE_PURPLE;
	this->texture = this->itemTexture = IMG_Load("res/block_purple.png");
}

void Item__Brick__INIT(Item* this, ...) {
	this->tile = TILE_BRICK;
	this->texture = this->itemTexture = IMG_Load("res/block_brick.png");
}

void Item__Stone__INIT(Item* this, ...) {
	this->tile = TILE_STONE;
	this->texture = this->itemTexture = IMG_Load("res/block_stone.png");
}

void Item__Dark__INIT(Item* this, ...) {
	this->tile = TILE_DARK;
	this->texture = this->itemTexture = IMG_Load("res/darkbrick.png");
}

void Item__Block__USE(Item* this, ...) {
	va_list list;
	va_start(list, this);
	Player* player = va_arg(list, Player*);
	Map* map = va_arg(list, Map*);

	int x = player->pos.x + player->dir.x;
	int y = player->pos.y + player->dir.y;
	int tile = getTile(*map, x, y);
	if (tile == TILE_NONE) {
		setTile(map, x, y, this->tile);
	}
}