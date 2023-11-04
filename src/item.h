#ifndef ITEM_H
#define ITEM_H

#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>

typedef struct Item {
	void (*init)(struct Item*, ...);
	void (*update)(struct Item*, ...);
	void (*use)(struct Item*, ...);
	int type, tile, ammo, rounds, damage;
	float range, fireTimer, reloadTimer, fireRate, reloadRate;
	SDL_Surface* texture;
} Item;
enum ItemType {
	ITEM_NONE,
	ITEM_PICKAXE,
	ITEM_NAGANT,
	ITEM_RED,
	ITEM_GREEN,
	ITEM_PURPLE,
	ITEM_BRICK,

	ITEM_TYPES,
};
typedef struct ItemFunction {
	void (*INIT_[ITEM_TYPES])(Item* item, ...);
	void (*USE_[ITEM_TYPES])(Item* item, ...);
} ItemFunction;

void SetItemFunctions(ItemFunction* itemFunc);
Item CreateItem(int type, ItemFunction* itemFunc);

void ItemGlobalINIT(Item* item);
void ItemGlobalUPDATE(Item* item, float dt);
void ItemGlobalUSE(Item* item);

#endif