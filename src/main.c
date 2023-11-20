#include "input.h"
#include "draw.h"
#include "map.h"
#include "object.h"
#include "player.h"

#include <SDL2/SDL.h>

SDL_Window* window;
SDL_Renderer* renderer;

int main(int argc, char** argv) {
	SDL_Init(SDL_INIT_EVERYTHING);
	IMG_Init(IMG_INIT_PNG);

	SDL_DisplayMode display;
	SDL_GetCurrentDisplayMode(0, &display);

	window = SDL_CreateWindow("GLOOM", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, display.w, display.h, SDL_WINDOW_ALLOW_HIGHDPI | SDL_WINDOW_FULLSCREEN_DESKTOP);
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

	SDL_Rect view = {0, 0, 640, 336};
	SDL_RenderSetScale(renderer, (float)display.w / view.w, (float)display.h / (view.h + 24));

	SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, 0);

	SDL_SetRelativeMouseMode(true);
	// SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

	SDL_Surface* texture[TEXTURES];
	texture[TEXTURE_HUDBAR]	= IMG_Load("res/hudbar.png");
	texture[TEXTURE_BRICK]	= IMG_Load("res/brick.png");
	texture[TEXTURE_STONE]	= IMG_Load("res/stone.png");
	texture[TEXTURE_DARK]	= IMG_Load("res/darkbrickbig.png");

	float ticks = SDL_GetTicks();
	float lastTicks;
	float dt;

	ItemFunction itemFunc;
	SetItemFunctions(&itemFunc);

	Player player;
	Player__INIT(&player, &itemFunc);

	Map map;
	parseMap(&map, player.map);

	ObjectFunction objFunc;
	SetObjectFunctions(&objFunc);

	int objects = map.spawns;
	Object object[objects];

	for (int i = 0; i < objects; i++) {
		object[i] = CreateObject(map.spawn[i].z, &objFunc);
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
		dt = (ticks - lastTicks) / 1000.f;

		Player__UPDATE(&player, &map, dt);

		for (int i = 0; i < objects; i++) {
			// Update Objects
			if (!object[i].destroy) {
				if (object[i].type > OBJECT_NONE && object[i].type < OBJECT_TYPES)
					object[i].update(&object[i], dt, &player, map, view);
				ObjectGlobalUPDATE(&object[i], player, map, dt);
			}
			else // Remove Objects
				DestroyObject(&object[i]);			
		}

		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
		SDL_RenderClear(renderer);

		drawBackground(renderer, view);
		drawForeground(renderer, texture, view, objects, object, player, map);
		drawHUD(renderer, texture, view, player);

		SDL_RenderPresent(renderer);
	}
	
	save(&player);

	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	IMG_Quit();
	SDL_Quit();
	return 0;
}

int WinMain(int argc, char** argv) {
	return main(argc, argv);
}