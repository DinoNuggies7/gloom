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
	for (int i = ITEM_RED; i <= ITEM_BRICK; i++)
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
	this->range = this->tile = 0;
	this->ammo = this->rounds = 0;
	this->fireTimer = this->reloadTimer = 0;
	this->fireRate = this->reloadRate = 0;
	this->texture = IMG_Load("res/null.png");
}

// Global Item Update
void ItemGlobalUPDATE(Item* this, float dt) {
	if (this->fireTimer > 0) this->fireTimer -= 1 * dt;
	else this->fireTimer = 0;

	if (this->reloadTimer > 0) this->reloadTimer -= 1 * dt;
	else this->reloadTimer = 0;

	if (this->ammo <= 0 && this->reloadTimer <= 0) {
		printf("reloading: %d\n", this->type);
		this->reloadTimer = this->reloadRate;
		this->ammo = this->rounds;
	}
}

// Global Item Use
void ItemGlobalUSE(Item* this) {
	printf("fired: %d\n", this->type);
	this->fireTimer = this->fireRate;
	this->ammo--;
}