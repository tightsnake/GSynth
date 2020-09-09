/*

Author: Chris Lefkarites

 */

#ifndef MIXER_H
#define MIXER_H

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

#define MAPSIZE 128

#define SCREEN_WIDTH 1200
#define SCREEN_HEIGHT 400

typedef enum { C = 262, Cs = 277, D = 294, Ds = 311, E = 330,
	F = 349, Fs = 370, G = 392, Gs = 415, A = 440, As = 466, B = 494 } Tone;

static const Tone FREQ[VOICES] = {
	C, Cs, D, Ds, E, F, Fs, G, Gs, A, As, B, 2*C, 2*Cs, 2*D
};

typedef struct KeyNode {
	SDL_Keycode key;
	SDL_Color color;
	SDL_Rect rect;
	int voiceIndex;
	struct KeyNode * prev, * next;
} KeyNode;

typedef struct{
	int size, count, lastCount;
	KeyNode * keys;
	KeyNode * listTail;
} KeyMap;

typedef struct{
	KeyMap * keyMap;
	SDL_Point points[RATE];
	char octave;
	Uint8 * head [VOICES], * end [VOICES];
	Sint16 sound [VOICES][SIZE], buffer [SIZE];
	int overlap, underlap;
	int screenWidth;
	int screenHeight;
} GSynth;

/* KeyMap functions. */
KeyMap * initKeyMap( KeyMap * keyMap);
KeyMap * createKeyMap( int size );
int insertKey( KeyMap * keyMap, SDL_Keycode key );
int removeKey( KeyMap * keyMap, SDL_Keycode key );
void printKeys(KeyMap * keyMap);
SDL_Keycode * getKeys(KeyMap * keyMap);
int getVoiceIndex(GSynth * gsynth, SDL_Keycode * key);

/* GSynth functions. */
GSynth * createGSynth();
void initVoices(GSynth * gsynth);
void prepareVoices(GSynth * gsynth);
SDL_AudioSpec * createSpec(SDL_AudioSpec * spec, int freq, SDL_AudioFormat format,
		Uint8 channels, Uint16 samples, SDL_AudioCallback callback, void * userdata);
void mixBuffer(void * userdata, Uint8 * stream, int len);
void updateGraphics(GSynth * gsynth);

#endif
