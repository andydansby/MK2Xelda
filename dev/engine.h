// MT Engine MK2
// Copyleft 2014 the Mojon Twins

// engine.h
// Well, self explanatory innit?

#ifndef PLAYER_MIN_KILLABLE
#define PLAYER_MIN_KILLABLE 0
#endif

// Animation frames
#include "engine/frames.h"

// Prepare level (compressed levels)
#if defined (SIMPLE_LEVEL_MANAGER)
#include "engine/clevels-s.h"
#elif defined (COMPRESSED_LEVELS)
#include "engine/clevels.h"
#endif

// Init player
#include "engine/initplayer.h"

// Collision
#include "engine/collision.h"

// Random
#include "engine/random.h"

// Messages
#include "engine/messages.h"

#ifdef PLAYER_STEP_SOUND
void step (void) {
	#asm
		ld a, 16
		out (254), a
		nop
		nop
		nop
		nop
		nop
		nop
		nop
		nop
		nop
		xor 16
		out (254), a
	#endasm
}
#endif

//curtain
void cortina (void) {
	#asm
		ld b, 7
	.fade_out_extern
		push bc

		ld   e, 3               ; 3 Thirds
		ld   hl, 22528          ; Attributes start here
	#endasm
#ifdef MODE_128K
	#asm
		halt                    ; We hope to delay.
	#endasm
#endif
	#asm
	.fade_out_bucle
		ld   a, (hl )           ; We bring the current attribute

		ld   d, a               ; Take attribute
		and  7                  ; isolate Ink 
		jr   z, ink_done        ; If it is 0, it is not decremented
		dec  a                  ; Decrement ink
	.ink_done
		ld   b, a               ; In b we now have the ink already processed.

		ld   a, d               ; Take attribute
		and  56                 ; Isolate the paper without changing its position in the byte
		jr   z, paper_done      ; If it is 0, it is not decremented
		sub  8                  ; Decrement paper remaining 8
	.paper_done
		ld   c, a               ; In c we now have the paper already processed.
		ld   a, d
		and  192                ; We were left with bits 6 and 7 (BRIGHT y FLASH)
		or   c                  ; We add paper
		or   b                  ; And ink, which recomposed the attribute
		ld   (hl),a             ; We write it,
		inc  l                  ; And increase the pointer.
		jr   nz, fade_out_bucle ; We continue until the end of the third (when L is 0)
		inc  h                  ; Next third
		dec  e
		jr   nz, fade_out_bucle ; Repeat 3 times
		pop bc
		djnz fade_out_extern
	#endasm
}

signed int addsign (signed int n, signed int value) {
	//if (n >= 0) return value; else return -value;
	return n == 0 ? 0 : n > 0 ? value : -value;
}

unsigned int abs (int n) {
	if (n < 0)
		return (unsigned int) (-n);
	else
		return (unsigned int) n;
}

// Engine globals (for speed) & size!
unsigned char gpx, gpy, gpd, gpc, gpt, gps;
unsigned char gpxx, gpyy;//changed from unsigned
unsigned char gpcx, gpcy;//changed from unsigned
unsigned char possee, hit_v, hit_h, hit, wall_h, wall_v;
unsigned char gpen_x, gpen_y, gpen_cx, gpen_cy, gpen_xx, gpen_yy, gpaux;
unsigned char tocado, active, killable, animate;
unsigned char gpit, gpjt;
unsigned char *map_pointer;

void kill_player (unsigned char sound) {
	p_life --;
#ifdef MODE_128K
	//_AY_ST_ALL ();
	_AY_PL_SND (sound);
#else
	peta_el_beeper (sound);
#endif
#ifdef DIE_AND_RESPAWN
	p_killme = 1;
	half_life = 0;
#endif
#ifdef PLAYER_FLICKERS
	p_estado = EST_PARP;
	p_ct_estado = 50;
#endif
#ifdef REENTER_ON_DEATH
	o_pant = 99;
	saca_a_todo_el_mundo_de_aqui (0);
#endif
}

// Floating objects
#if defined(ENABLE_FLOATING_OBJECTS) || defined (ENABLE_SIM)
#include "engine/floating.h"
#ifdef ENABLE_SIM
#include "engine/sim.h"
#endif
#endif

// Animated tiles
#ifdef ENABLE_TILANIMS
#include "tilanim.h"
#endif

// Breakable tiles helper functions
#ifdef BREAKABLE_WALLS
#include "engine/breakable.h"
#endif

#ifdef BREAKABLE_WALLS_SIMPLE
#include "engine/breakable-s.h"
#endif

// Initialization functions
#include "engine/inits.h"

// Hitter (punch/sword) helper functions
#if defined(PLAYER_CAN_PUNCH) || defined(PLAYER_HAZ_SWORD) || defined(PLAYER_HAZ_WHIP)
#include "engine/hitter.h"
#endif

// Bullets helper functions
#ifdef PLAYER_CAN_FIRE
#include "engine/bullets.h"
#endif



	

// Simple bomb helper functions
#ifdef PLAYER_SIMPLE_BOMBS
#include "engine/bombs-s.h"
#endif

// Block processing
#include "engine/blocks.h"

// Main player movement
#if defined (PHANTOMAS_ENGINE)
#include "engine/phantomasmove.h"
#else
#include "engine/playermove.h"
#endif

#ifdef ACTIVATE_SCRIPTING
void run_entering_script (void) {
#ifdef EXTENDED_LEVELS
	if (level_data->activate_scripting) {
#endif
#ifdef LINE_OF_TEXT
		print_str (LINE_OF_TEXT_X, LINE_OF_TEXT, LINE_OF_TEXT_ATTR, "                              ");
#endif
		// We execute the scripts to enter screen:
		run_script (2 * MAP_W * MAP_H + 1);
		run_script (n_pant + n_pant);
#ifdef EXTENDED_LEVELS
	}
#endif
}
#endif

// Extra prints (screen drawing helpers)
#ifdef ENABLE_EXTRA_PRINTS
#include "engine/extraprints.h"
#endif

// Level names (screen drawing helpers)
#ifdef ENABLE_LEVEL_NAMES
#include "engine/levelnames.h"
#endif

// Screen drawing
#include "engine/drawscr.h"

// Enemies
#ifdef USE_OLD_ENEMS
#include "engine/oldenems.h"
#else
#include "engine/enems.h"
#endif

void espera_activa (int espera) {
	do {
#ifndef MODE_128K
		gpjt = 250; do { gpit = 1; } while (--gpjt);
#else
		#asm
			halt
		#endasm
#endif
#ifdef DIE_AND_RESPAWN
		if (p_killme == 0 && button_pressed ()) break;
#else
		if (button_pressed ()) break;
#endif
	} while (--espera);
	sp_Border (0);
}

#ifdef ACTIVATE_SCRIPTING
void run_fire_script (void) {
	run_script (2 * MAP_W * MAP_H + 2);
	run_script (n_pant + n_pant + 1);
	// I use this for debug when further developing the engine, it comes handy:
/*
	for (gpit = 0; gpit < 16; gpit ++) {
		sp_PrintAtInv (0, gpit + gpit, 71, 16 + flags [gpit]);
	}
*/
/*
	for (gpit = 0; gpit < 16; gpit ++) {
		//sp_PrintAtInv (23, gpit + gpit, 71, 16 + baddies [enoffs + gpit].t);
		print_number2(gpit+gpit,23,flags [gpit]);
	}
*/
}
#endif

#ifndef PHANTOMAS_ENGINE
// UP DOWN LEFT RIGHT FIRE JUMP <- with fire/hitter/throwable
// UP DOWN LEFT RIGHT JUMP xxxx <- with just jump, so configure ahead:
unsigned int keyscancodes [] = {
	//0x02fb, 0x02fd, 0x01fd, 0x04fd, 0x047f, 0x087f,		// WSADMN
	0x02fb, 0x02fd, 0x01fd, 0x04fd, 0x017f, 0,
#ifdef USE_TWO_BUTTONS
	//0x01fb, 0x01fd, 0x02df, 0x01df, 0x047f, 0x087f 		// QAOPMN
	0x01fb, 0x01fd, 0x02df, 0x01df, 0x017f, 0 		// QAOPMN
#else
	//0x01fb, 0x01fd, 0x02df, 0x01df, 0x01fb, 0 			// QAOPQ-
	0x01fb, 0x01fd, 0x02df, 0x01df, 0x017f, 0 			// QAOPQ-
#endif
};
#endif

void select_joyfunc (void) {
#ifdef PHANTOMAS_ENGINE
	joyfunc = sp_JoyKeyboard;
	keys.up    = 0x01fb;	// Q
	keys.down  = 0x01fd;	// A
	keys.left  = 0x02df;	// O
	keys.right = 0x01df;	// P
	keys.fire  = 0x017f;	// SPACE
	#asm
		; Music generated by beepola
		; call musicstart
	#endasm
	while (0 == sp_GetKey ());
#else
#ifdef MODE_128K
#else
	#asm
		; Music generated by beepola
		call musicstart
	#endasm
#endif
	while (1) {
		gpit = sp_GetKey ();
		if (gpit == '1' || gpit == '2') {
			joyfunc = sp_JoyKeyboard;
			gpjt = (gpit - '1') ? 6 : 0;
#ifdef USE_TWO_BUTTONS
			keys.up = keyscancodes [gpjt ++];
			keys.down = keyscancodes [gpjt ++];
			keys.left = keyscancodes [gpjt ++];
			keys.right = keyscancodes [gpjt ++];
			key_fire = keys.fire = keyscancodes [gpjt ++];
			key_jump = keyscancodes [gpjt];
#else
			//ATTENTION
			// for some reason if key_jump is uncommented, if you
			// turn off USE_TWO_BUTTONS, it will not compile
			keys.up = keyscancodes [gpjt ++];
			keys.down = keyscancodes [gpjt ++];
			keys.left = keyscancodes [gpjt ++];
			keys.right = keyscancodes [gpjt ++];
			key_fire = keys.fire = keyscancodes [gpjt];
			//key_jump = keyscancodes [gpjt];//ATTENTION, commented out
			
			/*
			key_jump = keyscancodes [gpjt ++];		// UP
			keys.down = keyscancodes [gpjt ++];		// DOWN
			keys.left = keyscancodes [gpjt ++];		// LEFT
			keys.right = keyscancodes [gpjt ++];	// RIGHT
			keys.fire = keyscancodes [gpjt ++];		// FIRE
			keys.up = keyscancodes [gpjt];		// JUMP
			*/
#endif
			break;
		} else if (gpit == '3') {
			joyfunc = sp_JoyKempston;
			break;
		} else if (gpit == '4') {
			joyfunc = sp_JoySinclair1;
			break;
		}
	}
#ifdef MODE_128K
	_AY_PL_SND (0);
	sp_WaitForNoKey ();
#else
	#asm
		di
	#endasm
#endif
#endif
}

// Experimental
#ifdef ENABLE_LAVA
#include "engine\lava.h"
#endif
