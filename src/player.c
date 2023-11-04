#include "player.h"

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

void load(Player* this, FILE* file, ItemFunction* itemFunc) {
	char buffer[1024];
	int len = fread(buffer, 1, sizeof(buffer), file);
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
			this->inventory[i] = CreateItem(inv[i]->valueint, itemFunc);
	}

	fclose(file);
	cJSON_Delete(json);
	printf("loaded game\n");
}

void Player__INIT(Player* this, ItemFunction* itemFunc) {
	this->quit = false;
	this->maxSpeed = 5.f;
	this->speed = 5.f;
	this->minSpeed = 5.f;
	this->friction = 5.f;
	this->hitbox = 0.3f;
	this->select = 0;
	this->selectTimer = 0;
	this->choosing = false;
	this->forward = this->backword = this->left = this->right = false;
	this->lookup = this->lookdown = this->lookleft = this->lookright = false;
	this->leftclick = this->rightclick = false;
	this->lookspeed = 1.5f;
	this->dir.x = 1.0f;
	this->dir.y = 0.0f;
	this->plane.x = 0.0f;
	this->plane.y = 1.0f;

	this->selectTexture = IMG_Load("res/select.png");

	FILE* file = fopen(CONFIG_PATH, "r");
	if (file == NULL) {
		printf("couldn't find config file, creating one\n");

		// Default values
		this->map = "res/test.txt";
		this->sensitivity = 0.5;
		this->equip[LEFT] = 0;
		this->equip[RIGHT] = 2;
		this->inventory[0] = CreateItem(ITEM_PICKAXE, itemFunc);
		this->inventory[1] = CreateItem(ITEM_NAGANT, itemFunc);
		this->inventory[2] = CreateItem(ITEM_BRICK, itemFunc);
		this->inventory[3] = CreateItem(ITEM_RED, itemFunc);
		this->inventory[4] = CreateItem(ITEM_GREEN, itemFunc);
		this->inventory[5] = CreateItem(ITEM_PURPLE, itemFunc);

		save(this);
	}
	else
		load(this, file, itemFunc);
}

void camera(Player* player, float dt);
void movement(Player* player, float dt);
void collision(Player* player, Map map, float dt);
void handleItems(Player* this, Map* map, float dt);

void Player__UPDATE(Player* this, Map* map, float dt) {
	// Neater Code
	camera(this, dt);
	movement(this, dt);
	collision(this, *map, dt);

	// Applying Velocity
	this->pos.x += this->vel.x * dt;
	this->pos.y += this->vel.y * dt;

	handleItems(this, map, dt);
}

void camera(Player* this, float dt) {
	// Choose method of moving camera
	Vec2F rotSpeed;
	rotSpeed.x = 0.0f;
	rotSpeed.y = 0.0f;
	if (this->lookleft)
		rotSpeed.x = -this->lookspeed * dt;
	else if (this->lookright)
		rotSpeed.x = this->lookspeed * dt;
	else if (this->xrel != 0)
		rotSpeed.x = (this->lookspeed * (this->xrel * this->sensitivity)) * dt;
	if (this->lookup)
		rotSpeed.y = -this->lookspeed * dt;
	else if (this->lookdown)		
		rotSpeed.y = this->lookspeed * dt;

	// Move camera
	Vec2F oldDir = this->dir;
	this->dir.x = this->dir.x * cosf(rotSpeed.x) - this->dir.y * sinf(rotSpeed.x);
	this->dir.y = oldDir.x    * sinf(rotSpeed.x) + this->dir.y * cosf(rotSpeed.x);

	Vec2F oldPlane = this->plane;
	this->plane.x = (this->plane.x * cosf(rotSpeed.x) - this->plane.y * sinf(rotSpeed.x));
	this->plane.y = (oldPlane.x    * sinf(rotSpeed.x) + this->plane.y * cosf(rotSpeed.x));
	this->xrel = 0;

	// Normalizing dir
	float magnitude = this->dir.x * this->dir.x + this->dir.y * this->dir.y;
	if (magnitude != 0) {
		this->dir.x /= magnitude;
		this->dir.y /= magnitude;
	}
}

void movement(Player* this, float dt) {
	// Applying friction to velocity
	if (this->vel.x < -this->minSpeed)
		this->vel.x += this->friction;
	else if (this->vel.x > this->minSpeed)
		this->vel.x -= this->friction;
	else
		this->vel.x = 0;

	if (this->vel.y < -this->minSpeed)
		this->vel.y += this->friction;
	else if (this->vel.y > this->minSpeed)
		this->vel.y -= this->friction;
	else
		this->vel.y = 0;

	// Accelerate when pressing the movement keys
	if (this->forward) {
		this->vel.x += this->dir.x * this->speed;
		this->vel.y += this->dir.y * this->speed;
	}
	else if (this->backword) {
		this->vel.x -= this->dir.x * this->speed;
		this->vel.y -= this->dir.y * this->speed;
	}
	if (this->left) {
		this->vel.x += this->dir.y * this->speed;
		this->vel.y -= this->dir.x * this->speed;
	}
	else if (this->right) {
		this->vel.x -= this->dir.y * this->speed;
		this->vel.y += this->dir.x * this->speed;
	}

	// Keeping velocity under maximum
	if (this->vel.x > this->maxSpeed)
		this->vel.x = this->maxSpeed;
	if (this->vel.x < -this->maxSpeed)
		this->vel.x = -this->maxSpeed;

	if (this->vel.y > this->maxSpeed)
		this->vel.y = this->maxSpeed;
	if (this->vel.y < -this->maxSpeed)
		this->vel.y = -this->maxSpeed;
}

void collision(Player* this, Map map, float dt) {
	// Wall Collision
	Vec2I tile[4];
	tile[0].x = getTile(map, (this->pos.x - this->hitbox) + this->vel.x * dt, this->pos.y - this->hitbox);
	tile[1].x = getTile(map, (this->pos.x - this->hitbox) + this->vel.x * dt, this->pos.y + this->hitbox);
	tile[2].x = getTile(map, (this->pos.x + this->hitbox) + this->vel.x * dt, this->pos.y - this->hitbox);
	tile[3].x = getTile(map, (this->pos.x + this->hitbox) + this->vel.x * dt, this->pos.y + this->hitbox);
	tile[0].y = getTile(map, this->pos.x - this->hitbox, (this->pos.y - this->hitbox) + this->vel.y * dt);
	tile[1].y = getTile(map, this->pos.x - this->hitbox, (this->pos.y + this->hitbox) + this->vel.y * dt);
	tile[2].y = getTile(map, this->pos.x + this->hitbox, (this->pos.y - this->hitbox) + this->vel.y * dt);
	tile[3].y = getTile(map, this->pos.x + this->hitbox, (this->pos.y + this->hitbox) + this->vel.y * dt);
	for (int i = 0; i < 4; i++) {
		if (tile[i].x > TILE_COLLISION_START && tile[i].x < TILE_COLLISION_END)
			this->vel.x = 0;
		if (tile[i].y > TILE_COLLISION_START && tile[i].y < TILE_COLLISION_END)
			this->vel.y = 0;
	}
}

void handleItems(Player* this, Map* map, float dt) {
	// Globally Update Items
	for (int i = 0; i < SLOTS; i++) {
		if (this->inventory[i].type == ITEM_NAGANT)
			ItemGlobalUPDATE(&this->inventory[i], dt);
	}

	// Use/Equip Items
	if (this->leftclick) {
		if (!this->choosing)
			this->inventory[this->equip[LEFT]].use(&this->inventory[this->equip[LEFT]], this, map);
		else {
			this->equip[LEFT] = this->select;
			this->selectTimer = 0.001;
		}
	}
	if (this->rightclick) {
		if (!this->choosing)
			this->inventory[this->equip[RIGHT]].use(&this->inventory[this->equip[RIGHT]], this, map);
		else {
			this->equip[RIGHT] = this->select;
			this->selectTimer = 0.001;
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