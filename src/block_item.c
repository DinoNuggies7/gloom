#include "item.h"
#include "player.h"

void Item__Block__INIT(Item* this, ...) {
	// The tile to be placed
	this->damage = this->type + (TILE_RED - ITEM_RED);

	// Getting the right item texture
	char texturePath[15] = "res/block";
	char textureNum[2];
	sprintf(textureNum, "%d", this->type - 3);
	strncat(texturePath, textureNum, 2);
	strcat(texturePath, ".png");

	this->texture = this->itemTexture = IMG_Load(texturePath);
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
		setTile(map, x, y, this->damage);
	}
}

void __attribute__((constructor)) SetRegistry() {
	for (int i = ITEM_RED; i < ITEM_TYPES; i++) {
		ItemVtableRegistry.INIT_[i] = &Item__Block__INIT;
		ItemVtableRegistry.USE_[i] = &Item__Block__USE;
	}
}