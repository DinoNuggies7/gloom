#include "pickaxe_item.h"
#include "player.h"

void Item__Pickaxe__INIT(Item* this, ...) {
	this->texture = IMG_Load("res/pickaxe.png");
}

void Item__Pickaxe__USE(Item* this, ...) {
	va_list list;
	va_start(list, this);
	Player* player = va_arg(list, Player*);
	Map* map = va_arg(list, Map*);

	int x = player->pos.x + player->dir.x;
	int y = player->pos.y + player->dir.y;
	int tile = getTile(*map, x, y);
	if (tile > TILE_COLLISION_START && tile < TILE_COLLISION_END) {
		setTile(map, x, y, TILE_NONE);
	}
}