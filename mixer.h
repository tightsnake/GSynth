/*

Author: Chris Lefkarites

 */

#include <SDL2/SDL.h>

#define MAPSIZE 128

/* Tone - rooted at C3. */
typedef enum { C = 262, Cs = 277, D = 294, Ds = 311, E = 330,
	F = 349, Fs = 370, G = 392, Gs = 415, A = 440, As = 466, B = 494 } Tone;

typedef struct Node {
	SDL_Keycode key;
	struct Node * prev, * next;
} Node;

typedef struct{
	int size;
	Node * keys;
	Node * listTail;
} KeyMap;

KeyMap * createKeyMap( int size ){

	KeyMap * keyMap;
	keyMap = (KeyMap*) calloc( 1, sizeof(KeyMap) );
	keyMap->keys = (Node*) calloc( size, sizeof(Node) );
	keyMap->size = size;

	return keyMap;

}

int insertKey( KeyMap * keyMap, SDL_Keycode key ){

	int hash = key % keyMap->size;
	Node * node = &keyMap->keys[hash];

	/* Do not allow a duplicate insertion or key value 0. */
	if(node->key || !key ) return -1;

	node->key = key;
	
	if(keyMap->listTail){

		keyMap->listTail->next = node;
		node->prev = keyMap->listTail;

	}

	keyMap->listTail = node;

	return 0;

}

void removeKey( KeyMap * keyMap, SDL_Keycode key ){
	
	int hash = key % keyMap->size;
	Node * node = &keyMap->keys[hash];

	if(node == keyMap->listTail) {

		keyMap->listTail = keyMap->listTail->prev;
		if(keyMap->listTail) keyMap->listTail->next = 0;

	}else{

		if(node->prev) node->prev->next = node->next;
		if(node->next) node->next->prev = node->prev;

	}
		
	node->key = node->next = node->prev = 0;

	return;

}

static KeyMap * keyMap;
