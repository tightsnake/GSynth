/*

Author: Chris Lefkarites

 */

#include "mixer.h"

#include <SDL2/SDL.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <math.h>


#define RATE 44100
#define CHANNELS 1
#define SIZE (RATE * CHANNELS) /* How many <data type> are in the sample. */

#define PI 3.1415926

#define ZOOM 1

#define VOICES 15

static Uint8 * head [VOICES], * end [VOICES];

/* Global audio buffer which holds our current oscillator shape. */
static Sint16 sound [VOICES][SIZE], buffer [SIZE];
static SDL_Point points[RATE] = { 0 };

static int overlap, underlap;

static char octave = 0;

static int SCREEN_WIDTH = 1200;
static int SCREEN_HEIGHT = 200;

static const Tone FREQ[VOICES] = {
	C, Cs, D, Ds, E, F, Fs, G, Gs, A, As, B, 2*C, 2*Cs, 2*D
};

static int getVoiceIndex(SDL_Keycode * key) {
	switch(*key){
		case SDLK_a:
			return 0;
			break;
		case SDLK_w:
			return 1;
			break;
		case SDLK_s:
			return 2;
			break;
		case SDLK_e:
			return 3;
			break;
		case SDLK_d:
			return 4;
			break;
		case SDLK_f:
			return 5;
			break;
		case SDLK_t:
			return 6;
			break;
		case SDLK_g:
			return 7;
			break;
		case SDLK_y:
			return 8;
			break;
		case SDLK_h:
			return 9;
			break;
		case SDLK_u:
			return 10;
			break;
		case SDLK_j:
			return 11;
			break;
		case SDLK_k:
			return 12;
			break;
		case SDLK_o:
			return 13;
			break;
		case SDLK_l:
			return 14;
			break;
		default:
			return -1;
			break;
	}
}

static void initVoices(){

	for(int i = 0; i < VOICES; i++) {
		head[i] = (Uint8*) sound[i];
		end[i] = (Uint8*) &sound[i][0] + SIZE * sizeof(Sint16);
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

	while(key = getKeys(keyMap)){

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

int main(int argc, char* argv[]) {

	SDL_Window * window;
	SDL_Renderer * renderer;
	SDL_Event event;
	SDL_AudioSpec spec, obtained;
	SDL_AudioDeviceID device;

	SDL_Point point, prevpoint;

	int running = 1;

	int dcount;

	initVoices();
	prepareVoices();

	keyMap = createKeyMap( MAPSIZE );

	createSpec(&spec, RATE, AUDIO_S16LSB, CHANNELS, 2048, mixBuffer);

	if(SDL_Init(SDL_INIT_VIDEO)){

		printf("SDL could not initialize video: %s\n", SDL_GetError());
		//return EXIT_FAILURE;

	}

	if(SDL_Init(SDL_INIT_AUDIO)){

		printf("SDL could not initialize audio: %s\n", SDL_GetError());
		//return EXIT_FAILURE;

	}

	dcount = SDL_GetNumAudioDevices(0);
	for(int i = 0; i < dcount; i++){
		printf("Audio device %d: %s\n", i, SDL_GetAudioDeviceName(i,0));
	}

	device = SDL_OpenAudio(&spec, NULL);

	if(device){

		printf("Could not open audio device: %s\n", SDL_GetError());
		//return EXIT_FAILURE;

	}

	window = SDL_CreateWindow(
			"GSynth",
			SDL_WINDOWPOS_UNDEFINED,
			SDL_WINDOWPOS_UNDEFINED,
			SCREEN_WIDTH,
			SCREEN_HEIGHT,
			SDL_WINDOW_OPENGL
			);

	if (window == NULL) {

		printf("Could not create window: %s\n", SDL_GetError());
		return EXIT_FAILURE;

	}

	SDL_SetWindowResizable( window, SDL_TRUE );

	renderer = SDL_CreateRenderer( window, -1, SDL_RENDERER_ACCELERATED);
	if(renderer == NULL){

		printf("Could not create renderer: %s\n", SDL_GetError());
		return EXIT_FAILURE;

	}

	while(running){

		while(SDL_PollEvent(&event)){

			if(event.type == SDL_QUIT){

				running = 0;

			}

			if(event.type == SDL_KEYDOWN){

				switch(event.key.keysym.sym){

					case SDLK_l:
					case SDLK_o:
					case SDLK_k:
					case SDLK_j:
					case SDLK_u:
					case SDLK_h:
					case SDLK_y:
					case SDLK_g:
					case SDLK_t:
					case SDLK_f:
					case SDLK_d:
					case SDLK_e:
					case SDLK_s:
					case SDLK_w:
					case SDLK_a:

						insertKey(keyMap, event.key.keysym.sym);
						SDL_PauseAudio(0);
						break;

					case SDLK_z:

						octave--;
						prepareVoices();
						break;

					case SDLK_x:

						octave++;
						prepareVoices();
						break;

					case SDLK_ESCAPE:

						running = 0;
						break;

					default:
						break;

				}
				//printKeys(keyMap);
			}

			if(event.type == SDL_KEYUP){

				switch(event.key.keysym.sym){

					case SDLK_l:
					case SDLK_o:
					case SDLK_k:
					case SDLK_j:
					case SDLK_u:
					case SDLK_h:
					case SDLK_y:
					case SDLK_g:
					case SDLK_t:
					case SDLK_f:
					case SDLK_d:
					case SDLK_e:
					case SDLK_s:
					case SDLK_w:
					case SDLK_a:
						removeKey(keyMap, event.key.keysym.sym);
						if(!keyMap->count) SDL_PauseAudio(1);	
						break;

					default:
						break;

				}
				//printKeys(keyMap);
			}

			if(event.type == SDL_WINDOWEVENT && event.window.event == SDL_WINDOWEVENT_RESIZED){

				SCREEN_WIDTH = event.window.data1;
				SCREEN_HEIGHT = event.window.data2;
				SDL_SetWindowSize( window, SCREEN_WIDTH, SCREEN_HEIGHT );

			}

		}

		SDL_SetRenderDrawColor( renderer, 0x00, 0x00, 0x00, 0xFF );
		SDL_RenderClear( renderer );

		/* Draw Wave Form.
		   SDL_SetRenderDrawColor( renderer, 0xAA, 0x00, 0xFF, 0xFF );
		   SDL_RenderDrawLines( renderer, points, RATE );
		 */

		/* Draw a track head.
		   point.x = SCREEN_WIDTH * (head - (Uint8*) &sound[0]) / (SIZE * sizeof(Sint16));
		   SDL_SetRenderDrawColor( renderer, 0xFF, 0xFF, 0xFF, 0xFF );
		   SDL_RenderDrawLine( renderer, point.x, 0, point.x, SCREEN_HEIGHT); 
		 */

		SDL_RenderPresent( renderer );

	}

	SDL_CloseAudio();
	SDL_DestroyWindow(window);

	SDL_Quit();
	return EXIT_SUCCESS;

}
