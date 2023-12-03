#include "item.h"
#include "player.h"

void Item__Nagant__INIT(Item* this, ...) {
	this->isGun = true;
	this->damage = 2;
	this->range = 20;
	this->ammo = this->rounds = 7;
	this->fireRate = 0.4;
	this->reloadRate = 2;
	this->frames = 9;
	this->frameRect.x = 1; this->frameRect.y = 1;
	this->frameRect.w = 128; this->frameRect.h = 128;
	this->srcrect = malloc(sizeof(SDL_Rect) * this->frames);
	this->texture = IMG_Load("res/nagantsheet.png");
	this->itemTexture = IMG_Load("res/nagant_item.png");
	this->offset.x = -0.1;
	this->offset.y = -0.08;

	for (int j = 0; j < 3; j++) {
		for (int i = 0; i < 3; i++) {
			this->srcrect[i+j*3].x = this->frameRect.x + i * (this->frameRect.w + 1);
			this->srcrect[i+j*3].y = this->frameRect.x + j * (this->frameRect.h + 1);
			this->srcrect[i+j*3].w = this->frameRect.w;
			this->srcrect[i+j*3].h = this->frameRect.h;
		}
	}
}

void Item__Nagant__USE(Item* this, ...) {
	ItemGlobalUSE(this);
}

void __attribute__((constructor)) SetRegistry() {
	ItemVtableRegistry.INIT_[ITEM_NAGANT] = &Item__Nagant__INIT;
	ItemVtableRegistry.USE_[ITEM_NAGANT] = &Item__Nagant__USE;
}