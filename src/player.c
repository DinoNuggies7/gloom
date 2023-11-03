#include "player.h"

void Player__INIT(Player* this, const char* configPath) {
	this->maxSpeed = 5.f;
	this->speed = 5.f;
	this->minSpeed = 5.f;
	this->friction = 5.f;
	this->hitbox = 0.5f;
	this->forward = this->backword = this->left = this->right = false;
	this->lookup = this->lookdown = this->lookleft = this->lookright = false;
	this->dig = this->build = false;
	this->lookspeed = 1.5f;
	this->dir.x = 1.0f;
	this->dir.y = 0.0f;
	this->plane.x = 0.0f;
	this->plane.y = 1.0f;

	FILE* file = fopen(configPath, "r");
	if (file == NULL) {
		printf("couldn't find config file, creating one\n");
		cJSON* json = cJSON_CreateObject();
		this->map = "res/test.txt"; // Default values
		this->sensitivity = 1;
		this->equip = TILE_BRICK;
		cJSON_AddStringToObject(json, "map", this->map);
		cJSON_AddNumberToObject(json, "sensitivity", this->sensitivity);
		cJSON_AddNumberToObject(json, "equip", this->equip);
		char* json_str = cJSON_Print(json);
		FILE* newfile = fopen(configPath, "w");
		fputs(json_str, newfile);
		fclose(newfile);
		cJSON_free(json_str);
		cJSON_Delete(json);
	}
	else {
		char buffer[1024];
		int len = fread(buffer, 1, sizeof(buffer), file);
		cJSON* json = cJSON_Parse(buffer);

		cJSON* map = cJSON_GetObjectItemCaseSensitive(json, "map");
		if (cJSON_IsString(map)) {
			this->map = (char*)malloc(strlen(map->valuestring));
			strcpy(this->map, map->valuestring);
		}

		cJSON* sensitivity = cJSON_GetObjectItemCaseSensitive(json, "sensitivity");
		if (cJSON_IsNumber(sensitivity))
			this->sensitivity = sensitivity->valuedouble;

		cJSON* equip = cJSON_GetObjectItemCaseSensitive(json, "equip");
		if (cJSON_IsNumber(equip))
			this->equip = equip->valueint;

		fclose(file);
		cJSON_Delete(json);
	}
}

void Player__UPDATE(Player* this, Map* map, float dt) {
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
	this->plane.x = (this->plane.x * cosf(rotSpeed.x) - this->plane.y * sinf(rotSpeed.x));// + (this->plane.x * cosf(rotSpeed.y) - this->plane.y * sinf(rotSpeed.y));
	this->plane.y = (oldPlane.x    * sinf(rotSpeed.x) + this->plane.y * cosf(rotSpeed.x));// + (oldPlane.y    * sinf(rotSpeed.y) + this->plane.y * cosf(rotSpeed.y));
	this->xrel = 0;

	// Normalizing dir
	float magnitude = this->dir.x * this->dir.x + this->dir.y * this->dir.y;
	if (magnitude != 0) {
		this->dir.x /= magnitude;
		this->dir.y /= magnitude;
	}

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

	// Collision
	Vec2I tile;
	tile.x = getTile(*map, this->pos.x + ((this->vel.x < 0) ? this->vel.x * dt - this->hitbox : this->vel.x * dt + this->hitbox), this->pos.y);
	tile.y = getTile(*map, this->pos.x, this->pos.y + ((this->vel.y < 0) ? this->vel.y * dt - this->hitbox : this->vel.y * dt + this->hitbox));
	if (tile.x > TILE_COLLISION_START && tile.x < TILE_COLLISION_END)
		this->vel.x = 0;
	if (tile.y > TILE_COLLISION_START && tile.y < TILE_COLLISION_END)
		this->vel.y = 0;

	// Applying Velocity
	this->pos.x += this->vel.x * dt;
	this->pos.y += this->vel.y * dt;

	// Remove map tiles
	if (this->dig) {
		int x = this->pos.x + this->dir.x;
		int y = this->pos.y + this->dir.y;
		int tile = getTile(*map, x, y);
		if (tile > TILE_COLLISION_START && tile < TILE_COLLISION_END)
			setTile(map, x, y, TILE_NONE);
	}
	if (this->build) {
		int x = this->pos.x + this->dir.x;
		int y = this->pos.y + this->dir.y;
		int tile = getTile(*map, round(x), round(y));
		if (tile == TILE_NONE)
			setTile(map, x, y, this->equip);
	}
}
