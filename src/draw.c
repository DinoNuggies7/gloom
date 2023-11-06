#include "draw.h"

float dist2Object(Player player, Object object) {
	return sqrt(powf(player.pos.x - object.pos.x, 2.f) + powf(player.pos.y - object.pos.y, 2.f));
}

Uint32 getPixel(SDL_Surface* surface, int x, int y) {
	int bpp = surface->format->BytesPerPixel;
	/* Here p is the address to the pixel we want to retrieve */
	Uint8 *p = (Uint8 *)surface->pixels + y * surface->pitch + x * bpp;
	switch (bpp) {
		case 1:
			return *p;
			break;
		case 2:
			return *(Uint16 *)p;
			break;
		case 3:
			if (SDL_BYTEORDER == SDL_BIG_ENDIAN)
				return p[0] << 16 | p[1] << 8 | p[2];
			else
				return p[0] | p[1] << 8 | p[2] << 16;
			break;
			case 4:
				return *(Uint32 *)p;
				break;
			default:
				return 0;       /* shouldn't happen, but avoids warnings */
	}
}

void drawBackground(SDL_Renderer* renderer, SDL_Rect view) {
	SDL_Color floorColor = {40, 40, 40, 255};
	for (int i = 0; i < view.h / 2; i++) {
		SDL_SetRenderDrawColor(renderer, floorColor.r - (i / 4.4), floorColor.g - (i / 4.4), floorColor.b - (i / 4.4), floorColor.a);	
		SDL_RenderDrawLine(renderer, 0, i, view.w, i);
		SDL_SetRenderDrawColor(renderer, floorColor.r - (i / 4.4), floorColor.g - (i / 4.4), floorColor.b - (i / 4.4), floorColor.a);	
		SDL_RenderDrawLine(renderer, 0, -i + view.h, view.w, -i + view.h);
	}
}

void drawForeground(SDL_Renderer* renderer, SDL_Rect view, SDL_Surface** texture, int objects, Object object[objects], Player player, Map map) {
	// Draw Objects
	int drawOrder[objects];
	for (int i = 0; i < objects; i++)
		drawOrder[i] = i;
	bool done;
	while (!done) {
		done = true;
		for (int i = 0; i < objects - 1; i++) {
			int a = drawOrder[i];
			int b = drawOrder[i+1];
			if (dist2Object(player, object[a]) < dist2Object(player, object[b])) {
				drawOrder[i] = b;
				drawOrder[i+1] = a;
				done = false;
			}
		}
	}
	for (int t = 0; t < objects; t++) {
		Object* obj = &object[drawOrder[t]];
		if (obj->type != OBJECT_NONE) {

			float x = obj->pos.x - player.pos.x;
			float y = obj->pos.y - player.pos.y;

			float invDet = 1 / (player.plane.x * player.dir.y - player.dir.x * player.plane.y);

			float transformX = invDet * (player.dir.y * x - player.dir.x * y);
			float transformY = invDet * (-player.plane.y * x + player.plane.x * y);

			obj->rect.w = fabsf(view.h / (transformY));
			obj->rect.h = fabsf(view.h / (transformY));

			obj->rect.x = (view.w / 2.f) * (1 + transformX / transformY);
			obj->rect.y = (view.h / 2.f - obj->rect.h / 2.f);

			//calculate lowest and highest pixel to fill in current stripe
			int drawStartY = -obj->rect.h / 2 + view.h / 2.f;
			if(drawStartY < 0) drawStartY = 0;
			int drawEndY = obj->rect.h / 2 + view.h / 2.f;
			if(drawEndY >= view.h) drawEndY = view.h - 1;

			//calculate width of the sprite
			int drawStartX = -obj->rect.w / 2.f + obj->rect.x;
			if(drawStartX < 0) drawStartX = 0;
			int drawEndX = obj->rect.w / 2.f + obj->rect.x;
			if(drawEndX >= view.w) drawEndX = view.w - 1;

			for (int i = drawStartX; i < drawEndX; i++) {
				int texX = 256 * (i - (-obj->rect.w / 2.f + obj->rect.x)) * obj->texture->w / obj->rect.w / 256;
				if(transformY > 0 && i > 0 && i < view.w) {
					for (int j = drawStartY; j < drawEndY; j++) {
						int d = (j) * 256 - view.h * 128 + obj->rect.h * 128;
						int texY = ((d * obj->texture->h) / obj->rect.h) / 256;
						SDL_Color color;
						SDL_GetRGBA(getPixel(obj->texture, texX, texY), obj->texture->format, &color.r, &color.g, &color.b, &color.a);
						SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
						if (color.a != 0)
							SDL_RenderDrawPoint(renderer, i, j);
					}
				}
			}
		}
	}

	// Draw Walls
	for (int x = 0; x < view.w; x++) {
		// calculate ray position and direction
		float cameraX = 2 * x / (float)view.w - 1; // x-coordinate in camera space
		Vec2F rayDir = {
			.x = player.dir.x + player.plane.x * cameraX,
			.y = player.dir.y + player.plane.y * cameraX,
		};

		// wich box of the map we're in 
		Vec2I mapBox = {
			.x = (int)player.pos.x, 
			.y = (int)player.pos.y
		};
		// Length of ray from current position to next x- or y-side
		Vec2F sideDist = {};
		// Lenth of ray from one x- or y-side to next x- or y-side 
		Vec2F deltaDist = {
			.x = (rayDir.x == 0) ? 1e30 : fabsf(1 / rayDir.x),
			.y = (rayDir.y == 0) ? 1e30 : fabsf(1 / rayDir.y),
		};
		float perpWallDist;
		// What direction to step in x- or y-direction (either +1 or -1)
		Vec2I stepDir = {};

		int hit = 0; // was there a wall hit
		bool side; // was a NorthSouth or EastWest wall hit

		// calculate stepDir and initial sideDist
		if (rayDir.x < 0) {
			stepDir.x = -1;
			sideDist.x = (player.pos.x - mapBox.x) * deltaDist.x;
		} else {
			stepDir.x = 1;
			sideDist.x = (mapBox.x + 1.0f - player.pos.x) * deltaDist.x;
		}
		if (rayDir.y < 0) {
			stepDir.y = -1;
			sideDist.y = (player.pos.y - mapBox.y) * deltaDist.y;
		} else {
			stepDir.y = 1;
			sideDist.y = (mapBox.y + 1.0f - player.pos.y) * deltaDist.y;
		}

		// Perform DDA
		for (int i = 0; !hit; i++) {
			// jump to next map square
			if (sideDist.x < sideDist.y) {
				sideDist.x += deltaDist.x;
				mapBox.x += stepDir.x;
				side = false;
			} else {
				sideDist.y += deltaDist.y;
				mapBox.y += stepDir.y;
				side = true;
			}
			int tile = getTile(map, mapBox.x, mapBox.y);
			if (tile > TILE_COLLISION_START && tile < TILE_COLLISION_END)
				hit = tile;
			if (i > 200)
				break;
		}

		// Calculate the distance projceted on camera direction
		// (Euclidian distance would give fisheye effect)
		if (side)
			perpWallDist = (sideDist.y - deltaDist.y);
		else
			perpWallDist = (sideDist.x - deltaDist.x);

		// Calculate height of line to draw on screen 
		int lineHeight = (int)(view.h / perpWallDist);

		// calculate lowest and highest pixel to fill in current stripe
		int drawStart = -lineHeight / 2 + view.h / 2;
		int drawEnd = lineHeight / 2 + view.h / 2; 
		int texH = drawEnd - drawStart;
		if (drawStart < 0) drawStart = 0;
		if (drawEnd >= view.h) drawEnd = view.h;

		// choose wall color
		SDL_Surface* tex = NULL;
		SDL_Color color;
		color.a = 255;
		switch (hit) {
			case TILE_GLITCHED:
				color.r = rand() % 256;
				color.g = rand() % 256;
				color.b = rand() % 256;
				break;
			case TILE_RED:
				color.r = 255;
				color.g = 0;
				color.b = 0;
				break;
			case TILE_GREEN:
				color.r = 0;
				color.g = 80;
				color.b = 0;
				break;
			case TILE_PURPLE:
				color.r = 150;
				color.g = 0;
				color.b = 150;
				break;
			case TILE_BRICK:
				tex = texture[0];
				break;
			case TILE_STONE:
				tex = texture[1];
				break;
			case TILE_DARK:
				tex = texture[2];
				break;
			default:
				color.r = 0;
				color.g = 0;
				color.b = 0;
				break;
		}
		// give x and y sides different brightness
		if (!side && hit) {
			color.r /= 2; 
			color.g /= 2; 
			color.b /= 2; 
		}

		// Draw
		SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
		float invDet = 1 / (player.plane.x * player.dir.y - player.dir.x * player.plane.y);
		for (int i = drawStart; i < drawEnd; i++) {
			bool draw = true;
			for (int j = 0; j < objects; j++) {
				if (object[j].type != OBJECT_NONE) {
					int spriteX = object[j].rect.x - object[j].rect.w / 2;
					int spriteW = object[j].rect.x + object[j].rect.w / 2;
					int spriteY = object[j].rect.y;
					int spriteH = object[j].rect.y + object[j].rect.h;
					float transformY = invDet * (-player.plane.y * (object[j].pos.x - player.pos.x) + player.plane.x * (object[j].pos.y - player.pos.y));
					int texX = 256 * (x - (-object[j].rect.w / 2.f + object[j].rect.x)) * object[j].texture->w / object[j].rect.w / 256;
					int d = (i) * 256 - view.h * 128 + object[j].rect.h * 128;
					int texY = ((d * object[j].texture->h) / object[j].rect.h) / 256;
					if (transformY > 0 && x > spriteX && x < spriteW && i > spriteY && i < spriteH) {
						if (perpWallDist > dist2Object(player, object[j])) {
							SDL_Color objColor;
							SDL_GetRGBA(getPixel(object[j].texture, texX, texY), object[j].texture->format, &objColor.r, &objColor.g, &objColor.b, &objColor.a);
							if (objColor.a != 0) {
								draw = false;
								break;
							}
						}
					}
				}
			}
			if (draw) {
				if (tex != NULL) {
					//calculate value of wallX
					float wallX; //where exactly the wall was hit
					if (side == 0) wallX = player.pos.y + perpWallDist * rayDir.y;
					else           wallX = player.pos.x + perpWallDist * rayDir.x;
					wallX -= floor((wallX));

					//x coordinate on the texture
					int texX = wallX * tex->w;
					if(side == 0 && rayDir.x > 0) texX = tex->w - texX - 1;
					if(side == 1 && rayDir.y < 0) texX = tex->w - texX - 1;
					int d = (i) * 256 - view.h * 128 + texH * 128;
					int texY = ((d * tex->h) / texH) / 256;

					SDL_GetRGB(getPixel(tex, texX, texY), tex->format, &color.r, &color.g, &color.b);
					// give x and y sides different brightness
					if (!side && hit) {
						color.r /= 2; 
						color.g /= 2; 
						color.b /= 2; 
					}
				}
				SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
				SDL_RenderDrawPoint(renderer, x, i);
			}
		}
	}
}

void drawHUD(SDL_Renderer* renderer, SDL_Rect view, Player player) {
	// Equipped Items
	Item* wieldedItem[2] = {&player.inventory[player.equip[LEFT]], &player.inventory[player.equip[RIGHT]]};
	Vec2I wieldTex[2] = {
		{wieldedItem[LEFT]->frameRect.w * WIELD_SCALE, wieldedItem[LEFT]->frameRect.h * WIELD_SCALE},
		{wieldedItem[RIGHT]->frameRect.w * WIELD_SCALE, wieldedItem[RIGHT]->frameRect.h * WIELD_SCALE}
	};
	SDL_Rect handRect[2] = {
		{
			view.w * (0.2 + wieldedItem[LEFT]->offset.x * -1) - wieldTex[LEFT].x / 2.f,
			view.h * (0.8 + wieldedItem[LEFT]->offset.y) - wieldTex[LEFT].y / 2.f,
			wieldTex[LEFT].x,
			wieldTex[LEFT].y
		},
		{
			view.w * (0.8 + wieldedItem[RIGHT]->offset.x) - wieldTex[RIGHT].x / 2.f,
			view.h * (0.8 + wieldedItem[RIGHT]->offset.y) - wieldTex[RIGHT].y / 2.f,
			wieldTex[RIGHT].x,
			wieldTex[RIGHT].y
		}
	};
	for (int i = 0; i < 2; i++) {
		SDL_Texture* itemTexture = SDL_CreateTextureFromSurface(renderer, wieldedItem[i]->texture);
		SDL_RenderCopyEx(renderer, itemTexture, &wieldedItem[i]->srcrect[(int)wieldedItem[i]->frame], &handRect[i], 0, NULL, i == 0 ? SDL_FLIP_HORIZONTAL : SDL_FLIP_NONE);
	}

	// Hotbar
	SDL_Texture* hudTexture = IMG_LoadTexture(renderer, "res/hudbar.png");
	SDL_Rect hudRect = {0, view.h + 1, view.w, 24};
	SDL_RenderCopy(renderer, hudTexture, NULL, &hudRect);

	// Inventory Items
	for (int i = 0; i < SLOTS; i++) {
		SDL_Rect rect = {264 + i * 19, view.h + 5, 16, 16};
		SDL_Texture* itemTexture = SDL_CreateTextureFromSurface(renderer, player.inventory[i].itemTexture);
		SDL_RenderCopy(renderer, itemTexture, NULL, &rect);
	}
	if (player.choosing) { // Selection Icon
		SDL_Rect rect = {262 + player.select * 19, view.h + 3, player.selectTexture->w, player.selectTexture->h};
		SDL_Texture* selectTexture = SDL_CreateTextureFromSurface(renderer, player.selectTexture);
		SDL_RenderCopy(renderer, selectTexture, NULL, &rect);
	}
}