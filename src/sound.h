#ifndef SOUND_H
#define SOUND_H

#include <stdbool.h>
#include <SDL2/SDL_mixer.h>

enum SoundID {
	SOUND_JUMPSCARE,

	SOUNDS,
};

enum MusicID {
	MUSIC_AMBIENCE,

	MUSICS,
};

void loadSounds(Mix_Chunk** sound);
void loadMusics(Mix_Music** music);

#endif