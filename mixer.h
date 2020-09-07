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

#define ZOOM 5

#define VOICES 15

#define MAPSIZE 128

static Uint8 * head [VOICES], * end [VOICES];

/* Global audio buffer which holds our current oscillator shape. */
static Sint16 sound [VOICES][SIZE], buffer [SIZE];

static SDL_Point points[RATE] = { 0 };

static int overlap, underlap;

static char octave = 0;

static int SCREEN_WIDTH = 1200;
static int SCREEN_HEIGHT = 200;

/* Tone - rooted at C3. */
typedef enum { C = 262, Cs = 277, D = 294, Ds = 311, E = 330,
	F = 349, Fs = 370, G = 392, Gs = 415, A = 440, As = 466, B = 494 } Tone;

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

typedef struct KeyNode {
	SDL_Keycode key;
	SDL_Color color;
	SDL_Rect rect;
	struct KeyNode * prev, * next;
} KeyNode;

typedef struct{
	int size, count, lastCount;
	KeyNode * keys;
	KeyNode * listTail;
} KeyMap;

static KeyMap * initKeyMap( KeyMap * keyMap) { 

	SDL_Color white = {0xFF, 0xFF, 0xFF, 0xFF};
	SDL_Color black = {0x00, 0x00, 0x00, 0xFF};

	SDL_Rect whiteKey = { 0, 0, SCREEN_WIDTH / 9, SCREEN_HEIGHT / 2 };
	SDL_Rect blackKey = { 16, 0, 32, 256 };

	/* Set every key to it's correct color i.e. white/black. */
	keyMap->keys[ SDLK_a % keyMap->size ].color = white;
	keyMap->keys[ SDLK_w % keyMap->size ].color = black;
	keyMap->keys[ SDLK_s % keyMap->size ].color = white;
	keyMap->keys[ SDLK_e % keyMap->size ].color = black;
	keyMap->keys[ SDLK_d % keyMap->size ].color = white;
	keyMap->keys[ SDLK_f % keyMap->size ].color = white;
	keyMap->keys[ SDLK_t % keyMap->size ].color = black;
	keyMap->keys[ SDLK_g % keyMap->size ].color = white;
	keyMap->keys[ SDLK_y % keyMap->size ].color = black;
	keyMap->keys[ SDLK_h % keyMap->size ].color = white;
	keyMap->keys[ SDLK_u % keyMap->size ].color = black;
	keyMap->keys[ SDLK_j % keyMap->size ].color = white;
	keyMap->keys[ SDLK_k % keyMap->size ].color = white;
	keyMap->keys[ SDLK_o % keyMap->size ].color = black;
	keyMap->keys[ SDLK_l % keyMap->size ].color = white;

	/* Give it it's proper shape and location. */
	keyMap->keys[ SDLK_a % keyMap->size ].rect = whiteKey;
	whiteKey.x += SCREEN_WIDTH / 9;
	keyMap->keys[ SDLK_w % keyMap->size ].rect = blackKey;
	keyMap->keys[ SDLK_s % keyMap->size ].rect = whiteKey;
	keyMap->keys[ SDLK_e % keyMap->size ].rect = blackKey;
	keyMap->keys[ SDLK_d % keyMap->size ].rect = whiteKey;
	keyMap->keys[ SDLK_f % keyMap->size ].rect = whiteKey;
	keyMap->keys[ SDLK_t % keyMap->size ].rect = blackKey;
	keyMap->keys[ SDLK_g % keyMap->size ].rect = whiteKey;
	keyMap->keys[ SDLK_y % keyMap->size ].rect = blackKey;
	keyMap->keys[ SDLK_h % keyMap->size ].rect = whiteKey;
	keyMap->keys[ SDLK_u % keyMap->size ].rect = blackKey;
	keyMap->keys[ SDLK_j % keyMap->size ].rect = whiteKey;
	keyMap->keys[ SDLK_k % keyMap->size ].rect = whiteKey;
	keyMap->keys[ SDLK_o % keyMap->size ].rect = blackKey;
	keyMap->keys[ SDLK_l % keyMap->size ].rect = whiteKey;

	return keyMap;

}

static KeyMap * createKeyMap( int size ){

	KeyMap * keyMap;
	keyMap = (KeyMap*) calloc( 1, sizeof(KeyMap) );
	keyMap->keys = (KeyNode*) calloc( size, sizeof(KeyNode) );
	keyMap->size = size;

	initKeyMap( keyMap );

	return keyMap;

}

static int insertKey( KeyMap * keyMap, SDL_Keycode key ){

	int hash = key % keyMap->size;
	KeyNode * node = &keyMap->keys[hash];

	/* Do not allow a duplicate insertion or key value 0. */
	if(node->key || !key ) return -1;

	node->key = key;
	
	if(keyMap->listTail){

		keyMap->listTail->next = node;
		node->prev = keyMap->listTail;

	}

	keyMap->listTail = node;
	keyMap->count++;
	//printf("count: %d\n",keyMap->count);

	return 0;

}

static int removeKey( KeyMap * keyMap, SDL_Keycode key ){
	
	int hash = key % keyMap->size;
	KeyNode * node = &keyMap->keys[hash];

	/* Key is already deleted. 0 reserved to identify empty cells. */
	if(!node->key) return -1;

	if(node == keyMap->listTail) {

		keyMap->listTail = keyMap->listTail->prev;
		if(keyMap->listTail) keyMap->listTail->next = 0;

	}else{

		if(node->prev) node->prev->next = node->next;
		if(node->next) node->next->prev = node->prev;

	}
		
	node->key = node->next = node->prev = 0;

	keyMap->count--;
	//printf("count: %d\n",keyMap->count);

	return 0;

}

static void printKeys(KeyMap * keyMap){
	KeyNode * listTail = keyMap->listTail;
	printf("count %d: ", keyMap->count);
	while(listTail){
		printf("%d ", listTail->key);
		listTail = listTail->prev;
	}
	printf("\n");
}

/* To be called with a while loop statement i.e. while(key = getKeys(keyMap)){} */
static SDL_Keycode * getKeys(KeyMap * keyMap){

	static KeyNode * listTail = 0;
	static int flag = 0;

	if(!flag) {

		flag = 1;
		listTail = keyMap->listTail;
		if(listTail) return &listTail->key;

	}else{

		listTail = listTail->prev;
		if(listTail) return &listTail->key;

	}

	flag = 0;

	return 0;

}

static KeyMap * keyMap;

#endif
