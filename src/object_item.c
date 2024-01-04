#include "object.h"

void Item_Object_INIT(Object* this, ...) {
	va_list list;
	va_start(list, this);
	int itemID = va_arg(list, int);
	this->maxHealth = itemID;
	this->hitbox = 0.3;
	this->scale = 0.25;

	char texturePath[20] = "res/item/icon/";
	char textureNum[2];
	sprintf(textureNum, "%d", this->maxHealth);
	strncat(texturePath, textureNum, 2);
	strcat(texturePath, ".png");

	this->texture = IMG_Load(texturePath);
}

void Item_Object_UPDATE(Object* this, ...) {
	va_list list;
	va_start(list, this);
	double dt = va_arg(list, double);
	SDL_Rect view = va_arg(list, SDL_Rect);
	Player* player = va_arg(list, Player*);
	Map map = va_arg(list, Map);

	// Collect item when player collides
	if (this->pos.x + this->hitbox > player->pos.x - player->hitbox
	 && this->pos.y + this->hitbox > player->pos.y - player->hitbox
	 && this->pos.x - this->hitbox < player->pos.x + player->hitbox
	 && this->pos.y - this->hitbox < player->pos.y + player->hitbox) {
		for (int i = 0; i < SLOTS; i++) {
			if (player->inventory[i].type == ITEM_NONE) {
				player->inventory[i] = CreateItem(this->maxHealth);
				this->destroy = true;
				break;
			}
		}
	}
}

void __attribute__((constructor)) SetRegistry_Object_Item() {
	ObjectVtableRegistry.INIT_[OBJECT_ITEM] = &Item_Object_INIT;
	ObjectVtableRegistry.UPDATE_[OBJECT_ITEM] = &Item_Object_UPDATE;
}