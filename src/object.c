#include "object.h"

// True ObjectVtableRegistry instance
struct ObjectVtableRegistry ObjectVtableRegistry;

// Function for Spawning Objects
Object CreateObject(int type) {
	Object object;
	ObjectGlobalINIT(&object);
	if (type > OBJECT_NONE && type < OBJECT_TYPES) {
		object.init = ObjectVtableRegistry.INIT_[type];
		object.update = ObjectVtableRegistry.UPDATE_[type];
		object.type = type;
		object.init(&object);
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
	this->type = OBJECT_NONE;
	this->hitbox = 0.5f;
	this->speed = 0;
	this->hp = this->maxHealth = 1;
	this->pos.x = this->pos.y = 0;
	this->vel.x = this->vel.y = 0;
	this->dir.x = this->dir.y = 0;
	this->texture = IMG_Load("res/null.png");
}

// Global Object Update
void ObjectGlobalUPDATE(Object* this, Player player, Map map, float dt) {
	this->pos.x += this->vel.x * dt;
	this->pos.y += this->vel.y * dt;

	// take damage when shot
	for (int i = 0; i < SLOTS; i++) {
		if (player.inventory[i].isGun) {
			if (player.inventory[i].isFiring) {
				for (float j = 0; true; j += 0.001) {
					Vec2F dir = player.dir;

					float x = player.pos.x + player.dir.x * j;
					float y = player.pos.y + player.dir.y * j;
					int tile = getTile(map, x, y);

					if (tile > TILE_COLLISION_START && tile < TILE_COLLISION_END) {
						break;
					}

					if (x > this->pos.x - this->hitbox
					 && x < this->pos.x + this->hitbox
					 && y > this->pos.y - this->hitbox
					 && y < this->pos.y + this->hitbox) {
						this->hp -= player.inventory[i].damage;
						break;
					}
				}
			}
		}
	}

	// If no hp then die
	if (this->hp <= 0 && this->type > OBJECT_NULL)
		this->destroy = true;
}