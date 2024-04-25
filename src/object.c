#include "object.h"

// True ObjectVtableRegistry instance
struct ObjectVtableRegistry ObjectVtableRegistry;

// Function for Spawning Objects
Object CreateObject(int type, ...) {
	va_list list;
	va_start(list, type);
	int arg = va_arg(list, int);

	Object object;
	ObjectGlobalINIT(&object);
	if (type > OBJECT_NONE && type < OBJECT_TYPES) {
		object.init = ObjectVtableRegistry.INIT_[type];
		object.update = ObjectVtableRegistry.UPDATE_[type];
		object.type = type;
		object.init(&object, arg);
	}
	return object;
}

// Function for Removing Objects
void DestroyObject(Object* this) {
	printf("Removed Object\n");
	this->destroy = false;
	this->type = OBJECT_NULL;
	this->texture = IMG_Load("res/null.png");
}

// Global Object Init
void ObjectGlobalINIT(Object* this) {
	printf("Spawned Object\n");
	this->destroy = false;
	this->solid = false, this->hurtable = false;
	this->type = OBJECT_NONE;
	this->damage = 0;
	this->hitbox = 0.5;
	this->speed = 0;
	this->scale = 1;
	this->hp = this->maxHealth = 1;
	this->pos.x = this->pos.y = 0;
	this->vel.x = this->vel.y = 0;
	this->dir.x = this->dir.y = 0;
	this->texture = IMG_Load("res/null.png");
}

// Global Object Update
void ObjectGlobalUPDATE(Object* this, float dt, Object* object, int objects, Player* player, Map map) {
	// apply velocity
	this->pos.x += this->vel.x * dt;
	this->pos.y += this->vel.y * dt;
	
	// where the object WILL be, both sides of each 4 corners away from the object (hitbox)
	this->collisionBox[0].x = (this->pos.x - this->hitbox) + this->vel.x * dt;
	this->collisionBox[0].y = this->pos.y - this->hitbox;
	this->collisionBox[1].x = (this->pos.x - this->hitbox) + this->vel.x * dt;
	this->collisionBox[1].y = this->pos.y + this->hitbox;
	this->collisionBox[2].x = (this->pos.x + this->hitbox) + this->vel.x * dt;
	this->collisionBox[2].y = this->pos.y - this->hitbox;
	this->collisionBox[3].x = (this->pos.x + this->hitbox) + this->vel.x * dt;
	this->collisionBox[3].y = this->pos.y + this->hitbox;
	this->collisionBox[4].x = this->pos.x - this->hitbox;
	this->collisionBox[4].y = (this->pos.y - this->hitbox) + this->vel.y * dt;
	this->collisionBox[5].x = this->pos.x - this->hitbox;
	this->collisionBox[5].y = (this->pos.y + this->hitbox) + this->vel.y * dt;
	this->collisionBox[6].x = this->pos.x + this->hitbox;
	this->collisionBox[6].y = (this->pos.y - this->hitbox) + this->vel.y * dt;
	this->collisionBox[7].x = this->pos.x + this->hitbox;
	this->collisionBox[7].y = (this->pos.y + this->hitbox) + this->vel.y * dt;

	// collide with player
	// if (this->solid) {
	// 	Vec2B collide[2];
	// 	collide[0].x = (player->collisionBox[0].x < this->pos.x + this->hitbox && player->collisionBox[0].y < this->pos.y + this->hitbox)
	// 				|| (player->collisionBox[1].x < this->pos.x + this->hitbox && player->collisionBox[1].y < this->pos.y + this->hitbox);
	// 	collide[1].x = (player->collisionBox[2].x > this->pos.x - this->hitbox && player->collisionBox[2].y > this->pos.y - this->hitbox)
	// 				|| (player->collisionBox[3].x > this->pos.x - this->hitbox && player->collisionBox[3].y > this->pos.y - this->hitbox);
	// 	collide[0].y = (player->collisionBox[4].x < this->pos.x + this->hitbox && player->collisionBox[4].y < this->pos.y + this->hitbox)
	// 				|| (player->collisionBox[6].x < this->pos.x + this->hitbox && player->collisionBox[6].y < this->pos.y + this->hitbox);
	// 	collide[1].y = (player->collisionBox[5].x > this->pos.x - this->hitbox && player->collisionBox[5].y > this->pos.y - this->hitbox)
	// 				|| (player->collisionBox[7].x > this->pos.x - this->hitbox && player->collisionBox[7].y > this->pos.y - this->hitbox);
	// 	if (collide[0].x && collide[1].x)
	// 		player->collided.x = true;
	// 	if (collide[0].y && collide[1].y)
	// 		player->collided.y = true;
	// }

	// take damage when shot
	if (this->hurtable) {
		// for (int i = 0; i < SLOTS; i++) {
		// 	if (player->inventory[i].isGun) {
		// 		if (player->inventory[i].isFiring) {
		// 			for (float j = 0; true; j += 0.001) {
		// 				Vec2F dir = player->dir;

		// 				float x = player->pos.x + player->dir.x * j;
		// 				float y = player->pos.y + player->dir.y * j;
		// 				int tile = getTile(map, x, y);

		// 				if (tile >= TILE__COLLISION_OBJECT) {
		// 					break;
		// 				}

		// 				if (x > this->pos.x - this->hitbox
		// 				&& x < this->pos.x + this->hitbox
		// 				&& y > this->pos.y - this->hitbox
		// 				&& y < this->pos.y + this->hitbox) {
		// 					this->hp -= player->inventory[i].damage;
		// 					break;
		// 				}
		// 			}
		// 		}
		// 	}
		// }
		// If no hp then die
		if (this->hp <= 0 && this->type > OBJECT_NULL)
			this->destroy = true;
	}
}
