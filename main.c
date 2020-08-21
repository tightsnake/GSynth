/*

Author: Chris Lefkarites

*/

#include <SDL2/SDL.h>

#include <stdio.h>
#include <stdlib.h>

#define SCREEN_WIDTH 600
#define SCREEN_HEIGHT 200

void createSpec(SDL_AudioSpec * spec, int freq, SDL_AudioFormat format,
	Uint8 channels, Uint8 silence, Uint16 samples, Uint32 size, SDL_AudioCallback callback){

	//TODO
    return;

}

int main(int argc, char* argv[]) {

    SDL_Window * window;
    SDL_Event event;
    SDL_AudioSpec spec;

    int running = 1;

    SDL_Init(SDL_INIT_VIDEO);

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
            switch(event.type){
                case SDL_QUIT:
                case SDL_KEYDOWN:
                    running = 0;
                break;
                default:
                break;
            }
        }
    }

    SDL_DestroyWindow(window);

    SDL_Quit();
    return 0;

}
