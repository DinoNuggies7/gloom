#include "draw.h"

const int VIEW_DISTANCE = 256 / SHADE_STRENGTH;

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

void setPixel(SDL_Surface* surface, int x, int y, Uint32 pixel) {
	Uint32* const target_pixel = (Uint32*) ((Uint8*) surface->pixels + y * surface->pitch + x * surface->format->BytesPerPixel);
	*target_pixel = pixel;
}

void setPixelAlpha(SDL_Surface* surface, int x, int y, Uint8 alpha) {
    if (x >= 0 && x < surface->w && y >= 0 && y < surface->h) {
        Uint32* pixels = (Uint32*)surface->pixels;
        Uint32 pixel = pixels[y * surface->w + x];
        
        // Clear the alpha bits and then set the new alpha value
        pixel = (pixel & 0xFFFFFF) | (alpha << 24);

        pixels[y * surface->w + x] = pixel;
    }
}

void loadTextures(SDL_Surface** texture) {
	int loadedTextures = 0;
	for (int i = TEXTURE__WALL; i < TEXTURE__FLOOR; i++) {
		// Load Walls
		char texturePath[15] = "res/wall/";
		char textureNum[2];
		sprintf(textureNum, "%d", i - TEXTURE__WALL);
		strncat(texturePath, textureNum, 2);
		strcat(texturePath, ".png");

		texture[loadedTextures] = IMG_Load(texturePath);
		loadedTextures++;
	}
	for (int i = TEXTURE__FLOOR; i < TEXTURE__CEILING; i++) {
		// Load Floors
		char texturePath[15] = "res/floor/";
		char textureNum[2];
		sprintf(textureNum, "%d", i - TEXTURE__FLOOR);
		strncat(texturePath, textureNum, 2);
		strcat(texturePath, ".png");

		texture[loadedTextures] = IMG_Load(texturePath);
		loadedTextures++;
	}
	for (int i = TEXTURE__CEILING; i < TEXTURE__JUMPSCARE; i++) {
		// Load Ceilings
		char texturePath[15] = "res/ceiling/";
		char textureNum[2];
		sprintf(textureNum, "%d", i - TEXTURE__CEILING);
		strncat(texturePath, textureNum, 2);
		strcat(texturePath, ".png");

		texture[loadedTextures] = IMG_Load(texturePath);
		loadedTextures++;
	}
	for (int i = TEXTURE__JUMPSCARE; i < TEXTURES; i++) {
		// Load Jumpscares
		char texturePath[15] = "res/jumpscare/";
		char textureNum[2];
		sprintf(textureNum, "%d", i - TEXTURE__JUMPSCARE);
		strncat(texturePath, textureNum, 2);
		strcat(texturePath, ".png");

		texture[loadedTextures] = IMG_Load(texturePath);
		loadedTextures++;
	}
	if (loadedTextures == TEXTURES)
		printf("yippi, all textures loaded\n");
	else
		printf("we are missing textures damnit\n");
}

void drawBackground(SDL_Surface* screen, SDL_Surface** texture, SDL_Rect view, Player player) {
	// Floor Casting
	for (int y = 0; y < view.h; y++) {
		// rayDir for leftmost ray (x = 0) and rightmost ray (x = w)
		float rayDirX0 = player.dir.x - player.plane.x;
		float rayDirY0 = player.dir.y - player.plane.y;
		float rayDirX1 = player.dir.x + player.plane.x;
		float rayDirY1 = player.dir.y + player.plane.y;

		// Current y position compared to the center of the screen (the horizon)
		int p = y - view.h / 2;

		// Vertical position of the camera.
		float posZ = 0.5 * view.h;

		// Horizontal distance from the camera to the floor for the current row.
		// 0.5 is the z position exactly in the middle between floor and ceiling.
		float rowDistance = posZ / p;

		// calculate the real world step vector we have to add for each x (parallel to camera plane)
		// adding step by step avoids multiplications with a weight in the inner loop
		float floorStepX = rowDistance * (rayDirX1 - rayDirX0) / view.w;
		float floorStepY = rowDistance * (rayDirY1 - rayDirY0) / view.w;

		// real world coordinates of the leftmost column. This will be updated as we step to the right.
		float floorX = player.pos.x + rowDistance * rayDirX0;
		float floorY = player.pos.y + rowDistance * rayDirY0;

		bool invisible = false;
		for (int x = 0; x < view.w; x++) {
			if (invisible) {
				setPixel(screen, x, y, SDL_MapRGB(screen->format, 0, 0, 0));
				setPixel(screen, x, view.h - y - 1, SDL_MapRGB(screen->format, 0, 0, 0));
			}
			else {
				// the cell coord is simply got from the integer parts of floorX and floorY
				int cellX = (int)(floorX);
				int cellY = (int)(floorY);

				// get the texture coordinate from the fractional part
				int tx = (int)(64 * (floorX - cellX)) & (64 - 1);
				int ty = (int)(64 * (floorY - cellY)) & (64 - 1);

				floorX += floorStepX;
				floorY += floorStepY;

				// choose texture and draw the pixel
				int floorTexture = TEXTURE_CARPET;
				int ceilingTexture = TEXTURE_TILED;

				// Floor
				SDL_Color floorColor;
				SDL_GetRGBA(getPixel(texture[floorTexture], tx, ty), texture[floorTexture]->format, &floorColor.r, &floorColor.g, &floorColor.b, &floorColor.a);
				// shade according to distance from player
				int shadeStrength = rowDistance * SHADE_STRENGTH;
				if (floorColor.r - shadeStrength < 1) floorColor.r = 0; else floorColor.r -= shadeStrength;
				if (floorColor.g - shadeStrength < 1) floorColor.g = 0; else floorColor.g -= shadeStrength;
				if (floorColor.b - shadeStrength < 1) floorColor.b = 0; else floorColor.b -= shadeStrength;
				setPixel(screen, x, y, SDL_MapRGB(screen->format, floorColor.r, floorColor.g, floorColor.b));

				// Ceiling
				SDL_Color ceilingColor;
				SDL_GetRGBA(getPixel(texture[ceilingTexture], tx, ty), texture[ceilingTexture]->format, &ceilingColor.r, &ceilingColor.g, &ceilingColor.b, &ceilingColor.a);
				// shade according to distance from player
				if (ceilingColor.r - shadeStrength < 1) ceilingColor.r = 0; else ceilingColor.r -= shadeStrength;
				if (ceilingColor.g - shadeStrength < 1) ceilingColor.g = 0; else ceilingColor.g -= shadeStrength;
				if (ceilingColor.b - shadeStrength < 1) ceilingColor.b = 0; else ceilingColor.b -= shadeStrength;
				setPixel(screen, x, view.h - y - 1, SDL_MapRGB(screen->format, ceilingColor.r, ceilingColor.g, ceilingColor.b));

				if (floorColor.r == 0 && floorColor.g == 0 && floorColor.b == 0
				&& ceilingColor.r == 0 && ceilingColor.g == 0 && ceilingColor.b == 0)
					invisible = true;
			}
		}
	}
}

void drawForeground(SDL_Surface* screen, SDL_Surface** texture, SDL_Rect view, Player player, Object* object, int objects, Map map) {
	// Draw Objects
	int drawOrder[objects];
	for (int i = 0; i < objects; i++)
		drawOrder[i] = i;
	bool done = false;
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
	Vec2F objectMask[view.w][view.h];
	for (int j = 0; j < view.h; j++) {
		for (int i = 0; i < view.w; i++) {
			objectMask[i][j].x = -1;
			objectMask[i][j].y = -1;
		}
	}
	for (int t = 0; t < objects; t++) {
		Object* obj = &object[drawOrder[t]];
		float distance = dist2Object(player, *obj);
		if (distance < VIEW_DISTANCE) {
			if (obj->type > OBJECT_NULL && obj->type < OBJECT_TYPES) {
				float x = obj->pos.x - player.pos.x;
				float y = obj->pos.y - player.pos.y;

				float invDet = 1 / (player.plane.x * player.dir.y - player.dir.x * player.plane.y);

				float transformX = invDet * (player.dir.y * x - player.dir.x * y);
				float transformY = invDet * (-player.plane.y * x + player.plane.x * y);

				obj->rect.w = fabsf(view.h / (transformY)) * obj->scale;
				obj->rect.h = fabsf(view.h / (transformY)) * obj->scale;

				obj->rect.x = (view.w / 2.f) * (1 + transformX / transformY);
				obj->rect.y = (view.h / 2.f - obj->rect.h / 2);

				// calculate lowest and highest pixel to fill in current stripe
				int drawStartY = -obj->rect.h / 2 + view.h / 2.f;
				if(drawStartY < 0) drawStartY = 0;
				int drawEndY = obj->rect.h / 2 + view.h / 2.f;
				if(drawEndY >= view.h) drawEndY = view.h - 1;

				// calculate width of the sprite
				int drawStartX = -obj->rect.w / 2.f + obj->rect.x;
				if(drawStartX < 0) drawStartX = 0;
				int drawEndX = obj->rect.w / 2.f + obj->rect.x;
				if(drawEndX >= view.w) drawEndX = view.w - 1;

				int shadeStrength = distance * SHADE_STRENGTH;
				for (int i = drawStartX; i < drawEndX; i++) {
					int texX = 256 * (i - (-obj->rect.w / 2.f + obj->rect.x)) * obj->texture->w / obj->rect.w / 256;
					if (transformY > 0 && i > 0 && i < view.w) {
						for (int j = drawStartY; j < drawEndY; j++) {
							int d = (j) * 256 - view.h * 128 + obj->rect.h * 128;
							int texY = ((d * obj->texture->h) / obj->rect.h) / 256;
							SDL_Color color;
							SDL_GetRGBA(getPixel(obj->texture, texX, texY), obj->texture->format, &color.r, &color.g, &color.b, &color.a);
							if (color.a != 0) {
								// shade according to distance from player
								if (color.r - shadeStrength < 1)
									color.r = 0;
								else
									color.r -= shadeStrength;
								if (color.g - shadeStrength < 1)
									color.g = 0;
								else
									color.g -= shadeStrength;
								if (color.b - shadeStrength < 1)
									color.b = 0;
								else
									color.b -= shadeStrength;
								setPixel(screen, i, j, SDL_MapRGB(screen->format, color.r, color.g, color.b));
								objectMask[i][j].x = obj->pos.x;
								objectMask[i][j].y = obj->pos.y;
							}
						}
					}
				}
			}
		}
	}

	// Draw Walls
	for (int x = 0; x < view.w; x++) {
		// calculate ray position and direction
		Vec2F camera;
		camera.x = 2 * x / (float)view.w - 1; // x-coordinate in camera space
		Vec2F rayDir = {
			.x = player.dir.x + player.plane.x * camera.x,
			.y = player.dir.y + player.plane.y * camera.x,
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
			if (tile >= TILE__COLLISION_OBJECT)
				hit = tile;
			if (i > 20)
				break;
		}

		// Calculate the distance projected on camera direction
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
		int texNum = -1;
		SDL_Color color;
		color.a = 255;
		switch (hit) {
			case TILE_GLITCHED:
				color.r = rand() % 120;
				color.g = rand() % 120;
				color.b = rand() % 120;
				break;
			case TILE_BACKROOM:
				texNum = TEXTURE_BACKROOM;
				break;
			case TILE_DOOR:
				texNum = TEXTURE_DOOR;
				break;
			default:
				color.r = 0;
				color.g = 0;
				color.b = 0;
				break;
		}

		// Draw
		int shadeStrength = perpWallDist * SHADE_STRENGTH;
		if (perpWallDist < VIEW_DISTANCE) {
			for (int i = drawStart; i < drawEnd; i++) {
				bool draw = true;
				if (objectMask[x][i].x != -1 && objectMask[x][i].x != -1) {
					float distance = sqrt(powf(objectMask[x][i].x - player.pos.x, 2) + powf(objectMask[x][i].y - player.pos.y, 2));
					float angle = abs(x - view.h) * 0.001 + 1;
					if (distance < perpWallDist * angle)
						draw = false;
				}
				if (draw) {
					if (texNum != -1) {
						//calculate value of wallX
						float wallX; //where exactly the wall was hit
						if (side == 0) wallX = player.pos.y + perpWallDist * rayDir.y;
						else           wallX = player.pos.x + perpWallDist * rayDir.x;
						wallX -= floor((wallX));

						//x coordinate on the texture
						int texX = wallX * texture[texNum]->w;
						if(side == 0 && rayDir.x > 0) texX = texture[texNum]->w - texX - 1;
						if(side == 1 && rayDir.y < 0) texX = texture[texNum]->w - texX - 1;
						int d = (i) * 256 - view.h * 128 + texH * 128;
						int texY = ((d * texture[texNum]->h) / texH) / 256;

						SDL_GetRGB(getPixel(texture[texNum], texX, texY), texture[texNum]->format, &color.r, &color.g, &color.b);

					}
					// shade according to distance from player
					SDL_Color drawColor = color;
					if (drawColor.r - shadeStrength < 1)
						drawColor.r = 0;
					else
						drawColor.r -= shadeStrength;
					if (drawColor.g - shadeStrength < 1)
						drawColor.g = 0;
					else
						drawColor.g -= shadeStrength;
					if (drawColor.b - shadeStrength < 1)
						drawColor.b = 0;
					else
						drawColor.b -= shadeStrength;
					// give x and y sides different brightness
					// if (!side && hit) {
					// 	drawColor.r /= 2; 
					// 	drawColor.g /= 2; 
					// 	drawColor.b /= 2; 
					// }
					setPixel(screen, x, i, SDL_MapRGB(screen->format, drawColor.r, drawColor.g, drawColor.b));
				}
			}
		}
	}
}

// WORKING ON THIS RN
void drawHUD(SDL_Surface* screen, SDL_Surface** texture, SDL_Renderer* renderer, SDL_Rect windowSize, SDL_Rect view, Player player, float dt) {
	// Fade in to the game
	static float fade;
	if (fade == 0) fade = 255;
	if (fade > 0) {
		SDL_SetRenderDrawColor(renderer, 0, 0, 0, fade);
		SDL_RenderFillRect(renderer, &windowSize);
		fade -= 20 * dt;
	}

	// Get jumpscared if gotted
	if (player.jumpscare > 0) {
		int jumpscareSurface = -1;
		switch (player.jumpscare) {
			case OBJECT_DERK:
				jumpscareSurface = TEXTURE_DERK;
				break;
		}
		SDL_Rect jumpscareRect = {rand() % 30 - 15, 0, texture[jumpscareSurface]->w, texture[jumpscareSurface]->h};
		if (jumpscareSurface > -1) {
			SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
			SDL_RenderClear(renderer);
			SDL_Texture* jumpscareTexture = SDL_CreateTextureFromSurface(renderer, texture[jumpscareSurface]);
			SDL_RenderCopy(renderer, jumpscareTexture, &jumpscareRect, &windowSize);
			SDL_DestroyTexture(jumpscareTexture);
		}
	}

	// Equipped Items
	// Item* wieldedItem[2] = {&player.inventory[player.equip[LEFT]], &player.inventory[player.equip[RIGHT]]};
	// SDL_Rect handRect[2] = {
	// 	{
	// 		view.w * (0.2 + wieldedItem[LEFT]->offset.x * -1) - (wieldedItem[LEFT]->frameRect.w * WIELD_SCALE) / 2.f,
	// 		view.h * (0.8 + wieldedItem[LEFT]->offset.y     ) - (wieldedItem[LEFT]->frameRect.h * WIELD_SCALE) / 2.f,
	// 		wieldedItem[LEFT]->frameRect.w * WIELD_SCALE,
	// 		wieldedItem[LEFT]->frameRect.h * WIELD_SCALE
	// 	},
	// 	{
	// 		view.w * (0.8 + wieldedItem[RIGHT]->offset.x) - (wieldedItem[RIGHT]->frameRect.w * WIELD_SCALE) / 2.f,
	// 		view.h * (0.8 + wieldedItem[RIGHT]->offset.y) - (wieldedItem[RIGHT]->frameRect.h * WIELD_SCALE) / 2.f,
	// 		wieldedItem[RIGHT]->frameRect.w * WIELD_SCALE,
	// 		wieldedItem[RIGHT]->frameRect.h * WIELD_SCALE
	// 	}
	// };
	// for (int i = 0; i < 2; i++) {
	// 	SDL_BlitScaled(wieldedItem[i]->texture, &wieldedItem[i]->srcrect[(int)wieldedItem[i]->frame], screen, &handRect[i]);
	// }

	// // Hotbar
	// // SDL_Texture* hudbarTexture = SDL_CreateTextureFromSurface(renderer, player.hudbarTexture);
	// // SDL_RenderCopy(renderer, hudbarTexture, NULL, &hudRect);
	// // SDL_DestroyTexture(hudbarTexture);
	// SDL_Rect hudRect = {0, view.h + 1, view.w, 24};
	// SDL_BlitSurface(player.hudbarTexture, NULL, screen, &hudRect);

	// // Inventory Items
	// for (int i = 0; i < SLOTS; i++) {
	// 	if (player.inventory[i].type > ITEM_NONE) {
	// 		// SDL_Texture* itemTexture = SDL_CreateTextureFromSurface(renderer, player.inventory[i].itemTexture);
	// 		// SDL_RenderCopy(renderer, itemTexture, NULL, &rect);
	// 		// SDL_DestroyTexture(itemTexture);
	// 		SDL_Rect rect = {245 + i * 19, view.h + 5, 16, 16};
	// 		SDL_BlitSurface(player.inventory[i].itemTexture, NULL, screen, &rect);
	// 	}
	// }
	// if (player.choosing) { // Selection Icon
	// 	// SDL_Texture* selectTexture = SDL_CreateTextureFromSurface(renderer, player.selectTexture);
	// 	// SDL_RenderCopy(renderer, selectTexture, NULL, &rect);
	// 	// SDL_DestroyTexture(selectTexture);
	// 	SDL_Rect rect = {243 + player.select * 19, view.h + 3, player.selectTexture->w, player.selectTexture->h};
	// 	SDL_BlitSurface(player.selectTexture, NULL, screen, &rect);
	// }
}
