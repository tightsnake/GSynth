/*

Author: Chris Lefkarites

 */
#include <stdio.h>

#include <SDL2/SDL.h>

#define MAPSIZE 128

/* Tone - rooted at C3. */
typedef enum { C = 262, Cs = 277, D = 294, Ds = 311, E = 330,
	F = 349, Fs = 370, G = 392, Gs = 415, A = 440, As = 466, B = 494 } Tone;

typedef struct KeyNode {
	SDL_Keycode key;
	struct KeyNode * prev, * next;
} KeyNode;

typedef struct{
	int size, count;
	KeyNode * keys;
	KeyNode * listTail;
} KeyMap;

static KeyMap * createKeyMap( int size ){

	KeyMap * keyMap;
	keyMap = (KeyMap*) calloc( 1, sizeof(KeyMap) );
	keyMap->keys = (KeyNode*) calloc( size, sizeof(KeyNode) );
	keyMap->size = size;

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
	printf("count: %d\n",keyMap->count);

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
	printf("count: %d\n",keyMap->count);

	return 0;

}

static void printKeys(KeyMap * keyMap){
	KeyNode * listTail = keyMap->listTail;
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

}

static KeyMap * keyMap;
