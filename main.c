/*

Author: Chris Lefkarites

 */

#include <SDL2/SDL.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <math.h>

#define SCREEN_WIDTH 1200
#define SCREEN_HEIGHT 400

#define RATE 44100
#define CHANNELS 1
#define SIZE (RATE * CHANNELS) /* How many <data type> are in the sample. */

#define FREQ 72

#define PI 3.1415926

#define ZOOM 1

/* Length of the audio buffer in samples (both L & R). */
static Uint8 * head;

/* Global audio buffer which holds our current oscillator shape. */
static Sint16 sound [SIZE], buffer[SIZE];

static Uint8 * end = (Uint8*) &sound[0] + SIZE * sizeof(Sint16);
static int overlap, underlap;

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

	overlap = end - head;
	underlap = head + len - end;

	memset(stream, 0, len);

	if(head + len > end){

		memcpy((Uint8*) buffer, head, overlap);
		memcpy((Uint8*) &buffer[0] + overlap, (Uint8*) sound, underlap);
		head = (Uint8*) &sound[0] + underlap;
		SDL_MixAudio(stream, (Uint8*) buffer, len, SDL_MIX_MAXVOLUME);

	}else{
	
		SDL_MixAudio(stream, head, len, SDL_MIX_MAXVOLUME);
		head += len;

	}

	//printf("%d - %d - %d - %d\n",end, overlap, underlap, len);

	return;

}

void sinWAV(){
	
	for(int i = 0; i < SIZE; i += CHANNELS){

		for(int j = 0; j < CHANNELS; j++){

			sound [i+j] = 0x7FFF * sin( 2 * PI * FREQ * i / SIZE);

		}

	}

}

void plotWAV(){

	return;	

}

int main(int argc, char* argv[]) {

	SDL_Window * window;
	SDL_Renderer * renderer;
	SDL_Event event;
	SDL_AudioSpec spec, obtained;
	SDL_AudioDeviceID device;
	
	SDL_Point point, prevpoint;
	
	head = (Uint8*) sound;

	int running = 1;

	int dcount;
	
	sinWAV();

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

					case SDLK_a:
						SDL_PauseAudio(0);
						printf("A");
						break;

					default:
						break;

				}
			}

			if(event.type == SDL_KEYUP){

				switch(event.key.keysym.sym){

					case SDLK_a:
						SDL_PauseAudio(1);
						printf("-A");
						break;

					default:
						break;

				}
			}
		}
	
		SDL_SetRenderDrawColor( renderer, 0x00, 0x00, 0x00, 0xFF );

		SDL_RenderClear( renderer );
		
		SDL_SetRenderDrawColor( renderer, 0xFF, 0x00, 0x00, 0xFF );

		prevpoint.x = 0;
		prevpoint.y = SCREEN_HEIGHT / 2;

		/* Render all objects TODO  *//*
		for(int i = 0; i < (RATE / ZOOM); i++){
			
			point.x = SCREEN_WIDTH * i / (RATE / ZOOM);
			point.y = SCREEN_HEIGHT * (sound[CHANNELS*i] + (int) 0x00007FFF) / (int) 0x0000FFFF;
			SDL_RenderDrawLine( renderer, point.x, point.y, prevpoint.x, prevpoint.y );
			prevpoint = point;

		}

		*//* Draw a track head. */
		

		SDL_RenderPresent( renderer );
	}

	SDL_CloseAudio();
	SDL_DestroyWindow(window);

	SDL_Quit();
	return EXIT_SUCCESS;

}
