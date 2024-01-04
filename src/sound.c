#include "sound.h"

#include <stdio.h>
#include <string.h>

void loadSounds(Mix_Chunk** sound) {
	int loadedSounds = 0;
	for (int i = 0; i < SOUNDS; i++) {
		// Load Sounds
		char soundPath[15] = "res/sound/";
		char soundNum[2];
		sprintf(soundNum, "%d", i);
		strncat(soundPath, soundNum, 2);
		strcat(soundPath, ".wav");

		sound[loadedSounds] = Mix_LoadWAV(soundPath);
		loadedSounds++;
	}
	if (loadedSounds == SOUNDS)
		printf("loaded all sounds woohoo\n");
	else
		printf("did not load all the sounds, buh\n");
}

void loadMusics(Mix_Music** music) {
	int loadedMusics = 0;
	for (int i = 0; i < MUSICS; i++) {
		// Load Musics
		char musicPath[15] = "res/music/";
		char musicNum[2];
		sprintf(musicNum, "%d", i);
		strncat(musicPath, musicNum, 2);
		strcat(musicPath, ".ogg");

		music[loadedMusics] = Mix_LoadMUS(musicPath);
		loadedMusics++;
	}
	if (loadedMusics == MUSICS)
		printf("we have the music, GYAAAAT\n");
	else
		printf("no music );\n");
}