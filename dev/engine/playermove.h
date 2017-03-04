// MT Engine MK2
// Copyleft 2014 the Mojon Twins

// playermove.h
// Player movement v5.0 : half-box/point collision
// Copyleft 2013 by The Mojon Twins

// Predefine button detection
#ifdef USE_TWO_BUTTONS
	#define BUTTON_FIRE	((gpit & sp_FIRE) == 0 || sp_KeyPressed (key_fire))
	#define BUTTON_JUMP	(sp_KeyPressed (key_jump))
#else
#if defined (PLAYER_CAN_FIRE) || defined (PLAYER_CAN_PUNCH) || defined (PLAYER_HAZ_SWORD) || defined (CARRIABLE_BOXES_THROWABLE)
	#define BUTTON_FIRE	((gpit & sp_FIRE) == 0)
	#define BUTTON_JUMP	((gpit & sp_UP) == 0)
#else
	#define BUTTON_FIRE	(0)
	#define BUTTON_JUMP	((gpit & sp_FIRE) == 0)
#endif
#endif
#if defined (ENABLE_FLOATING_OBJECTS) && defined (ENABLE_FO_CARRIABLE_BOXES) && defined (CARRIABLE_BOXES_CORCHONETA)
signed int p_vlhit;
#endif
unsigned char ptx1, ptx2, pty1, pty2, pty3, pt1, pt2;
unsigned char move (void) {

	wall_v = wall_h = 0;
	gpit = (joyfunc) (&keys); // Leemos del teclado

	// ***************************************************************************
	//  MOVEMENT IN THE VERTICAL AXIS
	// ***************************************************************************

#ifdef PLAYER_MOGGY_STYLE
	// Controller

	if ( ! ((gpit & sp_UP) == 0 || (gpit & sp_DOWN) == 0)) {
		p_facing_v = 0xff;
		if (p_vy > 0) {
			p_vy -= PLAYER_RX; if (p_vy < 0) p_vy = 0;
		} else if (p_vy < 0) {
			p_vy += PLAYER_RX; if (p_vy > 0) p_vy = 0;
		}
	}

	if ((gpit & sp_UP) == 0) {
		p_facing_v = FACING_UP;
		if (p_vy > -PLAYER_MAX_VX) p_vy -= PLAYER_AX;
	}

	if ((gpit & sp_DOWN) == 0) {
		p_facing_v = FACING_DOWN;
		if (p_vy < PLAYER_MAX_VX) p_vy += PLAYER_AX;
	}
#endif

#if defined(PLAYER_HAS_JUMP) || defined(PLAYER_HAS_JETPAC) || defined(PLAYER_BOOTEE) || defined(PLAYER_CUMULATIVE_JUMP)
	if (do_gravity) {
		// Gravity
		if (p_vy < PLAYER_MAX_VY_CAYENDO) p_vy += PLAYER_G;	else p_vy = PLAYER_MAX_VY_CAYENDO;
	}
#ifdef PLAYER_CUMULATIVE_JUMP
	if (!p_saltando)
#endif
		if (p_gotten) p_vy = 0;
#endif

#ifdef PLAYER_HAS_JETPAC
	// Jetpac Boost
#ifdef SWITCHABLE_ENGINES
	if (p_engine == SENG_JETP)
#endif
		if (BUTTON_JUMP) {
#ifdef JETPAC_DEPLETES
			if (p_fuel) {
#endif			
				p_vy -= PLAYER_INCR_JETPAC;
				if (p_vy < -PLAYER_MAX_VY_JETPAC) p_vy = -PLAYER_MAX_VY_JETPAC;
#ifdef JETPAC_DEPLETES
				if (0 == (maincounter & (JETPAC_DEPLETES-1))) p_fuel --;
			}
		} else {
			if (0 == (maincounter & (JETPAC_AUTO_REFILLS-1)) && p_fuel < JETPAC_FUEL_MAX) p_fuel ++;
		}
#else
		}
#endif
#endif

#ifdef PLAYER_HAS_SWIM
#ifdef SWITCHABLE_ENGINES
	if (p_engine == SENG_SWIM) {
#endif
		p_gotten = 1;
		if ( ! ((gpit & sp_DOWN) == 0 || (gpit & sp_UP) == 0)) {
			p_vy -= PLAYER_ASWIM >> 1;
			if (p_vy < PLAYER_MAX_VSWIM >> 1) p_vy = -(PLAYER_MAX_VSWIM >> 1);
		}
	
		if ((gpit & sp_DOWN) == 0) {
			if (p_vy < PLAYER_MAX_VSWIM) p_vy += PLAYER_ASWIM;
		}

		//if ((gpit & sp_UP) == 0) {	
		//CUSTOM!
		if ((gpit & sp_UP) == 0 || sp_KeyPressed (key_jump)) {			
			if (p_vy > -PLAYER_MAX_VSWIM) p_vy -= PLAYER_ASWIM;
		}
#ifdef SWITCHABLE_ENGINES
	}
#endif
#endif

	// Move
	p_y += p_vy;

	// Safe
	if (p_y < 0) p_y = 0;
	if (p_y > 9216) p_y = 9216;

	// Handle collision
	// half-box collision. Check for tile behaviour in two points.
	// Which points? It depends on the type of collision configured:
	gpx = p_x >> 6;
	gpy = p_y >> 6;

#if defined (BOUNDING_BOX_8_BOTTOM)
	ptx1 = (gpx + 4) >> 4;
	ptx2 = (gpx + 11) >> 4;
	pty1 = (gpy + 8) >> 4;
	pty2 = (gpy + 15) >> 4;
#elif defined (BOUNDING_BOX_8_CENTERED)
	ptx1 = (gpx + 4) >> 4;
	ptx2 = (gpx + 11) >> 4;
	pty1 = (gpy + 4) >> 4;
	pty2 = (gpy + 11) >> 4;
#else
	ptx1 = (gpx) >> 4;
	ptx2 = (gpx + 15) >> 4;
	pty1 = (gpy) >> 4;
	pty2 = (gpy + 15) >> 4;
#endif

	hit_v = 0;
#ifdef PLAYER_MOGGY_STYLE
	if (p_vy < 0) {
#else
	if (p_vy + ptgmy < 0) {
#endif
		pt1 = attr (ptx1, pty1);
		pt2 = attr (ptx2, pty1);
		if ((pt1 & 8) || (pt2 & 8)) {
#ifdef PLAYER_BOUNCE_WITH_WALLS
			p_vy = -(p_vy / 2);
#else
			p_vy = 0;
#endif
#if defined (BOUNDING_BOX_8_BOTTOM)
			p_y = ((pty1 + 1) << 10) - 512;
#elif defined (BOUNDING_BOX_8_CENTERED)
			p_y = ((pty1 + 1) << 10) - 256;
#else
			p_y = ((pty1 + 1) << 10);
#endif
			wall_v = WTOP;
		} else if ((pt1 & 1) || (pt2 & 1)) {
			hit_v = 1;
		}
#ifdef PLAYER_MOGGY_STYLE
	} else if (p_vy > 0) {
#else
	} else if (p_vy + ptgmy > 0) {
#endif
		pt1 = attr (ptx1, pty2);
		pt2 = attr (ptx2, pty2);
		// Greed Optimization tip! Remove this line and uncomment the next one:
		// (As long as you don't have type 8 blocks over type 4 blocks in your game, the short line is fine)
		if ((pt1 & 8) || (pt2 & 8) || (((gpy - 1) & 15) < 8 && ((pt1 & 4) || (pt2 & 4)))) {
		//if (((gpy - 1) & 15) < 7 && ((pt1 & 12) || (pt2 & 12))) {
			
#if defined (ENABLE_FLOATING_OBJECTS) && defined (ENABLE_FO_CARRIABLE_BOXES) && defined (CARRIABLE_BOXES_CORCHONETA)
			// Remember original p_vy to use with the corchoneta.
			p_vlhit = p_vy;
#endif			
			
#ifdef PLAYER_BOUNCE_WITH_WALLS
			p_vy = -(p_vy / 2);
#else
			p_vy = 0;
#endif
#if defined (BOUNDING_BOX_8_BOTTOM)
			p_y = ((pty2 - 1) << 10);
#elif defined (BOUNDING_BOX_8_CENTERED)
			p_y = ((pty2 - 1) << 10) + 256;
#else
			p_y = ((pty2 - 1) << 10);
#endif
			wall_v = WBOTTOM;
		} else if ((pt1 & 1) || (pt2 & 1)) {
			hit_v = 1;
		}
	}
	
	gpxx = gpx >> 4;
	gpyy = gpy >> 4;
	
#ifndef PLAYER_MOGGY_STYLE
	// Possee
	gpy = p_y >> 6;
	pty3 = (gpy + 16) >> 4;
	possee = ((attr (ptx1, pty3) & 12) || (attr (ptx2, pty3) & 12)) && (gpy & 15) < 8;

#ifdef DIE_AND_RESPAWN
#ifndef DISABLE_AUTO_SAFE_SPOT
	// Store a "safe spot" to be respawned to if we die after the next jump.
	// Works a threat and it's pretty simple :)
#ifdef SWITCHABLE_ENGINES
	if (p_engine != SENG_SWIM)
#endif
		if (possee) {
#ifdef ENABLE_FLOATING_OBJECTS
			// This prevents floating objects from being considered "safe spots"
			if (0 == ((attr (ptx1, pty3) & 128) || (attr (ptx2, pty3) & 128))) {
#else
			{
#endif
				p_safe_pant = n_pant;
				p_safe_x = gpxx;
				p_safe_y = gpyy;
			}
		}		
#endif	
#endif
#endif

	// *************
	// Jumping Jack!
	// *************

#ifdef PLAYER_HAS_JUMP
#ifdef SWITCHABLE_ENGINES
	if (p_engine == SENG_JUMP) {
#endif

#ifdef PLAYER_CUMULATIVE_JUMP
		if (BUTTON_JUMP && (possee || p_gotten || hit_v)) {
			p_vy = -p_vy - PLAYER_VY_INICIAL_SALTO;
#ifdef PLAYER_VARIABLE_JUMP
			if (p_vy < -player_max_vy_saltando) p_vy = -player_max_vy_saltando;
#else
			if (p_vy < -PLAYER_MAX_VY_SALTANDO) p_vy = -PLAYER_MAX_VY_SALTANDO;
#endif
#else
		if (BUTTON_JUMP && p_saltando == 0 && (possee || p_gotten || hit_v)) {
#endif
			p_saltando = 1;
			p_cont_salto = 0;
#ifdef MODE_128K
			PLAY_SOUND (2);
#else
			peta_el_beeper (3);
#endif
		}

#ifndef PLAYER_CUMULATIVE_JUMP
		if (BUTTON_JUMP && p_saltando) {
			p_vy -= (PLAYER_VY_INICIAL_SALTO + PLAYER_INCR_SALTO - (p_cont_salto>>1));
#ifdef PLAYER_VARIABLE_JUMP
			if (p_vy < -player_max_vy_saltando) p_vy = -player_max_vy_saltando;
#else
			if (p_vy < -PLAYER_MAX_VY_SALTANDO) p_vy = -PLAYER_MAX_VY_SALTANDO;
#endif
			p_cont_salto ++;
			if (p_cont_salto == 8)
				p_saltando = 0;
		}
#endif
	
		if (!BUTTON_JUMP) p_saltando = 0;
#ifdef SWITCHABLE_ENGINES
	}
#endif
#endif

#ifdef PLAYER_BOOTEE
#ifdef SWITCHABLE_ENGINES
	if (p_engine == SENG_BOOT) {
#endif
		// Bootee engine
		if ( p_saltando == 0 && (possee || p_gotten || hit_v) ) {
			p_saltando = 1;
			p_cont_salto = 0;
#ifdef DIE_AND_RESPAWN
			p_safe_pant = n_pant;
			p_safe_x = gpxx;
			p_safe_y = gpyy;
#endif
#ifdef MODE_128K
			PLAY_SOUND (2);
#else
			peta_el_beeper (3);
#endif
		}
	
		if (p_saltando ) {
			p_vy -= (PLAYER_VY_INICIAL_SALTO + PLAYER_INCR_SALTO - (p_cont_salto>>1));
#ifdef PLAYER_VARIABLE_JUMP
			if (p_vy < -player_max_vy_saltando) p_vy = -player_max_vy_saltando;
#else
			if (p_vy < -PLAYER_MAX_VY_SALTANDO) p_vy = -PLAYER_MAX_VY_SALTANDO;
#endif
			p_cont_salto ++;
			if (p_cont_salto == 8)
				p_saltando = 0;
		}
#ifdef SWITCHABLE_ENGINES
	}
#endif
#endif


	// ***************************************************************************
	//  MOVEMENT IN THE HORIZONTAL AXIS
	// ***************************************************************************

#ifdef ENABLE_CONVEYORS	
	// Conveyors
	if (possee) {
		gpy = p_y >> 6;
		pt1 = attr (ptx1, pty3);
		pt2 = attr (ptx2, pty3);
		if (pt1 & 32) {
			p_gotten = 1; ptgmy = 0;
			ptgmx = (pt1 & 1) ? 64 : -64;
		}
		if (pt2 & 32) {
			p_gotten = 1; ptgmy = 0;
			ptgmx = (pt2 & 1) ? 64 : -64;
		}
	}
#endif
	
	// Controller
	if ( ! ((gpit & sp_LEFT) == 0 || (gpit & sp_RIGHT) == 0)) {
#ifdef PLAYER_MOGGY_STYLE
		p_facing_h = 0xff;
#endif
		if (p_vx > 0) {
			p_vx -= PLAYER_RX; if (p_vx < 0) p_vx = 0;
		} else if (p_vx < 0) {
			p_vx += PLAYER_RX; if (p_vx > 0) p_vx = 0;
		}
	}

	if ((gpit & sp_LEFT) == 0) {
#ifdef PLAYER_MOGGY_STYLE
		p_facing_h = FACING_LEFT;
#endif
		if (p_vx > -PLAYER_MAX_VX) {
#ifndef PLAYER_MOGGY_STYLE
			p_facing = 0;
#endif
			p_vx -= PLAYER_AX;
		}
	}

	if ((gpit & sp_RIGHT) == 0) {
#ifdef PLAYER_MOGGY_STYLE
		p_facing_h = FACING_RIGHT;
#endif
		if (p_vx < PLAYER_MAX_VX) {
			p_vx += PLAYER_AX;
#ifndef PLAYER_MOGGY_STYLE
			p_facing = 1;
#endif
		}
	}

	// Move
	p_x = p_x + p_vx;
#ifndef PLAYER_MOGGY_STYLE	
	if (p_gotten) p_x += ptgmx;
#endif

	// Safe
	if (p_x < 0) p_x = 0;
	if (p_x > 14336) p_x = 14336;

	// Handle collision
	// half-box collision. Check for tile behaviour in two points.
	// Which points? It depends on the type of collision configured:
	gpx = p_x >> 6;
	gpy = p_y >> 6;

#if defined (BOUNDING_BOX_8_BOTTOM)
	ptx1 = (gpx + 4) >> 4;
	ptx2 = (gpx + 11) >> 4;
	pty1 = (gpy + 8) >> 4;
	pty2 = (gpy + 15) >> 4;
#elif defined (BOUNDING_BOX_8_CENTERED)
	ptx1 = (gpx + 4) >> 4;
	ptx2 = (gpx + 11) >> 4;
	pty1 = (gpy + 4) >> 4;
	pty2 = (gpy + 11) >> 4;
#else
	ptx1 = (gpx) >> 4;
	ptx2 = (gpx + 15) >> 4;
	pty1 = (gpy) >> 4;
	pty2 = (gpy + 15) >> 4;
#endif

	hit_h = 0;

#ifdef PLAYER_MOGGY_STYLE
	if (p_vx < 0) {
#else
	if (p_vx + ptgmx < 0) {
#endif
		pt1 = attr (ptx1, pty1);
		pt2 = attr (ptx1, pty2);
		if ((pt1 & 8) || (pt2 & 8)) {
#ifdef PLAYER_BOUNCE_WITH_WALLS
			p_vx = -(p_vx / 2);
#else
			p_vx = 0;
#endif
#if defined (BOUNDING_BOX_8_BOTTOM) || defined (BOUNDING_BOX_8_CENTERED)
			p_x = ((ptx1 + 1) << 10) - 256;
#else
			p_x = ((ptx1 + 1) << 10);
#endif
			wall_h = WLEFT;
		} else if ((pt1 & 1) || (pt2 & 1)) {
			hit_h = 1;
		}
#ifdef PLAYER_MOGGY_STYLE
	} else if (p_vx > 0) {
#else
	} else if (p_vx + ptgmx > 0) {
#endif
		pt1 = attr (ptx2, pty1);
		pt2 = attr (ptx2, pty2);
		if ((pt1 & 8) || (pt2 & 8)) {
#ifdef PLAYER_BOUNCE_WITH_WALLS
			p_vx = -(p_vx / 2);
#else
			p_vx = 0;
#endif
#if defined (BOUNDING_BOX_8_BOTTOM) || defined (BOUNDING_BOX_8_CENTERED)
			p_x = ((ptx2 - 1) << 10) + 256;
#else
			p_x = ((ptx2 - 1) << 10);
#endif
			wall_h = WRIGHT;
		} else if ((pt1 & 1) || (pt2 & 1)) {
			hit_h = 1;
		}
	}

#ifdef PLAYER_MOGGY_STYLE
	// Priority to decide facing
	#ifdef TOP_OVER_SIDE
		if (p_facing_v != 0xff) {
			p_facing = p_facing_v;
		} else if (p_facing_h != 0xff) {
			p_facing = p_facing_h;
		}
	#else
		if (p_facing_h != 0xff) {
			p_facing = p_facing_h;
		} else if (p_facing_v != 0xff) {
			p_facing = p_facing_v;
		}
	#endif
#endif

#ifdef FIRE_TO_PUSH
pushed_any = 0;
#endif

	// ********************
	// Hail the corchoneta!
	// ********************
	
#if defined (ENABLE_FLOATING_OBJECTS) && defined (ENABLE_FO_CARRIABLE_BOXES) && defined (CARRIABLE_BOXES_CORCHONETA)
	// With special carriable boxes
	if (wall_v == WBOTTOM) {
		
		gpx = p_x >> 6;
		pty1 = (gpy + 16) >> 4;
		ptx1 = (gpx + 8) >> 4;
		if (attr (ptx1, pty1) & 64) p_vy = -(abs (p_vlhit)) << 1;	 // CHECK FORMULA!
		if (p_vy < -CARRIABLE_BOXES_MAX_C_VY) p_vy = -CARRIABLE_BOXES_MAX_C_VY;
	}
#endif

	// Other ways soon!

	// ************************************************
	// Tile type 10 operations (push boxes, open locks)
	// ************************************************
	
#if defined (PLAYER_PUSH_BOXES) || !defined (DEACTIVATE_KEYS)
	gpx = p_x >> 6;
	ptx1 = (gpx + 8) >> 4;
	pty1 = (gpy + 8) >> 4;
#ifdef PLAYER_MOGGY_STYLE
	if (wall_v == WTOP) {
		// interact up			
#if defined (BOUNDING_BOX_8_BOTTOM)
		pty1 = (gpy + 7) >> 4;
#elif defined (BOUNDING_BOX_8_CENTERED)
		pty1 = (gpy + 3) >> 4;
#else
		pty1 = gpy >> 3;		
#endif
		if (attr (ptx1, pty1) == 10) process_tile (ptx1, pty1, ptx1, pty1 - 1);
	} else if (wall_v == WBOTTOM) {
		// interact down
#if defined (BOUNDING_BOX_8_BOTTOM)
		pty1 = (gpy + 16) >> 4;
#elif defined (BOUNDING_BOX_8_CENTERED)
		pty1 = (gpy + 12) >> 4;
#else
		pty1 = (gpy + 16) >> 3;				
#endif		
		if (attr (ptx1, pty1) == 10) process_tile (ptx1, pty1, ptx1, pty1 + 1);
	} else
#endif	
	if (wall_h == WLEFT) {		
		// interact left
#if defined (BOUNDING_BOX_8_BOTTOM) || defined (BOUNDING_BOX_8_CENTERED)
		ptx1 = (gpx + 3) >> 4;
#else
		ptx1 = gpx >> 4;		
#endif		
		if (attr (ptx1, pty1) == 10) process_tile (ptx1, pty1, ptx1 - 1, pty1);
	} else if (wall_h == WRIGHT) {
		// interact right
#if defined (BOUNDING_BOX_8_BOTTOM) || defined (BOUNDING_BOX_8_CENTERED)
		ptx1 = (gpx + 12) >> 4;
#else
		ptx1 = (gpx + 16) >> 4;		
#endif		
		if (attr (ptx1, pty1) == 10) process_tile (ptx1, pty1, ptx1 + 1, pty1);
	}
#endif

	// ************
	// Fire bullets
	// ************
	
#include "engine/playermods/fire.h"

	// *******
	// Hitters
	// *******
	
#include "engine/playermods/hitter.h"

	// *****
	// Bombs
	// *****
	
#include "engine/playermods/bombs.h"

	// Done with the fire button...
#include "engine/playermods/fire_button.h"	

	// **********
	// Evil tiles
	// **********

#ifndef DEACTIVATE_EVIL_TILE
	// hit_v tiene preferencia sobre hit_h
	hit = 0;
	if (hit_v) {
		hit = 1;
#ifdef FULL_BOUNCE
		p_vy = addsign (-p_vy, PLAYER_MAX_VX);
#else
		p_vy = -p_vy;
#endif
	} else if (hit_h) {
		hit = 1;
#ifdef FULL_BOUNCE
		p_vx = addsign (-p_vx, PLAYER_MAX_VX);
#else
		p_vx = -p_vx;
#endif
	}
	if (hit) {
#ifdef PLAYER_FLICKERS
		if (p_life > 0 && p_estado == EST_NORMAL) {
#else
		if (p_life > 0) {
#endif
#ifdef MODE_128K
			kill_player (8);
#else
			kill_player (4);
#endif
		}
	}
#endif

	// ********
	// Tile get 
	// ********
	
#ifdef TILE_GET
	// Tile get
	gpxx = (gpx + 8) >> 4;
	gpyy = (gpy + 8) >> 4;
	if (qtile (gpxx, gpyy) == TILE_GET) {
		gpaux = gpxx + (gpyy << 4) - gpyy;
		map_buff [gpaux] = 0;
		map_attr [gpaux] = 0;
		draw_coloured_tile_gamearea (gpxx, gpyy, 0);
		flags [TILE_GET_FLAG] ++;
#ifdef MODE_128K
			PLAY_SOUND (5);
#else
			peta_el_beeper (4);
#endif

#ifdef TILE_GET_SCRIPT
		run_script run_script (2 * MAP_W * MAP_H + 4);
#endif
	}
#endif

	// **********************
	// Select animation frame
	// **********************

#ifndef PLAYER_MOGGY_STYLE
#ifdef PLAYER_BOOTEE
	gpit = p_facing << 2;
	if (p_vy == 0) {
		p_next_frame = player_frames [gpit];
	} else if (p_vy < 0) {
		p_next_frame = player_frames [gpit + 1];
	} else {
		p_next_frame = player_frames [gpit + 2];
	}
#else
	if (!possee && !p_gotten) {
		p_next_frame = player_frames [8 + p_facing];
	} else {
		gpit = p_facing << 2;
		if (p_vx == 0) {
#ifdef PLAYER_ALTERNATE_ANIMATION
			p_next_frame = player_frames [gpit];
#else
			p_next_frame = player_frames [gpit + 1];
#endif
		} else {
			p_subframe ++;
			if (p_subframe == 4) {
				p_subframe = 0;
#ifdef PLAYER_ALTERNATE_ANIMATION
				p_frame ++; if (p_frame == 3) p_frame = 0;
#else
				p_frame = (p_frame + 1) & 3;
#endif
#ifdef PLAYER_STEP_SOUND
				step ();
#endif
			}
			p_next_frame = player_frames [gpit + p_frame];
		}
	}
#endif
#else

	if (p_vx || p_vy) {
		p_subframe ++;
		if (p_subframe == 4) {
			p_subframe = 0;
			p_frame = !p_frame;
#ifdef PLAYER_STEP_SOUND
			step ();
#endif
		}
	}

	p_next_frame = player_frames [p_facing + p_frame];
#endif
}