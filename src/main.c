#include "input.h"
#include "draw.h"
#include "map.h"
#include "object.h"
#include "player.h"

#include <SDL2/SDL.h>

SDL_Window* window;
SDL_Renderer* renderer;

int main(int argc, char **argv) {
	SDL_Init(SDL_INIT_EVERYTHING);
	IMG_Init(IMG_INIT_PNG);

	SDL_DisplayMode display;
	SDL_GetCurrentDisplayMode(0, &display);

	window = SDL_CreateWindow("doom'd", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, display.w, display.h, SDL_WINDOW_ALLOW_HIGHDPI | SDL_WINDOW_FULLSCREEN_DESKTOP);
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	SDL_SetRelativeMouseMode(true);

	float ticks = SDL_GetTicks();
	float lastTicks;
	float dt;

	Player player;
	Player__INIT(&player);

	int objects = 1;
	Object object[objects];
	object[0] = CreateObject("Derk");
	// object[1] = CreateObject("Derk");

	for (int i = 0; i < objects; i++) {
		Object__INIT(&object[i]);
		object[i].init(&object[i], renderer);
	}

	object[0].pos.x = 4;
	object[0].pos.y = 5;
	object[1].pos.x = 6;
	object[1].pos.y = 5;

	Map map;
	parseMap(&map, "res/test.txt");

	for (int j = 0; j < map.h; j++) {		// Print map data
		for (int i = 0; i < map.w; i++) {
			if (getTile(map, i, j) == SPAWN) {
				printf("@ ");
				player.pos.x = i;	// Setting player spawn point,
				player.pos.y = j;	//  should move to player.c
			}
			else if (getTile(map, i, j) > 0)
				printf("# ");
			else
				printf(". ");
		}
		printf("\n");
	}

	while (!player.quit) {
		SDL_Event event;
		handleInput(&event, &player);

		lastTicks = ticks;
		ticks = SDL_GetTicks();
		dt = (ticks - lastTicks) / 1000.f;

		Player__UPDATE(&player, map, dt);

		for (int i = 0; i < objects; i++) {
			if (!strcmp(object[i].type, "Derk")) {
				object[i].update(&object[i], dt, &player, map, display);
			}
			Object__UPDATE(&object[i], dt);
		}


		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
		SDL_RenderClear(renderer);

		drawBackground(renderer, display);
		drawObjects(renderer, display, objects, object, player);
		drawWalls(renderer, display, objects, &object[0], player, map);

		SDL_RenderPresent(renderer);
	}

	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	IMG_Quit();
	SDL_Quit();
	return 0;
}