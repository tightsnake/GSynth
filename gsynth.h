#ifndef GSYNTH_H
#define GSYNTH_H

#include "mixer.h"

static void initVoices(){

	for(int i = 0; i < VOICES; i++) {
		head[i] = (Uint8*) sound[i];
		end[i] = (Uint8*) sound[i] + SIZE * sizeof(Sint16);
	}

}

SDL_AudioSpec * createSpec(SDL_AudioSpec * spec, int freq, SDL_AudioFormat format,
		Uint8 channels, Uint16 samples, SDL_AudioCallback callback){

	memset(spec, 0, sizeof(*spec));

	spec->freq = freq;
	spec->format = format;
	spec->channels = channels;
	spec->samples = samples;
	spec->callback = callback;

	return spec;

}

/* Circularly mixed audio buffer. */
void mixBuffer(void * userdata, Uint8 * stream, int len){

	SDL_Keycode * key;
	int voiceIndex;

	memset(stream, 0, len);

	while((key = getKeys(keyMap))){

		voiceIndex = getVoiceIndex(key);

		overlap = end[voiceIndex] - head[voiceIndex];
		underlap = head[voiceIndex] + len - end[voiceIndex];

		if(head[voiceIndex] + len > end[voiceIndex]){

			memcpy((Uint8*) buffer, head[voiceIndex], overlap);
			memcpy((Uint8*) &buffer[0] + overlap, (Uint8*) sound[voiceIndex], underlap);
			head[voiceIndex] = (Uint8*) &sound[voiceIndex][0] + underlap;
			SDL_MixAudio(stream, (Uint8*) buffer, len, SDL_MIX_MAXVOLUME / keyMap->count);

		}else{

			SDL_MixAudio(stream, head[voiceIndex], len, SDL_MIX_MAXVOLUME / keyMap->count);
			head[voiceIndex] += len;

		}

	}

	//printf("%d - %d - %d - %d\n",end, overlap, underlap, len);

	return;

}

void prepareVoices(){ //TODO

	for(int k = 0; k < VOICES; k++)

		for(int i = 0; i < SIZE; i += CHANNELS){

			for(int j = 0; j < CHANNELS; j++){

				sound [k][i+j] = 0x7FFF * sin( 2 * PI * round(pow(2,octave) * FREQ[k]) * i / SIZE);

			}

		}

	//TODO - Mix different signals into a final output.
	/*
	   for(int i = 0; i < RATE; i++){

	   points[i].x = SCREEN_WIDTH * i / (RATE / ZOOM);
	   points[i].y = SCREEN_HEIGHT * (sound[voiceIndex][CHANNELS*i] + (int) 0x00007FFF) / (int) 0x0000FFFF;

	   }
	 */

}

void updateGraphics(){

	SDL_Keycode * key;
	int voiceIndex;

	memset( points, 0, sizeof(SDL_Point) * RATE);

	while((key = getKeys(keyMap))) {

		voiceIndex = getVoiceIndex(key);

		for(int i = 0; i < (RATE / ZOOM); i++){

			/* TODO - Fix the math and drawing bug. */

			points[i].x = SCREEN_WIDTH * i / (RATE / ZOOM);
			points[i].y += ((float) SCREEN_HEIGHT / (float) keyMap->count)
				* (((int)sound[voiceIndex][CHANNELS*i] + (int) 0x00008000) / (float) 0x0000FFFF);

		}

	}

	return;

}

#endif
