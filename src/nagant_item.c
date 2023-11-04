#include "nagant_item.h"

void Item__Nagant__INIT(Item* this, ...) {
	this->range = 20;
	this->ammo = this->rounds = 7;
	this->fireRate = 0.4;
	this->reloadRate = 2;
	this->texture = IMG_Load("res/nagant.png");
}

void Item__Nagant__USE(Item* this, ...) {
	if (this->fireTimer <= 0 && this->reloadTimer <= 0 && this->ammo > 0) {
		ItemGlobalUSE(this);
		printf("fired nagant\n");
	}
}