#include "player.h"

void Player__INIT(Player* this) {
	this->quit = false;
	this->jumpscare = 0;
	this->finish = false;
	this->walkSpeed = 2;
	this->runSpeed = 3;
	this->maxStamina = 5;
	this->stamina = this->maxStamina;
	this->collided.x = this->collided.y = false;
	this->hitbox = 0.3;
	this->select = 0;
	this->selectTimer = 0;
	this->choosing = false;
	this->forward = this->backword = this->left = this->right = false;
	this->sprint = false, this->tired = false;
	this->lookup = this->lookdown = this->lookleft = this->lookright = false;
	this->leftclick = this->rightclick = false;
	this->lookspeed = 1.5;
	this->dir.x = 1.0;
	this->dir.y = 0.0;
	this->plane.x = 0.0;
	this->plane.y = 1.0;

	this->hudbarTexture = IMG_Load("res/item/hudbar.png");
	this->selectTexture = IMG_Load("res/item/select.png");

	FILE* file = fopen(CONFIG_PATH, "r");
	if (file == NULL) {
		printf("couldn't find config file, creating one\n");

		// Default values
		this->map = "res/map/level0.txt";
		this->sensitivity = 0.5;
		this->equip[LEFT] = 0;
		this->equip[RIGHT] = 1;
		for (int i = 0; i < SLOTS; i++)
			this->inventory[i] = CreateItem(ITEM_NONE);

		save(this);
	}
	else
		load(this, file);
}

void camera(Player* this, float dt);
void movement(Player* this, float dt);
void collision(Player* this, Map map, float dt);
void handleItems(Player* this, Map* map, float dt);

void Player__UPDATE(Player* this, Map* map, float dt) {
	// Neater Code
	camera(this, dt);
	movement(this, dt);
	collision(this, *map, dt);
	handleItems(this, map, dt);

	// Applying Velocity
	this->pos.x += this->vel.x * dt;
	this->pos.y += this->vel.y * dt;
}

void camera(Player* this, float dt) {
	// Choose method of moving camera
	Vec2F rotSpeed;
	rotSpeed.x = 0.0;
	rotSpeed.y = 0.0;
	if (this->lookleft)
		rotSpeed.x = -this->lookspeed * dt;
	else if (this->lookright)
		rotSpeed.x = this->lookspeed * dt;
	if (this->xrel != 0)
		rotSpeed.x = (this->lookspeed * (this->xrel * this->sensitivity)) * dt;

	// Move camera
	Vec2F oldDir = this->dir;
	this->dir.x = this->dir.x * cosf(rotSpeed.x) - this->dir.y * sinf(rotSpeed.x);
	this->dir.y = oldDir.x    * sinf(rotSpeed.x) + this->dir.y * cosf(rotSpeed.x);

	Vec2F oldPlane = this->plane;
	this->plane.x = (this->plane.x * cosf(rotSpeed.x) - this->plane.y * sinf(rotSpeed.x));
	this->plane.y = (oldPlane.x    * sinf(rotSpeed.x) + this->plane.y * cosf(rotSpeed.x));
	this->xrel = 0;

	// Normalizing dir
	this->dir = NormalizeVec2F(this->dir);
}

void movement(Player* this, float dt) {
	// Deplete stamina when sprinting
	bool isMoving = this->forward || this->backword || this->left || this->right;
	if (this->sprint && isMoving && !this->tired)
		this->stamina -= 1 * dt;
	else
		this->stamina += 1 * dt;
	if (this->stamina > this->maxStamina) {
		this->stamina = this->maxStamina;
		this->tired = false;
	}
	if (this->stamina < 0) {
		this->stamina = 0;
		this->tired = true;
	}

	Vec2F walkDir = {0, 0};
	float speed;
	if (this->sprint && !this->tired) speed = this->runSpeed; else speed = this->walkSpeed;

	if (this->forward) {
		walkDir.x += this->dir.x;
		walkDir.y += this->dir.y;
	}
	if (this->backword) {
		walkDir.x -= this->dir.x;
		walkDir.y -= this->dir.y;
	}
	if (this->left) {
		walkDir.x += this->dir.y;
		walkDir.y -= this->dir.x;
	}
	if (this->right) {
		walkDir.x -= this->dir.y;
		walkDir.y += this->dir.x;
	}

	walkDir = NormalizeVec2F(walkDir);
	
	this->vel.x = walkDir.x * speed;
	this->vel.y = walkDir.y * speed;
}

void collision(Player* this, Map map, float dt) {
	// where the player WILL be, both sides of each 4 corners away from the player (hitbox)
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

	// Wall Collision
	for (int i = 0; i < 4; i++) {
		Vec2I tile = {getTile(map, this->collisionBox[i].x, this->collisionBox[i].y), getTile(map, this->collisionBox[i+4].x, this->collisionBox[i+4].y)};
		if (tile.x > TILE__COLLISION_OBJECT)
			this->vel.x = 0;
		if (tile.y > TILE__COLLISION_OBJECT)
			this->vel.y = 0;
	}

	// Object Collision
	if (this->collided.x)
		this->vel.x = 0;
	if (this->collided.y)
		this->vel.y = 0;
	this->collided.x = this->collided.y = false;
	
	// End of Level Collision
	if (getTile(map, this->pos.x, this->pos.y) == TILE_DOOR) {
		this->finish = true;
		printf("You beat the game les go\n");
	}
}

void handleItems(Player* this, Map* map, float dt) {
	// Globally Update Items
	for (int i = 0; i < SLOTS; i++) {
		if (this->inventory[i].isGun)
			ItemGlobalUPDATE(&this->inventory[i], dt);
	}

	// Use/Equip Items
	if (this->leftclick) {
		if (!this->choosing) {
			if (this->inventory[this->equip[LEFT]].type > ITEM_NONE && this->inventory[this->equip[LEFT]].type < ITEM_TYPES)
				this->inventory[this->equip[LEFT]].use(&this->inventory[this->equip[LEFT]], this, map);
		}
		else {
			if (this->select != this->equip[RIGHT]) {
				this->equip[LEFT] = this->select;
				this->selectTimer = 0.001;
			}
			else {
				int tmp = this->equip[LEFT];
				this->equip[LEFT] = this->equip[RIGHT];
				this->equip[RIGHT] = tmp;
			}
		}
	}
	if (this->rightclick) {
		if (!this->choosing) {
			if (this->inventory[this->equip[RIGHT]].type > ITEM_NONE && this->inventory[this->equip[RIGHT]].type < ITEM_TYPES)
				this->inventory[this->equip[RIGHT]].use(&this->inventory[this->equip[RIGHT]], this, map);
		}
		else {
			if (this->select != this->equip[LEFT]) {
				this->equip[RIGHT] = this->select;
				this->selectTimer = 0.001;
			}
			else {
				int tmp = this->equip[RIGHT];
				this->equip[RIGHT] = this->equip[LEFT];
				this->equip[LEFT] = tmp;
			}
		}
	}

	// Select Items (to Equip)
	if (this->scroll < 0) {
		this->selectTimer = 1;
		this->select++;
	}
	else if (this->scroll > 0) {
		this->selectTimer = 1;
		this->select--;
	}
	this->scroll = 0;
	if (this->select > SLOTS - 1) this->select = 0;
	else if (this->select < 0) this->select = SLOTS - 1;

	if (this->selectTimer > 0) {
		this->choosing = true;
		this->selectTimer -= 1 * dt;
	}
	else {
		this->choosing = false;
		this->selectTimer = 0;
	}
}

void save(Player* this) {
	cJSON* json = cJSON_CreateObject();
	cJSON_AddStringToObject(json, "map", this->map);
	cJSON_AddNumberToObject(json, "sensitivity", this->sensitivity);
	cJSON_AddNumberToObject(json, "equipL", this->equip[LEFT]);
	cJSON_AddNumberToObject(json, "equipR", this->equip[RIGHT]);
	for (int i = 0; i < SLOTS; i++) { // Inventory
		char buffer[5];
		snprintf(buffer, 5, "inv%d", i);
		cJSON_AddNumberToObject(json, buffer, this->inventory[i].type);
	}
	char* json_str = cJSON_Print(json);
	FILE* file = fopen(CONFIG_PATH, "w");
	fputs(json_str, file);
	fclose(file);
	cJSON_free(json_str);
	cJSON_Delete(json);
	printf("saved game\n");
}

void load(Player* this, FILE* file) {
	char buffer[1024];
	fread(buffer, 1, sizeof(buffer), file);
	cJSON* json = cJSON_Parse(buffer);

	// Map
	cJSON* map = cJSON_GetObjectItemCaseSensitive(json, "map");
	if (cJSON_IsString(map)) {
		this->map = (char*)malloc(strlen(map->valuestring));
		strcpy(this->map, map->valuestring);
	}

	// Sensitivity
	cJSON* sensitivity = cJSON_GetObjectItemCaseSensitive(json, "sensitivity");
	if (cJSON_IsNumber(sensitivity))
		this->sensitivity = sensitivity->valuedouble;

	// Equip
	cJSON* equipL = cJSON_GetObjectItemCaseSensitive(json, "equipL");
	if (cJSON_IsNumber(equipL))
		this->equip[LEFT] = equipL->valueint;
	cJSON* equipR = cJSON_GetObjectItemCaseSensitive(json, "equipR");
	if (cJSON_IsNumber(equipR))
		this->equip[RIGHT] = equipR->valueint;

	// Inventory
	cJSON* inv[SLOTS];
	for (int i = 0; i < SLOTS; i++) {
		char buffer[5];
		snprintf(buffer, 5, "inv%d", i);
		inv[i] = cJSON_GetObjectItemCaseSensitive(json, buffer);
		if (cJSON_IsNumber(inv[i]))
			this->inventory[i] = CreateItem(inv[i]->valueint);
	}

	fclose(file);
	cJSON_Delete(json);
	printf("loaded game\n");
}
