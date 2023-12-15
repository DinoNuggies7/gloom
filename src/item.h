#ifndef ITEM_H
#define ITEM_H

#include <stdbool.h>
#include <SDL2/SDL_image.h>

typedef struct Item {
	void (*init)(struct Item*, ...);
	void (*update)(struct Item*, ...);
	void (*use)(struct Item*, ...);
	bool isGun, isFiring;
	int type, ammo, rounds, damage, frames;
	float frame, range, fireTimer, reloadTimer, fireRate, reloadRate;
	SDL_Surface* texture;
	SDL_Surface* itemTexture;
	SDL_Rect frameRect;
	SDL_Rect* srcrect;
	SDL_FPoint offset;
} Item;
enum ItemType {
	ITEM_NONE,
	ITEM_PICKAXE,
	ITEM_NAGANT,

	ITEM_TYPES,
};
extern struct ItemVtableRegistry {
	void (*INIT_[ITEM_TYPES])(Item* item, ...);
	void (*USE_[ITEM_TYPES])(Item* item, ...);
} ItemVtableRegistry;

Item CreateItem(int type);

void ItemGlobalINIT(Item* item);
void ItemGlobalUPDATE(Item* item, float dt);
void ItemGlobalUSE(Item* item);

#endif