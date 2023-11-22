#include "item.h"

// True itemFunc declaration
struct ItemFunction itemFunc;

// Function for Spawning Items
Item CreateItem(int type) {
	Item item;
	ItemGlobalINIT(&item);
	if (type > ITEM_NONE && type < ITEM_TYPES) {
		item.init = itemFunc.INIT_[type];
		item.use = itemFunc.USE_[type];
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
	this->range = this->tile = 0;
	this->ammo = this->rounds = 0;
	this->fireTimer = this->reloadTimer = 0;
	this->fireRate = this->reloadRate = 0;
	this->frame = this->frames = 0; this->srcrect = NULL;
	this->frameRect.x = 0; this->frameRect.y = 0;
	this->frameRect.w = 0; this->frameRect.h = 0;
	this->texture = this->itemTexture = IMG_Load("res/null.png");
	this->offset.x = this->offset.y = 0;
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

	if (this->fireTimer > 0)
		this->frame += 20 * dt;
	else
		this->frame = 0;
}

// Global Item Use
void ItemGlobalUSE(Item* this) {
	if (this->fireTimer <= 0 && this->reloadTimer <= 0 && this->ammo > 0) {
		this->isFiring = true;
		this->fireTimer = this->fireRate;
		this->ammo--;
	}
}