#include "draw.h"

float dist2Player(Player player, Object object) {
	return sqrt(powf(player.pos.x - object.pos.x, 2.f) + powf(player.pos.y - object.pos.y, 2.f));
}

float dist2Object(Player player, Object object) {
	return sqrt(powf(object.pos.x - player.pos.x, 2.f) + powf(object.pos.y - player.pos.y, 2.f));
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

void drawBackground(SDL_Renderer* renderer, SDL_DisplayMode display) {
	SDL_Color ceilingColor = {40, 40, 40};
	SDL_Color floorColor = {-2, -2, -2};
	SDL_SetRenderDrawColor(renderer, ceilingColor.r, ceilingColor.g, ceilingColor.b, ceilingColor.a);
	for (int i = 0; i < display.h / 2; i++) {
		SDL_RenderDrawLine(renderer, 0, i, display.w, i);
		SDL_SetRenderDrawColor(renderer, ceilingColor.r - (i % display.h / 10), ceilingColor.g - (i % display.h / 10), ceilingColor.b - (i % display.h / 10), ceilingColor.a);	
	}
	SDL_SetRenderDrawColor(renderer, floorColor.r, floorColor.g, floorColor.b, floorColor.a);
	for (int i = 0; i < display.h / 2; i++) {
		SDL_RenderDrawLine(renderer, 0, i + display.h / 2, display.w, i + display.h / 2);
		SDL_SetRenderDrawColor(renderer, floorColor.r - (-i % display.h / 10), floorColor.g - (-i % display.h / 10), floorColor.b - (-i % display.h / 10), floorColor.a);	
	}
}

void drawObjects(SDL_Renderer* renderer, SDL_DisplayMode display, int objects, Object object[objects], Player player) {
	int drawOrder[objects];
	for (int i = 0; i < objects; i++)
		drawOrder[i] = i;
	bool done;
	while (!done) {
		done = true;
		for (int i = 0; i < objects - 1; i++) {
			int a = drawOrder[i];
			int b = drawOrder[i+1];
			if (dist2Player(player, object[a]) < dist2Player(player, object[b])) {
				drawOrder[i] = b;
				drawOrder[i+1] = a;
				done = false;
			}
		}
	}
	for (int t = 0; t < objects; t++) {
		Object* this = &object[drawOrder[t]];

		float x = this->pos.x - player.pos.x;
		float y = this->pos.y - player.pos.y;

		float invDet = 1 / (player.plane.x * player.dir.y - player.dir.x * player.plane.y);

		float transformX = invDet * (player.dir.y * x - player.dir.x * y);
		float transformY = invDet * (-player.plane.y * x + player.plane.x * y);

		this->rect.w = fabsf(display.h / (transformY));
		this->rect.h = fabsf(display.h / (transformY));

		this->rect.x = (display.w / 2.f) * (1 + transformX / transformY);
		this->rect.y = (display.h / 2.f - this->rect.h / 2.f);

		//calculate lowest and highest pixel to fill in current stripe
		int drawStartY = -this->rect.h / 2 + display.h / 2.f;
		if(drawStartY < 0) drawStartY = 0;
		int drawEndY = this->rect.h / 2 + display.h / 2.f;
		if(drawEndY >= display.h) drawEndY = display.h - 1;

		//calculate width of the sprite
		int drawStartX = -this->rect.w / 2.f + this->rect.x;
		if(drawStartX < 0) drawStartX = 0;
		int drawEndX = this->rect.w / 2.f + this->rect.x;
		if(drawEndX >= display.w) drawEndX = display.w - 1;

		for (int i = drawStartX; i < drawEndX; i++) {
			int texX = 256 * (i - (-this->rect.w / 2.f + this->rect.x)) * this->texture->w / this->rect.w / 256;
			if(transformY > 0 && i > 0 && i < display.w) {
				for (int j = drawStartY; j < drawEndY; j++) {
					int d = (j) * 256 - display.h * 128 + this->rect.h * 128;
					int texY = ((d * this->texture->h) / this->rect.h) / 256;
					SDL_Color color;
					SDL_GetRGBA(getPixel(this->texture, texX, texY), this->texture->format, &color.r, &color.g, &color.b, &color.a);
					SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
					if (color.a != 0)
						SDL_RenderDrawPoint(renderer, i, j);
				}
			}
		}
	}
}

void drawWalls(SDL_Renderer* renderer, SDL_DisplayMode display, int objects, Object object[objects], Player player, Map map) {
	for (int x = 0; x < display.w; x++) {
		// calculate ray position and direction
		float cameraX = 2 * x / (float)display.w - 1; // x-coordinate in camera space
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
			if (tile > 0 && tile < 6)
				hit = tile;
			if (i > 16)
				break;
		}

		// Calculate the distance projceted on camera direction
		// (Euclidian distance would give fisheye effect)
		if (side)
			perpWallDist = (sideDist.y - deltaDist.y);
		else
			perpWallDist = (sideDist.x - deltaDist.x);

		// Calculate height of line to draw on screen 
		int lineHeight = (int)(display.h / perpWallDist);

		// calculate lowest and highest pixel to fill in current stripe
		int drawStart = -lineHeight / 2 + display.h / 2;
		if (drawStart < 0) drawStart = 0;
		int drawEnd = lineHeight / 2 + display.h / 2; 
		if (drawEnd >= display.h) drawEnd = display.h;

		// choose wall color
		SDL_Color color;
		if (hit == TILE_GLITCHED) {
			color.r = rand() % 256;
			color.g = rand() % 256;
			color.b = rand() % 256;
		}
		else if (hit == TILE_RED) {
			color.r = 255;
			color.g = 0;
			color.b = 0;
		}
		else if (hit == TILE_GREEN) {
			color.r = 0;
			color.g = 80;
			color.b = 0;
		}
		else if (hit == TILE_BLUE) {
			color.r = 0;
			color.g = 0;
			color.b = 200;
		}
		else if (hit == TILE_PURPLE) {
			color.r = 150;
			color.g = 0;
			color.b = 150;
		}
		else {
			color.r = 0;
			color.g = 0;
			color.b = 0;
		}

		// give x and y sides different brightness
		if (!side && hit) {
			color.r /= 2; 
			color.g /= 2; 
			color.b /= 2; 
		}

		// Draw
		SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
		for (int i = drawStart; i < drawEnd; i++) {
			bool draw = true;
			for (int j = 0; j < objects; j++) {
				int spriteX = object[j].rect.x - object[j].rect.w / 2;
				int spriteW = object[j].rect.x + object[j].rect.w / 2;
				int spriteY = object[j].rect.y;
				int spriteH = object[j].rect.y + object[j].rect.h;
				float invDet = 1 / (player.plane.x * player.dir.y - player.dir.x * player.plane.y);
				float transformY = invDet * (-player.plane.y * (object[j].pos.x - player.pos.x) + player.plane.x * (object[j].pos.y - player.pos.y));
				int texX = 256 * (x - (-object[j].rect.w / 2.f + object[j].rect.x)) * object[j].texture->w / object[j].rect.w / 256;
				int d = (i) * 256 - display.h * 128 + object[j].rect.h * 128;
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
			if (draw)
				SDL_RenderDrawPoint(renderer, x, i);
		}
	}
}

void drawHUD(SDL_Renderer* renderer, Player* player) {

}