#include "item.h"
#include "player.h"

void Item__Pickaxe__INIT(Item* this, ...) {
	this->texture = IMG_Load("res/item/wield/1.png");
	this->itemTexture = IMG_Load("res/item/icon/1.png");
	this->frameRect.w = this->texture->w; this->frameRect.h = this->texture->h;
	// this->offset.y = 0.04; TEMPORARY MAYBE ASWELL
}

void Item__Pickaxe__USE(Item* this, ...) {
	va_list list;
	va_start(list, this);
	Player* player = va_arg(list, Player*);
	Map* map = va_arg(list, Map*);

	int x = player->pos.x + player->dir.x;
	int y = player->pos.y + player->dir.y;
	int tile = getTile(*map, x, y);
	if (tile >= TILE__COLLISION_PLAYER) {
		setTile(map, x, y, TILE_NONE);
	}
}

void __attribute__((constructor)) SetRegistry_Item_Pickaxe() {
	ItemVtableRegistry.INIT_[ITEM_PICKAXE] = &Item__Pickaxe__INIT;
	ItemVtableRegistry.USE_[ITEM_PICKAXE] = &Item__Pickaxe__USE;
}