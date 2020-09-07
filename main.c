/*

Author: Chris Lefkarites

 */

#include "gsynth.h"


int main(int argc, char* argv[]) {

	SDL_Window * window;
	SDL_Renderer * renderer;
	SDL_Event event;
	SDL_AudioSpec spec;
	SDL_AudioDeviceID device;

	int running = 1;

	int dcount;

	initVoices();
	prepareVoices();

	keyMap = createKeyMap( MAPSIZE );

	createSpec(&spec, RATE, AUDIO_S16LSB, CHANNELS, 4096, mixBuffer);

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

	fprintf(stderr, "Audio Specification : samples %d channels %d\n", spec.samples, spec.channels);

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

						/* TODO - Logic Block in Question. */

						insertKey(keyMap, event.key.keysym.sym);
						
						printKeys(keyMap);

						if(keyMap->lastCount != keyMap->count){

							SDL_PauseAudio(0);
							updateGraphics();
							keyMap->lastCount = keyMap->count;

						}

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

						printKeys(keyMap);

						if(keyMap->lastCount != keyMap->count){

							if(!keyMap->count) SDL_PauseAudio(1);
							updateGraphics();
							keyMap->lastCount = keyMap->count;
						
						}

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

		/* Draw Wave Form. */
		SDL_SetRenderDrawColor( renderer, 0x00, 0xFF, 0x00, 0xFF );
		SDL_RenderDrawLines( renderer, points, RATE/ZOOM );

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
