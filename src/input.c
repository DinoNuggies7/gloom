#include "input.h"

void handleInput(SDL_Event* event, Player* player) {
	while (SDL_PollEvent(event)) {
		if (event->type == SDL_WINDOWEVENT_CLOSE)
			player->quit = true;
		if (event->type == SDL_MOUSEMOTION)
			player->xrel = event->motion.xrel;
		if (event->type == SDL_MOUSEBUTTONDOWN) {
			switch (event->button.button) {
				case SDL_BUTTON_LEFT:
					player->dig = true;
					break;
				case SDL_BUTTON_RIGHT:
					player->build = true;
					break;
			}
		}
		if (event->type == SDL_MOUSEBUTTONUP) {
			switch (event->button.button) {
				case SDL_BUTTON_LEFT:
					player->dig = false;
					break;
				case SDL_BUTTON_RIGHT:
					player->build = false;
					break;
			}
		}
		if (event->type == SDL_KEYDOWN) {
			switch (event->key.keysym.scancode) {
				case SDL_SCANCODE_ESCAPE:
					player->quit = true;
					break;
				case SDL_SCANCODE_W:
					player->forward = true;
					break;
				case SDL_SCANCODE_S:
					player->backword = true;
					break;
				case SDL_SCANCODE_A:
					player->left = true;
					break;
				case SDL_SCANCODE_D:
					player->right = true;
					break;
				case SDL_SCANCODE_UP:
					player->lookup = true;
				case SDL_SCANCODE_DOWN:
					player->lookdown = true;
				case SDL_SCANCODE_LEFT:
					player->lookleft = true;
					break;
				case SDL_SCANCODE_RIGHT:
					player->lookright = true;
					break;
			}
		}
		else if (event->type == SDL_KEYUP) {
			switch (event->key.keysym.scancode) {
				case SDL_SCANCODE_W:
					player->forward = false;
					break;
				case SDL_SCANCODE_S:
					player->backword = false;
					break;
				case SDL_SCANCODE_A:
					player->left = false;
					break;
				case SDL_SCANCODE_D:
					player->right = false;
					break;
				case SDL_SCANCODE_UP:
					player->lookup = false;
				case SDL_SCANCODE_DOWN:
					player->lookdown = false;
				case SDL_SCANCODE_LEFT:
					player->lookleft = false;
					break;
				case SDL_SCANCODE_RIGHT:
					player->lookright = false;
					break;
			}
		}
	}
}