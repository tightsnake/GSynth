/*

Author: Chris Lefkarites

 */

#include "mixer.h"

KeyMap * initKeyMap( KeyMap * keyMap ) {

	SDL_Color white = {0xFF, 0xFF, 0xFF, 0xFF};
	SDL_Color black = {0x00, 0x00, 0x00, 0xFF};

	/* Pre-Compute the hash values that will be used as input. 
		 0 to 14 : keyboard piano roll. */
	keyMap->hash[0]  = SDLK_a % keyMap->size;
	keyMap->hash[1]  = SDLK_w % keyMap->size;
	keyMap->hash[2]  = SDLK_s % keyMap->size;
	keyMap->hash[3]  = SDLK_e % keyMap->size;
	keyMap->hash[4]  = SDLK_d % keyMap->size;
	keyMap->hash[5]  = SDLK_f % keyMap->size;
	keyMap->hash[6]  = SDLK_t % keyMap->size;
	keyMap->hash[7]  = SDLK_g % keyMap->size;
	keyMap->hash[8]  = SDLK_y % keyMap->size;
	keyMap->hash[9]  = SDLK_h % keyMap->size;
	keyMap->hash[10] = SDLK_u % keyMap->size;
	keyMap->hash[11] = SDLK_j % keyMap->size;
	keyMap->hash[12] = SDLK_k % keyMap->size;
	keyMap->hash[13] = SDLK_o % keyMap->size;
	keyMap->hash[14] = SDLK_l % keyMap->size;

	/* Set every key to it's correct color i.e. white/black. */
	keyMap->keys[ keyMap->hash[0] ].color = white;
	keyMap->keys[ keyMap->hash[1] ].color = black;
	keyMap->keys[ keyMap->hash[2] ].color = white;
	keyMap->keys[ keyMap->hash[3] ].color = black;
	keyMap->keys[ keyMap->hash[4] ].color = white;
	keyMap->keys[ keyMap->hash[5] ].color = white;
	keyMap->keys[ keyMap->hash[6] ].color = black;
	keyMap->keys[ keyMap->hash[7] ].color = white;
	keyMap->keys[ keyMap->hash[8] ].color = black;
	keyMap->keys[ keyMap->hash[9] ].color = white;
	keyMap->keys[ keyMap->hash[10] ].color = black;
	keyMap->keys[ keyMap->hash[11] ].color = white;
	keyMap->keys[ keyMap->hash[12] ].color = white;
	keyMap->keys[ keyMap->hash[13] ].color = black;
	keyMap->keys[ keyMap->hash[14] ].color = white;

	keyMap->keys[ keyMap->hash[0] ].paintColor = white;
	keyMap->keys[ keyMap->hash[1] ].paintColor = black;
	keyMap->keys[ keyMap->hash[2] ].paintColor = white;
	keyMap->keys[ keyMap->hash[3] ].paintColor = black;
	keyMap->keys[ keyMap->hash[4] ].paintColor = white;
	keyMap->keys[ keyMap->hash[5] ].paintColor = white;
	keyMap->keys[ keyMap->hash[6] ].paintColor = black;
	keyMap->keys[ keyMap->hash[7] ].paintColor = white;
	keyMap->keys[ keyMap->hash[8] ].paintColor = black;
	keyMap->keys[ keyMap->hash[9] ].paintColor = white;
	keyMap->keys[ keyMap->hash[10] ].paintColor = black;
	keyMap->keys[ keyMap->hash[11] ].paintColor = white;
	keyMap->keys[ keyMap->hash[12] ].paintColor = white;
	keyMap->keys[ keyMap->hash[13] ].paintColor = black;
	keyMap->keys[ keyMap->hash[14] ].paintColor = white;

	keyMap->keys[ keyMap->hash[0] ].voiceIndex = 0;
	keyMap->keys[ keyMap->hash[1] ].voiceIndex = 1;
	keyMap->keys[ keyMap->hash[2] ].voiceIndex = 2;
	keyMap->keys[ keyMap->hash[3] ].voiceIndex = 3;
	keyMap->keys[ keyMap->hash[4] ].voiceIndex = 4;
	keyMap->keys[ keyMap->hash[5] ].voiceIndex = 5;
	keyMap->keys[ keyMap->hash[6] ].voiceIndex = 6;
	keyMap->keys[ keyMap->hash[7] ].voiceIndex = 7;
	keyMap->keys[ keyMap->hash[8] ].voiceIndex = 8;
	keyMap->keys[ keyMap->hash[9] ].voiceIndex = 9;
	keyMap->keys[ keyMap->hash[10] ].voiceIndex = 10;
	keyMap->keys[ keyMap->hash[11] ].voiceIndex = 11;
	keyMap->keys[ keyMap->hash[12] ].voiceIndex = 12;
	keyMap->keys[ keyMap->hash[13] ].voiceIndex = 13;
	keyMap->keys[ keyMap->hash[14] ].voiceIndex = 14;

	return keyMap;

}

KeyMap * updateKeyMapDim( KeyMap * keyMap, int screenWidth, int screenHeight ){

	int xHop = screenWidth / 9;
	int yHop = screenHeight / 2;

	SDL_Rect whiteKey = { 0, yHop, xHop, yHop };
	SDL_Rect blackKey = { xHop / 2, yHop, xHop, yHop / 2 };

	/* Give it it's proper shape and location. */
	keyMap->keys[ keyMap->hash[0] ].rect = whiteKey;
	whiteKey.x += xHop;
	keyMap->keys[ keyMap->hash[1] ].rect = blackKey;
	blackKey.x += xHop;
	keyMap->keys[ keyMap->hash[2] ].rect = whiteKey;
	whiteKey.x += xHop;
	keyMap->keys[ keyMap->hash[3] ].rect = blackKey;
	blackKey.x += xHop;
	blackKey.x += xHop;
	keyMap->keys[ keyMap->hash[4] ].rect = whiteKey;
	whiteKey.x += xHop;
	keyMap->keys[ keyMap->hash[5] ].rect = whiteKey;
	whiteKey.x += xHop;
	keyMap->keys[ keyMap->hash[6] ].rect = blackKey;
	blackKey.x += xHop;
	keyMap->keys[ keyMap->hash[7] ].rect = whiteKey;
	whiteKey.x += xHop;
	keyMap->keys[ keyMap->hash[8] ].rect = blackKey;
	blackKey.x += xHop;
	keyMap->keys[ keyMap->hash[9] ].rect = whiteKey;
	whiteKey.x += xHop;
	keyMap->keys[ keyMap->hash[10] ].rect = blackKey;
	blackKey.x += xHop;
	blackKey.x += xHop;
	keyMap->keys[ keyMap->hash[11] ].rect = whiteKey;
	whiteKey.x += xHop;
	keyMap->keys[ keyMap->hash[12] ].rect = whiteKey;
	whiteKey.x += xHop;
	keyMap->keys[ keyMap->hash[13] ].rect = blackKey;
	keyMap->keys[ keyMap->hash[14] ].rect = whiteKey;

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

	node->paintColor = (SDL_Color) { 0xFF, 0x0, 0x0, 0xFF };

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

	node->paintColor = node->color;

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
	updateKeyMapDim( gsynth->keyMap, gsynth->screenWidth, gsynth->screenHeight );

	initVoices(gsynth);
	prepareVoices(gsynth);

	initPiano(gsynth);

	gsynth->shouldRender = 1;

	return gsynth;

}

void initPiano( GSynth * gsynth ) {

	/* White Keys should render first. */
	gsynth->piano [0] = &gsynth->keyMap->keys[ gsynth->keyMap->hash[0] ];
	gsynth->piano [1] = &gsynth->keyMap->keys[ gsynth->keyMap->hash[2] ];
	gsynth->piano [2] = &gsynth->keyMap->keys[ gsynth->keyMap->hash[4] ];
	gsynth->piano [3] = &gsynth->keyMap->keys[ gsynth->keyMap->hash[5] ];
	gsynth->piano [4] = &gsynth->keyMap->keys[ gsynth->keyMap->hash[7] ];
	gsynth->piano [5] = &gsynth->keyMap->keys[ gsynth->keyMap->hash[9] ];
	gsynth->piano [6] = &gsynth->keyMap->keys[ gsynth->keyMap->hash[11] ];
	gsynth->piano [7] = &gsynth->keyMap->keys[ gsynth->keyMap->hash[12] ];
	gsynth->piano [8] = &gsynth->keyMap->keys[ gsynth->keyMap->hash[14] ];

	/* Black Keys. */
	gsynth->piano [9] = &gsynth->keyMap->keys[ gsynth->keyMap->hash[1] ];
	gsynth->piano [10] = &gsynth->keyMap->keys[ gsynth->keyMap->hash[3] ];
	gsynth->piano [11] = &gsynth->keyMap->keys[ gsynth->keyMap->hash[6] ];
	gsynth->piano [12] = &gsynth->keyMap->keys[ gsynth->keyMap->hash[8] ];
	gsynth->piano [13] = &gsynth->keyMap->keys[ gsynth->keyMap->hash[10] ];
	gsynth->piano [14] = &gsynth->keyMap->keys[ gsynth->keyMap->hash[13] ];

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

				gsynth->sound [k][i+j] = 0x7FFF * 
					sin( 2 * PI * round(pow(2,gsynth->octave) * FREQ[k]) * i / SIZE);

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

/* TODO */
void updateGraphics( GSynth * gsynth ){

	SDL_Keycode * key;
	int voiceIndex;

	gsynth->shouldRender = 1;

	memset( gsynth->points, 0, sizeof(SDL_Point) * RATE);

	while((key = getKeys(gsynth->keyMap))) {

		voiceIndex = getVoiceIndex(gsynth, key);

		for(int i = 0; i < (RATE / ZOOM); i++){

			/* TODO - Fix the math and drawing bug. */

			gsynth->points[i].x = gsynth->screenWidth * i / (RATE / ZOOM);
			gsynth->points[i].y += (double) ((double) gsynth->screenHeight / 2 / (double) gsynth->keyMap->count)
				* (double) (((int) gsynth->sound[voiceIndex][CHANNELS*i] + (int) 0x00008000) / (double) 0x0000FFFF);

		}


	}

	return;

}

void renderPianoRoll( GSynth * gsynth, SDL_Renderer * renderer ) {

	SDL_Color * color;
	SDL_Rect * rect;

	for(int i = 0; i < VOICES; i++) {

		color = &gsynth->piano[i]->paintColor;
		rect = &gsynth->piano[i]->rect;

		SDL_SetRenderDrawColor( renderer, color->r, color->g, color->b, color->a );
		SDL_RenderFillRect( renderer, rect );
		SDL_SetRenderDrawColor( renderer, 0x00, 0xFF, 0x00, 0xFF );
		SDL_RenderDrawRect( renderer, rect );

	}

}

/* TODO */
void renderGSynth( GSynth * gsynth, SDL_Renderer * renderer){

	/* Render if the shouldRender flag is set. */

	if(gsynth->shouldRender){

		/* Clear the renderer with draw color. */

		SDL_SetRenderDrawColor( renderer, 0x00, 0x00, 0x00, 0xFF );
		SDL_RenderClear( renderer );

		/* Draw Wave Form. */
		SDL_SetRenderDrawColor( renderer, 0x00, 0xFF, 0x00, 0xFF );
		SDL_RenderDrawLines( renderer, gsynth->points, RATE/ZOOM );

		/* Draw Piano Roll. */
		renderPianoRoll( gsynth, renderer );

		/* Draw Modules. */

		SDL_RenderPresent( renderer );

		gsynth->shouldRender = 0;

	}

}
