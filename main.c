/*

Author: Chris Lefkarites

*/

#include <SDL2/SDL.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

#define SCREEN_WIDTH 600
#define SCREEN_HEIGHT 200

static Uint32 length = 441000;
static Uint8 * position;

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

void generateSound(void * userdata, Uint8 * stream, int len){

	memset(stream, 0, len);

	//if(length == 0) return;

	//len = (len > length ? length : len);
	SDL_MixAudio(stream, position, len, SDL_MIX_MAXVOLUME);
	
	//position += len;
	//length -= len;

}

int main(int argc, char* argv[]) {

	short sound [length];

	position = sound;

	for(int i = 0; i < length; i+=2){
		if(i % 200 < 100) sound[i] = sound[i+1] = 0x8000;
		else sound[i] = sound[i+1] = 0x7FFF;
	}

    SDL_Window * window;
    SDL_Event event;
    SDL_AudioSpec spec, obtained;
	SDL_AudioDeviceID device;

    int running = 1;

	int dcount;

	createSpec(&spec, 44100, AUDIO_S16LSB, 2, 512, generateSound);

    if(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO)){
	
		printf("SDL could not initialize: %s/n", SDL_GetError());
		return EXIT_FAILURE;

	}

	dcount = SDL_GetNumAudioDevices(0);
	for(int i = 0; i < dcount; i++){
		printf("Audio device %d: %s\n", i, SDL_GetAudioDeviceName(i,0));
	}
	
	device = SDL_OpenAudio(&spec, NULL);

	if(device){

		printf("Could not open audio device: %s\n", SDL_GetError());
		return EXIT_FAILURE;

	}
/*
	SDL_PauseAudio(0);

	while(length > 0){
		SDL_Delay(100);
	}
*/

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
        return 1;

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
    }

	SDL_CloseAudio();
    SDL_DestroyWindow(window);

    SDL_Quit();
    return 0;

}
