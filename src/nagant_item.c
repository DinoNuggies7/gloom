#include "nagant_item.h"
#include "player.h"

void Item__Nagant__INIT(Item* this, ...) {
	this->isGun = true;
	this->damage = 2;
	this->range = 20;
	this->ammo = this->rounds = 7;
	this->fireRate = 0.4;
	this->reloadRate = 2;
	this->drot = -15;
	this->rotPoint.x = 1, this->rotPoint.y = 1;
	this->texture = IMG_Load("res/nagant.png");
	this->itemTexture = IMG_Load("res/nagant_item.png");
}

void Item__Nagant__USE(Item* this, ...) {
	ItemGlobalUSE(this);
}