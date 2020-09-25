/*

Author: Chris Lefkarites

 */

#include "mixer.h"

int main(int argc, char* argv[]) {

	SDL_Window * window;
	SDL_Renderer * renderer;
	SDL_AudioSpec spec;
	SDL_AudioDeviceID device;

	GSynth * gsynth = createGSynth();

	int running = 1;

	int dcount;

	/* Pass along the GSynth object as the Audio Spec. user data. */

	createSpec(&spec, RATE, AUDIO_S16LSB, CHANNELS, 4096, mixBuffer, gsynth);

	if(SDL_Init(SDL_INIT_VIDEO)){

		printf("SDL could not initialize video: %s\n", SDL_GetError());
		return EXIT_FAILURE;

	}

	if(SDL_Init(SDL_INIT_AUDIO)){

		printf("SDL could not initialize audio: %s\n", SDL_GetError());
		return EXIT_FAILURE;

	}

	dcount = SDL_GetNumAudioDevices(0);
	for(int i = 0; i < dcount; i++){
		printf("Audio device %d: %s\n", i, SDL_GetAudioDeviceName(i,0));
	}

	device = SDL_OpenAudio(&spec, NULL);

	fprintf(stderr, "Audio Specification : samples %d channels %d\n", spec.samples, spec.channels);

	if(device){

		printf("Could not open audio device: %s\n", SDL_GetError());
		return EXIT_FAILURE;

	}

	window = SDL_CreateWindow(
			"GSynth",
			SDL_WINDOWPOS_UNDEFINED,
			SDL_WINDOWPOS_UNDEFINED,
			gsynth->screenWidth,
			gsynth->screenHeight,
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

		while(SDL_PollEvent(&gsynth->event)){

			if(gsynth->event.type == SDL_QUIT){

				running = 0;

			}

			if(gsynth->event.type == SDL_KEYDOWN){

				switch(gsynth->event.key.keysym.sym){

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

						insertKey(gsynth->keyMap, gsynth->event.key.keysym.sym);


						if(gsynth->keyMap->lastCount != gsynth->keyMap->count){

							SDL_PauseAudio(0);
							updateGraphics(gsynth);
							gsynth->keyMap->lastCount = gsynth->keyMap->count;

						}

						break;

					case SDLK_z:

						gsynth->octave--;
						prepareVoices(gsynth);
						break;

					case SDLK_x:

						gsynth->octave++;
						prepareVoices(gsynth);
						break;

					case SDLK_ESCAPE:

						running = 0;
						break;

					default:
						break;

				}
			}

			if(gsynth->event.type == SDL_KEYUP){

				switch(gsynth->event.key.keysym.sym){

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

						removeKey(gsynth->keyMap, gsynth->event.key.keysym.sym);

						if(gsynth->keyMap->lastCount != gsynth->keyMap->count){

							if(!gsynth->keyMap->count) SDL_PauseAudio(1);
							updateGraphics(gsynth);
							gsynth->keyMap->lastCount = gsynth->keyMap->count;

						}

						break;

					default:
						break;

				}
			}

			if(gsynth->event.type == SDL_WINDOWEVENT && gsynth->event.window.event == SDL_WINDOWEVENT_RESIZED){

				gsynth->screenWidth = gsynth->event.window.data1;
				gsynth->screenHeight = gsynth->event.window.data2;
				SDL_SetWindowSize( window, gsynth->screenWidth, gsynth->screenHeight );
				updateKeyMapDim( gsynth->keyMap, gsynth->screenWidth, gsynth->screenHeight );
				gsynth->shouldRender = 1;

			}

		}

		renderGSynth( gsynth, renderer );

	}

	SDL_CloseAudio();
	SDL_DestroyWindow(window);

	SDL_Quit();
	return EXIT_SUCCESS;

}
