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

#define SCREEN_WIDTH 1200
#define SCREEN_HEIGHT 200

#define RATE 44100
#define CHANNELS 1
#define SIZE (RATE * CHANNELS) /* How many <data type> are in the sample. */

#define FREQ 440

#define PI 3.1415926

#define ZOOM 1

/* Length of the audio buffer in samples (both L & R). */
static Uint8 * head;

/* Global audio buffer which holds our current oscillator shape. */
static Sint16 sound [SIZE], buffer[SIZE];
static SDL_Point points[RATE] = { 0 };

static Uint8 * end = (Uint8*) &sound[0] + SIZE * sizeof(Sint16);
static int overlap, underlap;

static char octave = 0;

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

void sinWAV(int freq){
	
	for(int i = 0; i < SIZE; i += CHANNELS){

		for(int j = 0; j < CHANNELS; j++){

			sound [i+j] = 0x7FFF * sin( 2 * PI * round(pow(2,octave) * freq) * i / SIZE);

		}

	}
	
	//TODO - assign SDL_Point array.

	for(int i = 0; i < RATE; i++){
		
			points[i].x = SCREEN_WIDTH * i / (RATE / ZOOM);
			points[i].y = SCREEN_HEIGHT * (sound[CHANNELS*i] + (int) 0x00007FFF) / (int) 0x0000FFFF;

	}

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

					case SDLK_l:
						
						sinWAV(2*D);
						SDL_PauseAudio(0);
						lastNoteKey = event.key.keysym.sym;
						break;

					case SDLK_o:
						
						sinWAV(2*Cs);
						SDL_PauseAudio(0);
						lastNoteKey = event.key.keysym.sym;
						break;

					case SDLK_k:
						
						sinWAV(2*C);
						SDL_PauseAudio(0);
						lastNoteKey = event.key.keysym.sym;
						break;

					case SDLK_j:
						
						sinWAV(B);
						SDL_PauseAudio(0);
						lastNoteKey = event.key.keysym.sym;
						break;

					case SDLK_u:
						
						sinWAV(As);
						SDL_PauseAudio(0);
						lastNoteKey = event.key.keysym.sym;
						break;

					case SDLK_h:
						
						sinWAV(A);
						SDL_PauseAudio(0);
						lastNoteKey = event.key.keysym.sym;
						break;

					case SDLK_y:
						
						sinWAV(Gs);
						SDL_PauseAudio(0);
						lastNoteKey = event.key.keysym.sym;
						break;

					case SDLK_g:
						
						sinWAV(G);
						SDL_PauseAudio(0);
						lastNoteKey = event.key.keysym.sym;
						break;

					case SDLK_t:
						
						sinWAV(Fs);
						SDL_PauseAudio(0);
						lastNoteKey = event.key.keysym.sym;
						break;

					case SDLK_f:
						
						sinWAV(F);
						SDL_PauseAudio(0);
						lastNoteKey = event.key.keysym.sym;
						break;

					case SDLK_d:
						
						sinWAV(E);
						SDL_PauseAudio(0);
						lastNoteKey = event.key.keysym.sym;
						break;

					case SDLK_e:
						
						sinWAV(Ds);
						SDL_PauseAudio(0);
						lastNoteKey = event.key.keysym.sym;
						break;

					case SDLK_s:
						
						sinWAV(D);
						SDL_PauseAudio(0);
						lastNoteKey = event.key.keysym.sym;
						break;

					case SDLK_w:
						
						sinWAV(Cs);
						SDL_PauseAudio(0);
						lastNoteKey = event.key.keysym.sym;
						break;

					case SDLK_a:
						
						sinWAV(C);
						SDL_PauseAudio(0);
						lastNoteKey = event.key.keysym.sym;
						break;

					case SDLK_z:

						octave--;
						break;

					case SDLK_x:

						octave++;
						break;

					default:
						break;

				}
			}

			if(event.type == SDL_KEYUP){

				switch(event.key.keysym.sym){

					case SDLK_a:
					case SDLK_w:
					case SDLK_s:
					case SDLK_e:
					case SDLK_d:
					case SDLK_f:
					case SDLK_t:
					case SDLK_g:
					case SDLK_y:
					case SDLK_h:
					case SDLK_u:
					case SDLK_j:
					case SDLK_k:
					case SDLK_o:
					case SDLK_l:
						if(lastNoteKey == event.key.keysym.sym) SDL_PauseAudio(1);
						break;

					default:
						break;

				}
			}
		}
	
		SDL_SetRenderDrawColor( renderer, 0x00, 0x00, 0x00, 0xFF );

		SDL_RenderClear( renderer );
		
		SDL_SetRenderDrawColor( renderer, 0xAA, 0x00, 0xFF, 0xFF );

		SDL_RenderDrawLines( renderer, points, RATE );

		/* Draw a track head. */
		point.x = SCREEN_WIDTH * (head - (Uint8*) &sound[0]) / (SIZE * sizeof(Sint16));
		
		SDL_SetRenderDrawColor( renderer, 0xFF, 0xFF, 0xFF, 0xFF );
		SDL_RenderDrawLine( renderer, point.x, 0, point.x, SCREEN_HEIGHT); 

		SDL_RenderPresent( renderer );
	}

	SDL_CloseAudio();
	SDL_DestroyWindow(window);

	SDL_Quit();
	return EXIT_SUCCESS;

}
