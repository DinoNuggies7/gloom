#include "item.h"
#include "items.h"

void SetItemFunctions(ItemFunction* itemFunc) {
	// Pickaxe
	itemFunc->INIT_[ITEM_PICKAXE] = &Item__Pickaxe__INIT;
	itemFunc->USE_[ITEM_PICKAXE] = &Item__Pickaxe__USE;

	// Nagant
	itemFunc->INIT_[ITEM_NAGANT] = &Item__Nagant__INIT;
	itemFunc->USE_[ITEM_NAGANT] = &Item__Nagant__USE;

	// Blocks
	itemFunc->INIT_[ITEM_RED] = &Item__Red__INIT;
	itemFunc->INIT_[ITEM_GREEN] = &Item__Green__INIT;
	itemFunc->INIT_[ITEM_PURPLE] = &Item__Purple__INIT;
	itemFunc->INIT_[ITEM_BRICK] = &Item__Brick__INIT;
	itemFunc->INIT_[ITEM_STONE] = &Item__Stone__INIT;
	itemFunc->INIT_[ITEM_DARK] = &Item__Dark__INIT;
	for (int i = ITEM_RED; i < ITEM_TYPES; i++)
		itemFunc->USE_[i] = &Item__Block__USE;
}

// Function for Spawning Items
Item CreateItem(int type, ItemFunction* itemFunc) {
	Item item;
	ItemGlobalINIT(&item);
	if (type > ITEM_NONE && type < ITEM_TYPES) {
		item.init = itemFunc->INIT_[type];
		item.use = itemFunc->USE_[type];
		item.type = type;
		item.init(&item);
	}
	return item;
}

// Global Item Init
void ItemGlobalINIT(Item* this) {
	printf("Created Item\n");
	this->type = ITEM_NONE;
	this->isGun = this->isFiring = false;
	this->rot = this->drot = 0;
	this->range = this->tile = 0;
	this->ammo = this->rounds = 0;
	this->fireTimer = this->reloadTimer = 0;
	this->fireRate = this->reloadRate = 0;
	this->rotPoint.x = this->rotPoint.y = 0;
	this->firePoint.x = this->firePoint.y = 0;
	this->texture = this->itemTexture = IMG_Load("res/null.png");
}

// Global Item Update
void ItemGlobalUPDATE(Item* this, float dt) {
	if (this->fireTimer > 0) this->fireTimer -= 1 * dt;
	else this->fireTimer = 0;

	if (this->reloadTimer > 0) this->reloadTimer -= 1 * dt;
	else this->reloadTimer = 0;

	if (this->ammo <= 0 && this->reloadTimer <= 0) {
		this->reloadTimer = this->reloadRate;
		this->ammo = this->rounds;
	}

	if (this->fireTimer > 0 || this->reloadTimer > 0 || this->ammo <= 0)
		this->isFiring = false;

	this->rot = this->fireTimer * 100 + this->drot;
}

// Global Item Use
void ItemGlobalUSE(Item* this) {
	if (this->fireTimer <= 0 && this->reloadTimer <= 0 && this->ammo > 0) {
		this->isFiring = true;
		this->fireTimer = this->fireRate;
		this->ammo--;
	}
}