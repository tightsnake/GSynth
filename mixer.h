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

#define MODULES 2
#define MODULE_OSC 0
#define MODULE_ADSR 1

typedef enum { C = 262, Cs = 277, D = 294, Ds = 311, E = 330,
	F = 349, Fs = 370, G = 392, Gs = 415, A = 440, As = 466, B = 494 } Tone;

static const Tone FREQ[VOICES] = {
	C, Cs, D, Ds, E, F, Fs, G, Gs, A, As, B, 2*C, 2*Cs, 2*D
};

/* KeyNode -

		SDL_Keycode key - the key which hashes to this KeyNode.
		SDL_Color color - the color of the piano key.
		SDL_Color color - the paintColor that should be painted on call to renderGSynth. 
		SDL_Rect rect - the coordinates of the paint location as { x, y, w, h }. 
		int voiceIndex - precomputed index of this KeyNode's piano voice 
			( see GSynth : .head, .end, .sound ). */

typedef struct KeyNode {
	SDL_Keycode key;
	SDL_Color color, paintColor;
	SDL_Rect rect;
	int voiceIndex;
	struct KeyNode * prev, * next;
} KeyNode;

/*	KeyMap - 

		int size - size of hash table (MAPSIZE).
		int count - the number of elements currently inserted into the hash table.
		int lastCount - the count previous to calling a insertKey or removeKey method.
		KeyNode * keys - the hash table, an array of 'size' KeyNode * .
		KeyNode * listTail - the tail of the underlying linked list.
		int hash [VOICES] - pre computed hash LUT for the program keyboard input.

		A hash table of size specified in MAPSIZE (128 for SDL),
		hashes the SDL_Keycode data type into it's respective bin with modulo MAPSIZE.
		Contains an underlying stack like linked list which maintains a pointer to
		the tail node of the linked list. Also contains a pre computed LUT for the
		hash values of the keys the program will use indexed by their 'voiceIndex' 
		( see KeyNode.voiceIndex ). */

typedef struct{
	int size, count, lastCount;
	KeyNode * keys;
	KeyNode * listTail;
	int hash [VOICES];
} KeyMap;

/* GSynth -

	 KeyMap * keyMap - a hash table of size specified in MAPSIZE (128 for SDL),
	 hashes the SDL_Keycode data type into it's respective bin with modulo MAPSIZE.
	 SDL_Point points [RATE] - Deprecated waveform rendering.  TODO - remove ? */

typedef struct{
	KeyMap * keyMap;
	SDL_Point points[RATE];
	int octave;
	Uint8 * head [VOICES], * end [VOICES];
	Sint16 sound [VOICES][SIZE], buffer [SIZE];
	int overlap, underlap;
	int screenWidth;
	int screenHeight;
	int shouldRender;
	int retrigger;
	SDL_Event event;
	KeyNode * piano [VOICES];
	SDL_Rect modules [MODULES];
} GSynth;

/* KeyMap functions. */
KeyMap * initKeyMap( KeyMap * keyMap );
KeyMap * updateKeyMapDim( KeyMap * keyMap, int screenWidth, int screenHeight );
KeyMap * createKeyMap( int size );
int insertKey( KeyMap * keyMap, SDL_Keycode key );
int removeKey( KeyMap * keyMap, SDL_Keycode key );
void printKeys(KeyMap * keyMap);
SDL_Keycode * getKeys(KeyMap * keyMap);
int getVoiceIndex(GSynth * gsynth, SDL_Keycode * key);

/* GSynth functions. */
GSynth * createGSynth();
void initPiano( GSynth * gsynth );
void initVoices(GSynth * gsynth);
void prepareVoices(GSynth * gsynth);
SDL_AudioSpec * createSpec(SDL_AudioSpec * spec, int freq, SDL_AudioFormat format,
		Uint8 channels, Uint16 samples, SDL_AudioCallback callback, void * userdata);
void mixBuffer(void * userdata, Uint8 * stream, int len);
void updateGraphics(GSynth * gsynth);
void renderGSynth( GSynth * gsynth, SDL_Renderer * renderer);

#endif
