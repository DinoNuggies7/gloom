#include "input.h"
#include "draw.h"
#include "map.h"
#include "object.h"
#include "player.h"

SDL_Window* window;
SDL_Surface* windowSurface;
SDL_Surface* screen;

int main(int argc, char** argv) {
	SDL_Init(SDL_INIT_EVERYTHING);
	IMG_Init(IMG_INIT_PNG);

	SDL_DisplayMode display;
	SDL_GetCurrentDisplayMode(0, &display);

	window = SDL_CreateWindow("GLOOM.EXE", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, display.w, display.h, SDL_WINDOW_FULLSCREEN_DESKTOP);
	SDL_Surface* icon = IMG_Load("res/icon.png");
	SDL_SetWindowIcon(window, icon);
	windowSurface = SDL_GetWindowSurface(window);

	SDL_Rect windowSize = {0, 0, display.w, display.h};
	SDL_Rect view = {0, 0, 640, 360};
	screen = SDL_CreateRGBSurface(0, view.w, view.h, 32, 0, 0, 0, 0);

	SDL_SetRelativeMouseMode(true);

	SDL_Surface* texture[TEXTURES];
	for (int i = 0; i < TEXTURES; i++) {
		char texturePath[15] = "res/wall/";
		char textureNum[2];
		sprintf(textureNum, "%d", i);
		strncat(texturePath, textureNum, 2);
		strcat(texturePath, ".png");

		texture[i] = IMG_Load(texturePath);
	}

	float ticks = SDL_GetTicks();
	float lastTicks;
	float dt;

	Player player;
	Player__INIT(&player);

	Map map;
	parseMap(&map, player.map);

	int objects = map.spawns;
	Object object[objects];

	for (int i = 0; i < objects; i++) {
		if (map.spawn[i].z < 0) {
			object[i] = CreateObject(OBJECT_ITEM, abs(map.spawn[i].z));
		}
		else
			object[i] = CreateObject(map.spawn[i].z);
		object[i].pos.x = map.spawn[i].x + 0.5;
		object[i].pos.y = map.spawn[i].y + 0.5;
	}

	for (int j = 0; j < map.h; j++) {		// Print map data
		for (int i = 0; i < map.w; i++) {
			if (getTile(map, i, j) == TILE_SPAWN) {
				printf("@ ");
				player.pos.x = i;
				player.pos.y = j;
			}
			else if (getTile(map, i, j) > TILE_COLLISION_START && getTile(map, i, j) < TILE_COLLISION_END)
				printf("%d ", getTile(map, i, j));
			else
				printf(". ");
		}
		printf("\n");
	}

	SDL_Event event;
	while (!player.quit) {
		handleInput(&event, &player);

		lastTicks = ticks;
		ticks = SDL_GetTicks();
		dt = (ticks - lastTicks) / 1000;

		Player__UPDATE(&player, &map, dt);

		for (int i = 0; i < objects; i++) {
			// Update Objects
			if (!object[i].destroy) {
				if (object[i].type > OBJECT_NONE && object[i].type < OBJECT_TYPES)
					object[i].update(&object[i], dt, view, &player, map);
				ObjectGlobalUPDATE(&object[i], dt, player, map);
			}
			else // Remove Objects
				DestroyObject(&object[i]);
		}

		SDL_FillRect(screen, &view, SDL_MapRGB(screen->format, 0, 0, 0));

		drawBackground(screen, texture, view, player);
		drawForeground(screen, texture, view, player, object, objects, map);
		// drawHUD(screen, view, player);

		SDL_BlitScaled(screen, NULL, windowSurface, &windowSize);
		SDL_UpdateWindowSurface(window);
	}
	
	save(&player);

	SDL_FreeSurface(icon);
	for (int i = 0; i < SLOTS; i++) {
		SDL_FreeSurface(player.inventory[i].texture);
		if (player.inventory[i].texture != player.inventory[i].itemTexture)
			SDL_FreeSurface(player.inventory[i].itemTexture);
	}
	for (int i = 0; i < objects; i++) {
		SDL_FreeSurface(object[i].texture);
	}
	for (int i = 0; i < TEXTURES; i++) {
		SDL_FreeSurface(texture[i]);
	}
	SDL_FreeSurface(screen);
	SDL_FreeSurface(windowSurface);
	SDL_DestroyWindow(window);
	IMG_Quit();
	SDL_Quit();
	return 0;
}

int WinMain(int argc, char** argv) {
	return main(argc, argv);
}