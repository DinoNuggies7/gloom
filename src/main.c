#include "input.h"
#include "sound.h"
#include "draw.h"
#include "map.h"
#include "object.h"
#include "player.h"

SDL_Window* window;
SDL_Surface* screen;
SDL_Renderer* renderer;

int main(int argc, char** argv) {
	// Initialize SDL2
	SDL_Init(SDL_INIT_EVERYTHING);
	IMG_Init(IMG_INIT_PNG);
	Mix_OpenAudio(22050, MIX_DEFAULT_FORMAT, 2, 4096);
	bool playMusic = false, playJumpScare = false;

	SDL_DisplayMode display;
	SDL_GetCurrentDisplayMode(0, &display);

	// Create Window
	window = SDL_CreateWindow("GLOOM.EXE", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, display.w, display.h, SDL_WINDOW_FULLSCREEN_DESKTOP | SDL_WINDOW_ALLOW_HIGHDPI);
	SDL_Surface* icon = IMG_Load("res/icon.png");
	SDL_SetWindowIcon(window, icon);
	SDL_SetRelativeMouseMode(true);

	// Create Renderer
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

	// Create Screen Surface
	SDL_Rect windowSize = {0, 0, display.w, display.h};
	SDL_Rect view = {0, 0, 640, 360};
	screen = SDL_CreateRGBSurface(0, view.w, view.h, 32, 0, 0, 0, 0);

	// Load Sounds and Musics
	Mix_Chunk* sound[SOUNDS];
	Mix_Music* music[MUSICS];
	loadSounds(sound);
	loadMusics(music);

	// Load Textures
	SDL_Surface* texture[TEXTURES];
	loadTextures(texture);

	// Variable for frame independence
	float ticks = SDL_GetTicks();
	float lastTicks;
	float dt;

	// Create and initialize Player
	Player player;
	Player__INIT(&player);

	// Create and initialize Map
	Map map;
	parseMap(&map, player.map);

	// Create Objects
	int objects = map.spawns;
	Object object[objects];

	// Initialize Objects
	for (int i = 0; i < objects; i++) {
		if (map.spawn[i].z < 0)
			object[i] = CreateObject(OBJECT_ITEM, abs(map.spawn[i].z));
		else
			object[i] = CreateObject(map.spawn[i].z);
		object[i].pos.x = map.spawn[i].x + 0.5;
		object[i].pos.y = map.spawn[i].y + 0.5;
	}

	// Print the gotten map data (don't really need this anymore)
	for (int j = 0; j < map.h; j++) {		// Print map data
		for (int i = 0; i < map.w; i++) {
			if (getTile(map, i, j) == TILE_SPAWN) {
				printf("@ ");
				player.pos.x = i + 0.5;
				player.pos.y = j + 0.5;
			}
			else if (getTile(map, i, j) > TILE__COLLISION_OBJECT)
				printf("%d ", getTile(map, i, j));
			else
				printf(". ");
		}
		printf("\n");
	}

	// Main Loop
	float endTimer = 0;
	SDL_Event event;
	while (!player.quit) {
		// Get Input
		handleInput(&event, &player);

		// Calculate DeltaTime for frame independence
		lastTicks = ticks;
		ticks = SDL_GetTicks();
		dt = (ticks - lastTicks) / 1000;

		// Scuffed game mode handling
		if (!player.jumpscare && !player.finish) {
			if (!playMusic) Mix_PlayMusic(music[MUSIC_AMBIENCE], -1);
			playMusic = true;

			Player__UPDATE(&player, &map, dt);

			for (int i = 0; i < objects; i++) {
				// Update Objects
				if (!object[i].destroy) {
					if (object[i].type > OBJECT_NONE && object[i].type < OBJECT_TYPES)
						object[i].update(&object[i], dt, view, &player, map);
					ObjectGlobalUPDATE(&object[i], dt, object, objects, &player, map);
				}
				else // Remove Objects
					DestroyObject(&object[i]);
			}
			SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
			SDL_RenderClear(renderer);

			drawBackground(screen, texture, view, player);
			drawForeground(screen, texture, view, player, object, objects, map);
		}
		else if (player.finish) { // If you beat the level
			if (Mix_PlayingMusic()) Mix_HaltMusic();
			playMusic = false;
			if (endTimer < 7)
				SDL_BlitSurface(texture[TEXTURE_YAY], NULL, screen, NULL);
			else if (endTimer < 10) {
				SDL_BlitSurface(texture[TEXTURE_END], NULL, screen, NULL);
				if (!playJumpScare)
					Mix_PlayChannel(-1, sound[SOUND_JUMPSCARE], 0);
				playJumpScare = true;
			}
			else
				player.quit = true;
			endTimer += 1 * dt;
		}
		else if (player.jumpscare) { // If you die
			if (Mix_PlayingMusic()) Mix_HaltMusic();
			playMusic = false; 
			if (!playJumpScare)
				Mix_PlayChannel(-1, sound[SOUND_JUMPSCARE], 0);
			playJumpScare = true;
			if (endTimer > 3)
				player.quit = true;
			endTimer += 1 * dt;
		}
		SDL_Texture* screenTexture = SDL_CreateTextureFromSurface(renderer, screen);
		SDL_RenderCopy(renderer, screenTexture, NULL, NULL);
		SDL_DestroyTexture(screenTexture);

		drawHUD(screen, texture, renderer, windowSize, view, player, dt);

		SDL_RenderPresent(renderer);
	}
	
	// commented cuz item duping lol
	// save(&player);

	// Free up memory
	for (int i = 0; i < SOUNDS; i++)
		Mix_FreeChunk(sound[i]);
	for (int i = 0; i < MUSICS; i++)
		Mix_FreeMusic(music[i]);

	for (int i = 0; i < SLOTS; i++) {
		SDL_FreeSurface(player.inventory[i].texture);
		if (player.inventory[i].texture != player.inventory[i].itemTexture)
			SDL_FreeSurface(player.inventory[i].itemTexture);
	}

	for (int i = 0; i < objects; i++)
		SDL_FreeSurface(object[i].texture);
	for (int i = 0; i < TEXTURES; i++)
		SDL_FreeSurface(texture[i]);

	SDL_FreeSurface(icon);
	SDL_FreeSurface(screen);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	Mix_CloseAudio();
	IMG_Quit();
	SDL_Quit();
	return 0;
}

int WinMain(int argc, char** argv) {
	return main(argc, argv);
}