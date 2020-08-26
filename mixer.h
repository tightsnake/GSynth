/*

Author: Chris Lefkarites

 */

#include <SDL2/SDL.h>

/* Tone - rooted at C3. */
typedef enum { C = 262, Cs = 277, D = 294, Ds = 311, E = 330,
	F = 349, Fs = 370, G = 392, Gs = 415, A = 440, As = 466, B = 494 } Tone;

static SDL_Keycode lastNoteKey;
