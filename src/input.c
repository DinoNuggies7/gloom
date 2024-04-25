#include "input.h"

void handleInput(SDL_Event* event, Player* player) {
	while (SDL_PollEvent(event)) {
		if (event->type == SDL_QUIT || event->type == SDL_WINDOWEVENT_CLOSE)
			player->quit = true;

		if (event->type == SDL_MOUSEMOTION)
			player->xrel = event->motion.xrel;

		if (event->type == SDL_MOUSEWHEEL)
			player->scroll = event->wheel.y;

		if (event->type == SDL_MOUSEBUTTONDOWN) {
			switch (event->button.button) {
				case SDL_BUTTON_LEFT:
					player->leftclick = true;
					break;
				case SDL_BUTTON_RIGHT:
					player->rightclick = true;
					break;
			}
		}
		if (event->type == SDL_MOUSEBUTTONUP) {
			switch (event->button.button) {
				case SDL_BUTTON_LEFT:
					player->leftclick = false;
					break;
				case SDL_BUTTON_RIGHT:
					player->rightclick = false;
					break;
			}
		}

		if (event->type == SDL_KEYDOWN) {
			if (event->key.keysym.scancode >= 30 && event->key.keysym.scancode < 30 + SLOTS) {
				player->select = event->key.keysym.scancode - 30;
				player->choosing = true;
				player->selectTimer = 2;
			}
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
				case SDL_SCANCODE_LSHIFT:
					player->sprint = true;
					break;
				case SDL_SCANCODE_UP:
					player->lookup = true;
					break;
				case SDL_SCANCODE_DOWN:
					player->lookdown = true;
					break;
				case SDL_SCANCODE_LEFT:
					player->lookleft = true;
					break;
				case SDL_SCANCODE_RIGHT:
					player->lookright = true;
					break;
				default:
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
				case SDL_SCANCODE_LSHIFT:
					player->sprint = false;
					break;
				case SDL_SCANCODE_UP:
					player->lookup = false;
					break;
				case SDL_SCANCODE_DOWN:
					player->lookdown = false;
					break;
				case SDL_SCANCODE_LEFT:
					player->lookleft = false;
					break;
				case SDL_SCANCODE_RIGHT:
					player->lookright = false;
					break;
				default:
					break;
			}
		}
	}
}
