/*

Author: Chris Lefkarites

 */

#include "mixer.h"

KeyMap * initKeyMap( KeyMap * keyMap) { 

	SDL_Color white = {0xFF, 0xFF, 0xFF, 0xFF};
	SDL_Color black = {0x00, 0x00, 0x00, 0xFF};

	SDL_Rect whiteKey = { 0, SCREEN_HEIGHT / 2, SCREEN_WIDTH / 9, SCREEN_HEIGHT / 2 };
	SDL_Rect blackKey = { SCREEN_WIDTH / 9 / 2, SCREEN_HEIGHT / 2, SCREEN_WIDTH / 9, SCREEN_HEIGHT / 2 };

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
	blackKey.x += SCREEN_WIDTH / 9;
	keyMap->keys[ SDLK_s % keyMap->size ].rect = whiteKey;
	whiteKey.x += SCREEN_WIDTH / 9;
	keyMap->keys[ SDLK_e % keyMap->size ].rect = blackKey;
	blackKey.x += SCREEN_WIDTH / 9;
	blackKey.x += SCREEN_WIDTH / 9;
	keyMap->keys[ SDLK_d % keyMap->size ].rect = whiteKey;
	whiteKey.x += SCREEN_WIDTH / 9;
	keyMap->keys[ SDLK_f % keyMap->size ].rect = whiteKey;
	whiteKey.x += SCREEN_WIDTH / 9;
	keyMap->keys[ SDLK_t % keyMap->size ].rect = blackKey;
	blackKey.x += SCREEN_WIDTH / 9;
	keyMap->keys[ SDLK_g % keyMap->size ].rect = whiteKey;
	whiteKey.x += SCREEN_WIDTH / 9;
	keyMap->keys[ SDLK_y % keyMap->size ].rect = blackKey;
	blackKey.x += SCREEN_WIDTH / 9;
	keyMap->keys[ SDLK_h % keyMap->size ].rect = whiteKey;
	whiteKey.x += SCREEN_WIDTH / 9;
	keyMap->keys[ SDLK_u % keyMap->size ].rect = blackKey;
	blackKey.x += SCREEN_WIDTH / 9;
	blackKey.x += SCREEN_WIDTH / 9;
	keyMap->keys[ SDLK_j % keyMap->size ].rect = whiteKey;
	whiteKey.x += SCREEN_WIDTH / 9;
	keyMap->keys[ SDLK_k % keyMap->size ].rect = whiteKey;
	whiteKey.x += SCREEN_WIDTH / 9;
	keyMap->keys[ SDLK_o % keyMap->size ].rect = blackKey;
	keyMap->keys[ SDLK_l % keyMap->size ].rect = whiteKey;

	keyMap->keys[ SDLK_a % keyMap->size ].voiceIndex = 0;
	keyMap->keys[ SDLK_w % keyMap->size ].voiceIndex = 1;
	keyMap->keys[ SDLK_s % keyMap->size ].voiceIndex = 2;
	keyMap->keys[ SDLK_e % keyMap->size ].voiceIndex = 3;
	keyMap->keys[ SDLK_d % keyMap->size ].voiceIndex = 4;
	keyMap->keys[ SDLK_f % keyMap->size ].voiceIndex = 5;
	keyMap->keys[ SDLK_t % keyMap->size ].voiceIndex = 6;
	keyMap->keys[ SDLK_g % keyMap->size ].voiceIndex = 7;
	keyMap->keys[ SDLK_y % keyMap->size ].voiceIndex = 8;
	keyMap->keys[ SDLK_h % keyMap->size ].voiceIndex = 9;
	keyMap->keys[ SDLK_u % keyMap->size ].voiceIndex = 10;
	keyMap->keys[ SDLK_j % keyMap->size ].voiceIndex = 11;
	keyMap->keys[ SDLK_k % keyMap->size ].voiceIndex = 12;
	keyMap->keys[ SDLK_o % keyMap->size ].voiceIndex = 13;
	keyMap->keys[ SDLK_l % keyMap->size ].voiceIndex = 14;

	return keyMap;

}

KeyMap * createKeyMap( int size ){

	KeyMap * keyMap;
	keyMap = (KeyMap*) calloc( 1, sizeof(KeyMap) );
	keyMap->keys = (KeyNode*) calloc( size, sizeof(KeyNode) );
	keyMap->size = size;

	initKeyMap( keyMap );

	return keyMap;

}

int insertKey( KeyMap * keyMap, SDL_Keycode key ){

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

int removeKey( KeyMap * keyMap, SDL_Keycode key ){

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

void printKeys(KeyMap * keyMap){
	KeyNode * listTail = keyMap->listTail;
	printf("count %d: ", keyMap->count);
	while(listTail){
		printf("%d ", listTail->key);
		listTail = listTail->prev;
	}
	printf("\n");
}

/* To be called with a while loop statement i.e. while(key = getKeys(keyMap)){} */
SDL_Keycode * getKeys(KeyMap * keyMap){

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

int getVoiceIndex(GSynth * gsynth, SDL_Keycode * key) {
	return gsynth->keyMap->keys[*key % gsynth->keyMap->size].voiceIndex;
}


GSynth * createGSynth() {

	GSynth * gsynth;

	gsynth = (GSynth *) calloc( 1, sizeof(GSynth) );

	gsynth->screenWidth = SCREEN_WIDTH;
	gsynth->screenHeight = SCREEN_HEIGHT;
	
	gsynth->keyMap = createKeyMap( MAPSIZE );
	
	initVoices(gsynth);
	prepareVoices(gsynth);

	return gsynth;

}

void initVoices( GSynth * gsynth ){

	for(int i = 0; i < VOICES; i++) {
		gsynth->head[i] = (Uint8*) gsynth->sound[i];
		gsynth->end[i] = (Uint8*) gsynth->sound[i] + SIZE * sizeof(Sint16);
	}

}

SDL_AudioSpec * createSpec(SDL_AudioSpec * spec, int freq, SDL_AudioFormat format,
		Uint8 channels, Uint16 samples, SDL_AudioCallback callback, void * userdata){

	memset(spec, 0, sizeof(*spec));

	spec->freq = freq;
	spec->format = format;
	spec->channels = channels;
	spec->samples = samples;
	spec->callback = callback;
	spec->userdata = userdata;

	return spec;

}

/* Circularly mixed audio buffer. */
void mixBuffer(void * userdata, Uint8 * stream, int len){

	SDL_Keycode * key;
	int voiceIndex;

	GSynth * gsynth = (GSynth *) userdata;

	memset(stream, 0, len);

	while((key = getKeys(gsynth->keyMap))){

		voiceIndex = getVoiceIndex(gsynth, key);

		gsynth->overlap = gsynth->end[voiceIndex] - gsynth->head[voiceIndex];
		gsynth->underlap = gsynth->head[voiceIndex] + len - gsynth->end[voiceIndex];

		if(gsynth->head[voiceIndex] + len > gsynth->end[voiceIndex]){

			memcpy((Uint8*) gsynth->buffer, gsynth->head[voiceIndex], gsynth->overlap);
			memcpy((Uint8*) &gsynth->buffer[0] + gsynth->overlap, (Uint8*) gsynth->sound[voiceIndex], gsynth->underlap);
			gsynth->head[voiceIndex] = (Uint8*) &gsynth->sound[voiceIndex][0] + gsynth->underlap;
			SDL_MixAudio(stream, (Uint8*) gsynth->buffer, len, SDL_MIX_MAXVOLUME / gsynth->keyMap->count);

		}else{

			SDL_MixAudio(stream, gsynth->head[voiceIndex], len, SDL_MIX_MAXVOLUME / gsynth->keyMap->count);
			gsynth->head[voiceIndex] += len;

		}

	}

	//printf("%d - %d - %d - %d\n",end, overlap, underlap, len);

	return;

}

void prepareVoices( GSynth * gsynth){ //TODO

	for(int k = 0; k < VOICES; k++)

		for(int i = 0; i < SIZE; i += CHANNELS){

			for(int j = 0; j < CHANNELS; j++){

				gsynth->sound [k][i+j] = 0x7FFF * sin( 2 * PI * round(pow(2,gsynth->octave) * FREQ[k]) * i / SIZE);

			}

		}

	//TODO - Mix different signals into a final output.
	/*
		 for(int i = 0; i < RATE; i++){

		 points[i].x = gsynth->screenWidth * i / (RATE / ZOOM);
		 points[i].y = gsynth->screenHeight * (sound[voiceIndex][CHANNELS*i] + (int) 0x00007FFF) / (int) 0x0000FFFF;

		 }
	 */

}

void updateGraphics( GSynth * gsynth ){

	SDL_Keycode * key;
	int voiceIndex;

	memset( gsynth->points, 0, sizeof(SDL_Point) * RATE);

	while((key = getKeys(gsynth->keyMap))) {

		voiceIndex = getVoiceIndex(gsynth, key);

		for(int i = 0; i < (RATE / ZOOM); i++){

			/* TODO - Fix the math and drawing bug. */

			gsynth->points[i].x = gsynth->screenWidth * i / (RATE / ZOOM);
			gsynth->points[i].y += ((float) gsynth->screenHeight / (float) gsynth->keyMap->count)
				* (((int) gsynth->sound[voiceIndex][CHANNELS*i] + (int) 0x00008000) / (float) 0x0000FFFF);

		}

	}

	return;

}

