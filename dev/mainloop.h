// MT Engine MK2
// MT Engine MK2
// Copyleft 2014 the Mojon Twins

// mainloop.h
// This is, the main game loop (plus initializations, plus menu, plus level screen...

//Get everybody out of here

#define MIN(x, y) (((x) < (y)) ? (x) : (y))
#define MAX(x, y) (((x) > (y)) ? (x) : (y))


void saca_a_todo_el_mundo_de_aqui (unsigned char which_ones)
{
    if (which_ones == 0)
        sp_MoveSprAbs (sp_player, spritesClip, 0, VIEWPORT_Y + 30, VIEWPORT_X + 20, 0, 0);

    for (gpit = 0; gpit < 3; gpit ++)
        sp_MoveSprAbs (sp_moviles [gpit], spritesClip, 0, VIEWPORT_Y + 30, VIEWPORT_X + 20, 0, 0);

#ifdef PLAYER_CAN_FIRE
    for (gpit = 0; gpit < MAX_BULLETS; gpit ++)
        sp_MoveSprAbs (sp_bullets [gpit], spritesClip, 0, -2, -2, 0, 0);

#endif
#ifdef ENABLE_SHOOTERS
    for (gpit = 0; gpit < MAX_COCOS; gpit ++)
        sp_MoveSprAbs (sp_cocos [gpit], spritesClip, 0, -2, -2, 0, 0);

#endif
#if defined(PLAYER_CAN_PUNCH) || defined(PLAYER_HAZ_SWORD) || defined(PLAYER_HAZ_WHIP)
    sp_MoveSprAbs (sp_hitter, spritesClip, 0, -2, -2, 0, 0);

#endif
#ifdef ENABLE_FO_CARRIABLE_BOXES
    sp_MoveSprAbs (sp_carriable, spritesClip, 0, -2, -2, 0, 0);

#endif
}

unsigned char action_pressed;
unsigned char objs_old, keys_old, life_old, killed_old;

#ifdef MAX_AMMO
unsigned char ammo_old;
#endif

#if defined(TIMER_ENABLE) && defined(PLAYER_SHOW_TIMER)
unsigned char timer_old;
#endif

#ifdef PLAYER_SHOW_FLAG
unsigned char flag_old;
#endif

#if defined (PLAYER_HAS_JETPAC) && defined (JETPAC_DEPLETES) && defined (PLAYER_SHOW_FUEL)
unsigned char fuel_old;
#endif

#ifdef GET_X_MORE
unsigned char *getxmore = " GET X MORE ";
#endif

#ifdef SCRIPTING_KEY_M
int key_m;
#endif

#ifdef PAUSE_ABORT
int key_h, key_y;
#endif

#if defined (MSC_MAXITEMS) || defined (ENABLE_SIM)
int key_z;
unsigned char key_z_pressed = 0;
#endif

#ifdef COMPRESSED_LEVELS
unsigned char mlplaying;
#endif

unsigned char success;
unsigned char playing;

// Main loop

void main (void)
{
    // *********************
    // SYSTEM INITIALIZATION
    // *********************

    cortina ();

#asm
    di
#endasm

#ifdef MODE_128K
    // Install ISR

    sp_InitIM2(0xf1f1);
    sp_CreateGenericISR(0xf1f1);
    sp_RegisterHook(255, ISR);
#endif

    // splib2 initialization
    sp_Initialize (0, 0);
    sp_Border (BLACK);

    // Reserve memory blocks for sprites
    sp_AddMemory(0, NUMBLOCKS, 14, AD_FREE);

#ifdef SCRIPTING_KEY_M
    key_m = 0x047f;
#endif
#ifdef PAUSE_ABORT
    key_h = 0x08bf;
    key_y = 0x10df;
#endif
#if defined (MSC_MAXITEMS) || defined (ENABLE_SIM)
    key_z = 0x02fe;
#endif
    joyfunc = sp_JoyKeyboard;

    // Load tileset
    gen_pt = tileset;
    gpit = 0;

    do
        {
            sp_TileArray (gpit, gen_pt);
            gen_pt += 8;
            gpit ++;
        }

    while (gpit);

    // Clipping rectangle
    spritesClipValues.row_coord = VIEWPORT_Y;

    spritesClipValues.col_coord = VIEWPORT_X;

    spritesClipValues.height = 20;

    spritesClipValues.width = 30;

    spritesClip = &spritesClipValues;

    // Sprite creation
#ifdef NO_MASKS
    sp_player = sp_CreateSpr (sp_OR_SPRITE, 3, sprite_2_a, 1, TRANSPARENT);

    sp_AddColSpr (sp_player, sprite_2_b, TRANSPARENT);

    sp_AddColSpr (sp_player, sprite_2_c, TRANSPARENT);

    p_current_frame = p_next_frame = sprite_2_a;

    for (gpit = 0; gpit < 3; gpit ++)
        {
            sp_moviles [gpit] = sp_CreateSpr(sp_OR_SPRITE, 3, sprite_9_a, 1, TRANSPARENT);
            sp_AddColSpr (sp_moviles [gpit], sprite_9_b, TRANSPARENT);
            sp_AddColSpr (sp_moviles [gpit], sprite_9_c, TRANSPARENT);
            en_an_current_frame [gpit] = sprite_9_a;
        }

#else
    sp_player = sp_CreateSpr (sp_MASK_SPRITE, 3, sprite_2_a, 1, TRANSPARENT);

    sp_AddColSpr (sp_player, sprite_2_b, TRANSPARENT);

    sp_AddColSpr (sp_player, sprite_2_c, TRANSPARENT);

    p_current_frame = p_next_frame = sprite_2_a;

    for (gpit = 0; gpit < 3; gpit ++)
        {
            sp_moviles [gpit] = sp_CreateSpr(sp_MASK_SPRITE, 3, sprite_9_a, 3, TRANSPARENT);
            sp_AddColSpr (sp_moviles [gpit], sprite_9_b, TRANSPARENT);
            sp_AddColSpr (sp_moviles [gpit], sprite_9_c, TRANSPARENT);
            en_an_current_frame [gpit] = en_an_next_frame [gpit] = sprite_9_a;
        }

#endif

#ifdef PLAYER_CAN_FIRE
    for (gpit = 0; gpit < MAX_BULLETS; gpit ++)
        {
#ifdef MASKED_BULLETS
            sp_bullets [gpit] = sp_CreateSpr (sp_MASK_SPRITE, 2, sprite_19_a, 1, TRANSPARENT);
#else
            sp_bullets [gpit] = sp_CreateSpr (sp_OR_SPRITE, 2, sprite_19_a, 1, TRANSPARENT);
#endif
            sp_AddColSpr (sp_bullets [gpit], sprite_19_a + 32, TRANSPARENT);
        }

#endif

#ifdef ENABLE_FLOATING_OBJECTS
#ifdef ENABLE_FO_CARRIABLE_BOXES
    sp_carriable = sp_CreateSpr (sp_MASK_SPRITE, 3, carriable_a, 2, TRANSPARENT);

    sp_AddColSpr (sp_carriable, carriable_b, TRANSPARENT);

    sp_AddColSpr (sp_carriable, carriable_c, TRANSPARENT);

#endif
#endif

#if defined(PLAYER_CAN_PUNCH)
    sp_hitter = sp_CreateSpr (sp_MASK_SPRITE, 2, sprite_20_a, 2, TRANSPARENT);

    sp_AddColSpr (sp_hitter, sprite_20_a + 32, TRANSPARENT);

    hitter_current_frame = sprite_20_a;

#endif

#if defined(PLAYER_HAZ_SWORD)
    sp_hitter = sp_CreateSpr (sp_MASK_SPRITE, 2, sprite_sword_l, 2, TRANSPARENT);

    sp_AddColSpr (sp_hitter, sprite_sword_l + 32, TRANSPARENT);

    hitter_current_frame = sprite_sword_l;

#endif

#if defined(PLAYER_HAZ_WHIP)
    sp_hitter = sp_CreateSpr (sp_MASK_SPRITE, 2, sprite_whip, 2, TRANSPARENT);

    sp_AddColSpr (sp_hitter, sprite_whip + 32, TRANSPARENT);

    sp_AddColSpr (sp_hitter, sprite_whip + 64, TRANSPARENT);

    hitter_next_frame = hitter_current_frame = sprite_whip;

#endif

#ifdef ENABLE_SHOOTERS
    for (gpit = 0; gpit < MAX_COCOS; gpit ++)
        {
#ifdef MASKED_BULLETS
            sp_cocos [gpit] = sp_CreateSpr (sp_MASK_SPRITE, 2, sprite_19_a, 1, TRANSPARENT);
#else
            sp_cocos [gpit] = sp_CreateSpr (sp_OR_SPRITE, 2, sprite_19_a, 1, TRANSPARENT);
#endif
            sp_AddColSpr (sp_cocos [gpit], sprite_19_a + 32, TRANSPARENT);
        }

#endif

#ifdef MODE_128K
#asm
    ei
#endasm

    // Music player initialization
#ifdef USE_ARKOS
    _AY_ST_ALL ();

#else
    wyz_init ();

#endif
#endif

    // If not using compressed levels, make backup of enemy types now!
#if defined (ENEMY_BACKUP) && !defined (COMPRESSED_LEVELS)
    backup_baddies ();

#endif

    while (1)
        {

            // ************
            // TITLE SCREEN
            // ************
            sp_UpdateNow();

            //blackout ();
#ifdef MODE_128K
            // Resource 0 = title.bin

            get_resource (0, 16384);

#else
            unpack ((unsigned int) (s_title), 16384);
#endif

#ifdef MODE_128K
            _AY_PL_MUS (4);
			//_AY_PL_MUS (0);
#endif
            select_joyfunc ();
#ifdef MODE_128K
            _AY_ST_ALL ();
#endif

            // *******************
            // GAME INITIALIZATION
            // *******************

#ifdef ACTIVATE_SCRIPTING
	#ifdef CLEAR_FLAGS
				msc_init_all ();
	#endif
#endif

#ifdef COMPRESSED_LEVELS
            mlplaying = 1;
            silent_level = 0;
            level = 0;

#ifndef REFILL_ME
            p_life = PLAYER_LIFE;

#endif
#endif

#ifndef DIRECT_TO_PLAY
            // Clear screen and show game frame
			// only for Direct to play
            cortina ();
            sp_UpdateNow();
#ifdef MODE_128K
            // Resource 1 = marco.bin
            get_resource (1, 16384);
#else
            unpack ((unsigned int) (s_marco), 16384);
#endif
#endif

#ifdef ACTIVATE_SCRIPTING
            script_result = 0;
#endif

#ifdef DIE_AND_RESPAWN
            p_killme = 0;
#endif

#ifdef COMPRESSED_LEVELS

            while (mlplaying)
                {
                    prepare_level ();

                    // ****************
                    // NEW LEVEL SCREEN
                    // ****************
					




                    if (!silent_level)
                        {
                            blackout_area ();
                            //print_str (12, 12, 71, "LEVEL");
                            //print_number2 (18, 12, level + 1);
                            /*
                            				if (level > 0) {
                            					gen_password ();
                            					print_str (12, 14, 71, password_text);
                            				}
                            */
                            sp_UpdateNow ();

#ifdef MODE_128K
                            //_AY_PL_MUS (5);
#else
                            _AY_PL_SND (8);
#endif
                            //espera_activa (2500);
                            espera_activa (50);

                        }

                    silent_level = 0;

#endif

                    // ********************
                    // LEVEL INITIALIZATION
                    // ********************

#if defined (ENEMY_BACKUP) && defined (COMPRESSED_LEVELS)
                    backup_baddies ();
#endif

                    playing = 1;

#ifdef ENABLE_SIM
                    sim_init ();
#endif

#ifndef COMPRESSED_LEVELS
                    init_hotspots ();
#ifdef ACTIVATE_SCRIPTING
#ifdef MODE_128K
                    main_script_offset = (unsigned int) (SCRIPT_INIT);
#else
                    main_script_offset = (unsigned int) (main_script);
#endif
#endif
#ifndef DEACTIVATE_KEYS
                    init_bolts ();
#endif
#ifdef RESTORE_ON_INIT
                    restore_baddies ();
#endif
#if defined(PLAYER_KILLS_ENEMIES) || defined (PLAYER_CAN_FIRE)
                    init_baddies ();
#endif
#endif

#ifdef PLAYER_CAN_FIRE
                    init_bullets ();
#endif

#ifdef ENABLE_SHOOTERS
                    init_cocos ();
#endif

#ifndef COMPRESSED_LEVELS
                    n_pant = SCR_INICIO;
#endif

                    init_player ();
                    maincounter = 0;

#ifdef ACTIVATE_SCRIPTING
                    script_result = 0;
	#ifdef CLEAR_FLAGS
						msc_init_all ();
	#endif
#endif

#ifdef ACTIVATE_SCRIPTING
#ifdef EXTENDED_LEVELS

                    if (level_data->activate_scripting)
                        {
#endif
                            // Entering game script
                            run_script (MAP_W * MAP_H * 2);
#ifdef EXTENDED_LEVELS
                        }

#endif
#endif

#ifdef ENABLE_LAVA
                    init_lava ();

#endif

                    do_respawn = 1;

#ifdef PLAYER_KILLS_ENEMIES
#ifdef SHOW_TOTAL
                    // Show total of enemies next to the killed amount.

                    sp_PrintAtInv (KILLED_Y, 2 + KILLED_X, 71, 15);

                    sp_PrintAtInv (KILLED_Y, 3 + KILLED_X, 71, 16 + BADDIES_COUNT / 10);

                    sp_PrintAtInv (KILLED_Y, 4 + KILLED_X, 71, 16 + BADDIES_COUNT % 10);

#endif
#endif

                    objs_old = keys_old = life_old = killed_old = 0xff;

#ifdef MAX_AMMO
                    ammo_old = 0xff;

#endif
#if defined(TIMER_ENABLE) && defined(PLAYER_SHOW_TIMER)
                    timer_old = 0;

#endif
#ifdef PLAYER_SHOW_FLAG
                    flag_old = 99;

#endif
#if defined (PLAYER_HAS_JETPAC) && defined (JETPAC_DEPLETES) && defined (PLAYER_SHOW_FUEL)
                    fuel_old = 99;

#endif
                    success = 0;

#if defined (BREAKABLE_WALLS) || defined (BREAKABLE_WALLS_SIMPLE)
#ifdef BREAKABLE_ANIM
                    do_process_breakable = 0;

                    gen_pt = breaking_f;

                    for (gpit = 0; gpit < MAX_BREAKABLE; gpit ++)
                        *gen_pt ++ = 0;

#endif
#endif

#ifdef MODE_128K
                    // Play music
#if !defined(HANNA_LEVEL_MANAGER) && !defined(SIMPLE_LEVEL_MANAGER)
#ifdef COMPRESSED_LEVELS
#ifdef EXTENDED_LEVELS
                    _AY_PL_MUS (level_data->music_id);

#else
                    _AY_PL_MUS (levels [level].music_id);

#endif
#else
                    //_AY_PL_MUS (0);
#endif
#endif
#endif
                    o_pant = 0xff;

#if defined (MSC_MAXITEMS) || defined (ENABLE_SIM)
                    display_items ();

#endif

                    no_draw = 0;

                    // *********
                    // MAIN LOOP
                    // *********

                    while (playing)
                        {

#ifdef TIMER_ENABLE
                            // Timer

                            if (ctimer.on)
                                {
                                    ctimer.count ++;

                                    if (ctimer.count == ctimer.frames)
                                        {
                                            ctimer.count = 0;

                                            if (ctimer.t)
                                                {
                                                    ctimer.t --;
                                                }

                                            else
                                                {
                                                    ctimer.zero = 1;
                                                }
                                        }
                                }

#if defined(TIMER_SCRIPT_0) && defined(ACTIVATE_SCRIPTING)
                            if (ctimer.zero)
                                {
                                    ctimer.zero = 0;
#ifdef SHOW_TIMER_OVER
                                    saca_a_todo_el_mundo_de_aqui (0);
                                    time_over ();
                                    espera_activa (500);
#endif
#ifdef EXTENDED_LEVELS

                                    if (level_data->activate_scripting)
                                        {
#endif
                                            run_script (MAP_W * MAP_H * 2 + 3);
#ifdef EXTENDED_LEVELS
                                        }

#endif
                                }

#endif

#ifdef TIMER_KILL_0
                            if (ctimer.zero)
                                {
#ifdef SHOW_TIMER_OVER
#ifndef TIMER_SCRIPT_0
                                    saca_a_todo_el_mundo_de_aqui (0);
                                    time_over ();
                                    espera_activa (500);
#endif
#endif
                                    ctimer.zero = 0;
#ifdef TIMER_AUTO_RESET
                                    ctimer.t = TIMER_INITIAL;
#endif
#ifdef MODE_128K
                                    kill_player (7);
#else
                                    kill_player (4);
#endif
#if defined(TIMER_WARP_TO_X) && defined(TIMER_WARP_TO_Y)
                                    p_x = TIMER_WARP_TO_X << 10;
                                    p_y = TIMER_WARP_TO_Y << 10;
#endif
#ifdef TIMER_WARP_TO
                                    n_pant = TIMER_WARP_TO;
                                    draw_scr ();
#endif
                                }

#endif
#endif

                            if (n_pant != o_pant)
                                {
                                    o_pant = n_pant;
                                    draw_scr ();
#ifdef ENABLE_LAVA

                                    if (flags [LAVA_FLAG] == 1)
                                        lava_reenter ();

#endif
                                }


#ifndef DEACTIVATE_OBJECTS
                            if (p_objs != objs_old)
                                {
                                    draw_objs ();
                                    objs_old = p_objs;
                                }

#endif

                            if (p_life != life_old)
                                {
                                    print_number2 (LIFE_X, LIFE_Y, p_life);
                                    life_old = p_life;
                                }

#ifndef DEACTIVATE_KEYS
                            if (p_keys != keys_old)
                                {
                                    print_number2 (KEYS_X, KEYS_Y, p_keys);
                                    keys_old = p_keys;
                                }

#endif

#if defined(PLAYER_KILLS_ENEMIES) || defined(PLAYER_CAN_FIRE)
#ifdef PLAYER_SHOW_KILLS
#ifdef BODY_COUNT_ON
                            if (flags [BODY_COUNT_ON] != killed_old)
                                {
                                    print_number2 (KILLED_X, KILLED_Y, flags [BODY_COUNT_ON]);
                                    killed_old = flags [BODY_COUNT_ON];
                                }

#else
                            if (p_killed != killed_old)
                                {
                                    print_number2 (KILLED_X, KILLED_Y, p_killed);
                                    killed_old = p_killed;
                                }

#endif
#endif
#endif

#ifdef MAX_AMMO
                            if (p_ammo != ammo_old)
                                {
                                    print_number2 (AMMO_X, AMMO_Y, p_ammo);
                                    ammo_old = p_ammo;
                                }

#endif

#if defined(TIMER_ENABLE) && defined(PLAYER_SHOW_TIMER)
                            if (ctimer.t != timer_old)
                                {
                                    print_number2 (TIMER_X, TIMER_Y, ctimer.t);
                                    timer_old = ctimer.t;
                                }

#endif

#ifdef PLAYER_SHOW_FLAG
                            if (flags [PLAYER_SHOW_FLAG] != flag_old)
                                {
                                    print_number2 (FLAG_X, FLAG_Y, flags [PLAYER_SHOW_FLAG]);
                                    flag_old = flags [PLAYER_SHOW_FLAG];
                                }

#endif

#if defined (PLAYER_HAS_JETPAC) && defined (JETPAC_DEPLETES) && defined (PLAYER_SHOW_FUEL)
                            if (p_fuel != fuel_old)
                                {
                                    print_number2 (FUEL_X, FUEL_Y, p_fuel);
                                    fuel_old = p_fuel;
                                }

#endif

                            maincounter ++;

                            half_life = !half_life;

                            // Move player
                            move ();

                            // Move hitter
#if defined(PLAYER_CAN_PUNCH) || defined(PLAYER_HAZ_SWORD) || defined(PLAYER_HAZ_WHIP)
                            if (hitter_on)
                                render_hitter ();

#endif
#if defined (PHANTOMAS_ENGINE) || defined (HANNA_ENGINE)
                            gpx = p_x;

                            gpy = p_y;

#endif
                            // Move enemies
                            mueve_bicharracos ();

#ifdef CARRIABLE_BOXES_THROWABLE
                            if (n_pant != o_pant)
                                continue;

#endif

#ifdef ENABLE_SHOOTERS
                            move_cocos ();

#endif

#if defined (BREAKABLE_WALLS) || defined (BREAKABLE_WALLS_SIMPLE)
#ifdef BREAKABLE_ANIM
                            // Breakable
                            if (do_process_breakable)
                                process_breakable ();

#endif
#endif

#ifdef PLAYER_SIMPLE_BOMBS
                            // Update bombas
                            bomb_run ();

#endif

#ifdef PLAYER_CAN_FIRE
                            // Move bullets
                            mueve_bullets ();

#endif

#ifdef ENABLE_TILANIMS
                            do_tilanims ();

#endif

                            for (gpit = 0; gpit < 3; gpit ++)
                                {
                                    enoffsmasi = enoffs + gpit;

#if defined (RANDOM_RESPAWN) || defined (ENABLE_CUSTOM_TYPE_6)

                                    if (
#ifdef RANDOM_RESPAWN
                                        en_an_fanty_activo [gpit]
#ifdef ENABLE_CUSTOM_TYPE_6
                                        ||
#endif
#endif
#ifdef ENABLE_CUSTOM_TYPE_6
#ifdef USE_OLD_ENEMS
                                        (baddies [enoffsmasi].t & 0xEF) == 6
#else
                                        (baddies [enoffsmasi].t & 0x78) == 16
#endif
#endif
                                    )
                                        {
                                            gpen_x = en_an_x [gpit] >> 6;
                                            gpen_y = en_an_y [gpit] >> 6;
                                        }

                                    else
                                        {
#endif
                                            gpen_x = baddies [enoffsmasi].x;
                                            gpen_y = baddies [enoffsmasi].y;
#if defined (RANDOM_RESPAWN) || defined (ENABLE_CUSTOM_TYPE_6)
                                        }

#endif
                                    sp_MoveSprAbs (sp_moviles [gpit], spritesClip, en_an_next_frame [gpit] - en_an_current_frame [gpit], VIEWPORT_Y + (gpen_y >> 3), VIEWPORT_X + (gpen_x >> 3),gpen_x & 7, gpen_y & 7);

                                    en_an_current_frame [gpit] = en_an_next_frame [gpit];
                                }

                            if ( !(p_estado & EST_PARP) || half_life == 0)
                                {
                                    sp_MoveSprAbs (sp_player, spritesClip, p_next_frame - p_current_frame, VIEWPORT_Y + (gpy >> 3), VIEWPORT_X + (gpx >> 3), gpx & 7, gpy & 7);
                                }

                            else
                                {
                                    sp_MoveSprAbs (sp_player, spritesClip, p_next_frame - p_current_frame, -2, -2, 0, 0);
                                }

                            p_current_frame = p_next_frame;


#ifdef PLAYER_CAN_FIRE

                            for (gpit = 0; gpit < MAX_BULLETS; gpit ++)
                                {
                                    if (bullets_estado [gpit] == 1)
                                        {
                                            sp_MoveSprAbs (sp_bullets [gpit], spritesClip, 0, VIEWPORT_Y + (bullets_y [gpit] >> 3), VIEWPORT_X + (bullets_x [gpit] >> 3), bullets_x [gpit] & 7, bullets_y [gpit] & 7);
                                        }

                                    else
                                        {
                                            sp_MoveSprAbs (sp_bullets [gpit], spritesClip, 0, -2, -2, 0, 0);
                                        }
                                }
#endif

#ifdef ENABLE_SHOOTERS
                            for (gpit = 0; gpit < MAX_COCOS; gpit ++)
                                {
                                    if (coco_s [gpit] == 1)
                                        {
                                            sp_MoveSprAbs (sp_cocos [gpit], spritesClip, 0, VIEWPORT_Y + (coco_y [gpit] >> 3), VIEWPORT_X + (coco_x [gpit] >> 3), coco_x [gpit] & 7, coco_y [gpit] & 7);
                                        }

                                    else
                                        {
                                            sp_MoveSprAbs (sp_cocos [gpit], spritesClip, 0, -2, -2, 0, 0);
                                        }
                                }

#endif

                            // Update to screen
							//for shooters
                            sp_UpdateNow();

// Experimental
#ifdef ENABLE_LAVA
                            if (flags [LAVA_FLAG] == 1)
                                {
                                    if (do_lava ())
                                        {
#ifdef MODE_128K
                                            kill_player (7);
#else
                                            kill_player (4);
#endif
                                            success = 2;	// repeat
                                            playing = 0;
                                            //continue;
                                        }
                                }

#endif




			//lighting effects
			// Lit?
			
				// Lit?
	//ADDED FOR LIGHTING EFFECTS
	
	//lights off
	if (lit == 0) 
	{
		*((unsigned char *) (24999)) = 0;
		
		player_X = gpx >> 3;
				player_Y = (gpy + gpy) >> 4;//so close!
				
				if (player_Y < 2) 
				{
					player_Y = MAX (2,player_Y);
				}//upwards direction
				//ok at 3
				//ok at 2
				
				if (player_Y > 16) 
				{
					player_Y = MIN (16,player_Y);
				}//downwards direction
				//ok at 16
				//crashes at 17
				
				
				draw_overlay (player_X, player_Y);
				draw_buff ();
				del_overlay (player_X, player_Y);
	} 
	
	//lights on
	if (lit == 1)
	{	
		*((unsigned char *) (24999)) = 1;
	}
			
			sp_UpdateNow();

			


#ifdef PLAYER_CAN_FIRE
                            for (gpit = 0; gpit < 3; gpit ++)
                                if (en_an_morido [gpit] == 1)
                                    {
#ifdef MODE_128K
                                        _AY_PL_SND (7);
#else
                                        peta_el_beeper (1);
#endif
                                        en_an_morido [gpit] = 0;
                                    }

#endif

#ifdef PLAYER_FLICKERS
                            // Flickering
                            if (p_estado == EST_PARP)
                                {
                                    p_ct_estado --;

                                    if (p_ct_estado == 0)
                                        p_estado = EST_NORMAL;
                                }

#endif

                            // Hotspot interaction.
                            if (collide (gpx, gpy, hotspot_x, hotspot_y))
                                {
                                    // Deactivate hotspot
                                    draw_coloured_tile (VIEWPORT_X + (hotspot_x >> 3), VIEWPORT_Y + (hotspot_y >> 3), orig_tile);
                                    gpit = 0;
#ifndef USE_HOTSPOTS_TYPE_3
                                    // Was it an object, key or life boost?

                                    if (hotspots [n_pant].act == 0)
                                        {
                                            p_life += PLAYER_REFILL;
#ifndef DONT_LIMIT_LIFE

                                            if (p_life > PLAYER_LIFE)
                                                p_life = PLAYER_LIFE;

#endif
                                            hotspots [n_pant].act = 2;

#ifdef MODE_128K
                                            _AY_PL_SND (3);

#else
                                            peta_el_beeper (8);

#endif
                                        }

                                    else
                                        {
                                            switch (hotspots [n_pant].tipo)
                                                {
#ifndef DEACTIVATE_OBJECTS

                                                case 1:
#ifdef ONLY_ONE_OBJECT

                                                    if (p_objs == 0)
                                                        {
                                                            p_objs ++;
#ifdef MODE_128K
                                                            _AY_PL_SND (3);
#else
                                                            peta_el_beeper (9);
#endif
                                                        }

                                                    else
                                                        {
#ifdef MODE_128K
                                                            _AY_PL_SND (3);
#else
                                                            peta_el_beeper (4);
#endif
                                                            draw_coloured_tile (VIEWPORT_X + (hotspot_x >> 3), VIEWPORT_Y + (hotspot_y >> 3), 17);
                                                            gpit = 1;
                                                        }

#else
                                                    p_objs ++;

#ifdef OBJECT_COUNT
                                                    flags [OBJECT_COUNT] = player.objs;

#endif
#ifdef MODE_128K
                                                    _AY_PL_SND (3);

#else
                                                    peta_el_beeper (9);

#endif
#endif
                                                    break;

#endif
#ifndef DEACTIVATE_KEYS

                                                case 2:
                                                    p_keys ++;

#ifdef MODE_128K
                                                    _AY_PL_SND (3);

#else
                                                    peta_el_beeper (7);

#endif
                                                    break;

#endif
#ifdef MAX_AMMO

                                                case 4:
                                                    if (MAX_AMMO - p_ammo > AMMO_REFILL)
                                                        p_ammo += AMMO_REFILL;
                                                    else
                                                        p_ammo = MAX_AMMO;

#ifdef MODE_128K
                                                    _AY_PL_SND (3);

#else
                                                    peta_el_beeper (9);

#endif
                                                    break;

#endif

#ifdef TIMER_ENABLE
#ifdef TIMER_REFILL

                                                case 5:
                                                    if (99 - ctimer.t > TIMER_REFILL)
                                                        ctimer.t += TIMER_REFILL;
                                                    else
                                                        ctimer.t = 99;

#ifdef MODE_128K
                                                    _AY_PL_SND (3);

#else
                                                    peta_el_beeper (7);

#endif
                                                    break;

#endif
#endif

#if defined (PLAYER_HAS_JETPAC) && defined (JETPAC_DEPLETES) && defined (JETPAC_REFILLS)

                                                case 6:
                                                    p_fuel += JETPAC_FUEL_REFILL;

                                                    if (p_fuel > JETPAC_FUEL_MAX)
                                                        p_fuel = JETPAC_FUEL_MAX;

#ifdef MODE_128K
                                                    _AY_PL_SND (3);

#else
                                                    peta_el_beeper (9);

#endif
                                                    break;

#endif
                                                }

                                            hotspots [n_pant].act = gpit;
                                        }

                                    hotspot_y = 240;
                                }

#else
                                    // Modification for type 3 hotspots to be direct recharges
                                    // Was it an object, key or life boost?
                                    if (hotspots [n_pant].act)
                                        {
                                            hotspots [n_pant].act = 0;

                                            switch (hotspots [n_pant].tipo)
                                                {
#ifndef DEACTIVATE_OBJECTS

                                                case 1:
#ifdef ONLY_ONE_OBJECT

                                                    if (p_objs == 0)
                                                        {
                                                            p_objs ++;
#ifdef MODE_128K
                                                            _AY_PL_SND (3);
#else
                                                            peta_el_beeper (9);
#endif
                                                        }

                                                    else
                                                        {
#ifdef MODE_128K
                                                            _AY_PL_SND (3);
#else
                                                            peta_el_beeper (4);
#endif
                                                            draw_coloured_tile (VIEWPORT_X + (hotspot_x >> 3), VIEWPORT_Y + (hotspot_y >> 3), 17);
                                                            hotspots [n_pant].act = 1;
                                                        }

#else
                                                    p_objs ++;

#ifdef OBJECT_COUNT
                                                    flags [OBJECT_COUNT] = player.objs;

#endif
#ifdef MODE_128K
                                                    _AY_PL_SND (5);//intro music

#else
                                                    peta_el_beeper (9);

#endif
#ifdef GET_X_MORE
#if defined (MODE_128K) && defined (COMPRESSED_LEVELS) && !defined (HANNA_LEVEL_MANAGER) && !defined (SIMPLE_LEVEL_MANAGER)
                                                    if (level_data.max_objs > p_objs)
                                                        {
#else

                                                            if (PLAYER_NUM_OBJETOS > p_objs)
                                                                {
#endif
                                                                    print_str (10, 11, 79, spacer);
#if defined (MODE_128K) && defined (COMPRESSED_LEVELS) && !defined (HANNA_LEVEL_MANAGER) && !defined (SIMPLE_LEVEL_MANAGER)
                                                                    gpjt = level_data.max_objs - p_objs;
#else
                                                                    gpjt = PLAYER_NUM_OBJETOS - p_objs;
#endif
                                                                    getxmore [8] = '0' + gpjt / 10;
                                                                    getxmore [9] = '0' + gpjt % 10;
                                                                    print_str (10, 12, 79, getxmore);
                                                                    print_str (10, 13, 79, spacer);
                                                                    sp_UpdateNow ();
                                                                    sp_WaitForNoKey ();
                                                                    espera_activa (100);
                                                                    draw_scr_background ();
                                                                }

#endif
                                                            break;

#endif
#endif

#ifndef DEACTIVATE_KEYS

                                                        case 2:
                                                            p_keys ++;

#ifdef MODE_128K
                                                            _AY_PL_SND (3);

#else
                                                            peta_el_beeper (7);

#endif
                                                            break;

#endif

                                                        case 3:
                                                            p_life += PLAYER_REFILL;

#ifndef DONT_LIMIT_LIFE
                                                            if (p_life > PLAYER_LIFE)
                                                                p_life = PLAYER_LIFE;

#endif
#ifdef MODE_128K
                                                            _AY_PL_SND (3);

#else
                                                            peta_el_beeper (8);

#endif
                                                            break;

#ifdef MAX_AMMO

                                                        case 4:
                                                            if (MAX_AMMO - p_ammo > AMMO_REFILL)
                                                                p_ammo += AMMO_REFILL;
                                                            else
                                                                p_ammo = MAX_AMMO;

#ifdef MODE_128K
                                                            _AY_PL_SND (3);

#else
                                                            peta_el_beeper (9);

#endif
                                                            break;

#endif
#ifdef TIMER_ENABLE
#ifdef TIMER_REFILL

                                                        case 5:
                                                            if (99 - ctimer.t > TIMER_REFILL)
                                                                ctimer.t += TIMER_REFILL;
                                                            else
                                                                ctimer.t = 99;

#ifdef MODE_128K
                                                            _AY_PL_SND (3);

#else
                                                            peta_el_beeper (7);

#endif
                                                            break;

#endif
#endif
                                                        }

                                                }

                                            hotspot_y = 240;
                                        }

#endif


#if defined (MSC_MAXITEMS) || defined (ENABLE_SIM)
                            // Select object
                            if (sp_KeyPressed (key_z))
                                {
                                    if (!key_z_pressed)
                                        {
#ifdef MODE_128K
                                            _AY_PL_SND (2);
#else
                                            peta_el_beeper (2);
#endif
#ifdef MSC_MAXITEMS
                                            flags [FLAG_SLOT_SELECTED] = (flags [FLAG_SLOT_SELECTED] + 1) % MSC_MAXITEMS;
#else
                                            flags [FLAG_SLOT_SELECTED] = (flags [FLAG_SLOT_SELECTED] + 1) % SIM_DISPLAY_MAXITEMS;
#endif
                                            display_items ();
                                        }

                                    key_z_pressed = 1;
                                }

                            else
                                {
                                    key_z_pressed = 0;
                                }

#endif

                            // Flick screen checks and scripting related stuff
                            gpit = (joyfunc) (&keys);

							
							
							
							
#ifdef ACTIVATE_SCRIPTING
#ifdef EXTENDED_LEVELS
			if (level_data->activate_scripting) {
#endif
#ifdef SCRIPTING_KEY_M
				if (sp_KeyPressed (key_m)) {
#endif
#ifdef SCRIPTING_DOWN
				if ((gpit & sp_DOWN) == 0) {
#endif
#ifdef SCRIPTING_KEY_FIRE
				if ((gpit & sp_FIRE) == 0) {
#endif
					if (action_pressed == 0)  {
						action_pressed = 1;
						// Any scripts to run in this screen?
						run_fire_script ();
					}
				} else {
					action_pressed = 0;
				}
#ifdef EXTENDED_LEVELS
			}
#endif
#endif

#ifdef ACTIVATE_SCRIPTING
#ifdef ENABLE_FIRE_ZONE
#ifdef EXTENDED_LEVELS
			if (level_data->activate_scripting) {
#endif
				if (f_zone_ac) {
					if (gpx >= fzx1 && gpx <= fzx2 && gpy >= fzy1 && gpy <= fzy2) {
						run_fire_script ();
					}
				}
#ifdef EXTENDED_LEVELS
			}
#endif
#endif
#endif






                                            // Interact w/floating objects
#ifdef ENABLE_FLOATING_OBJECTS
                                            FO_do ();

#endif


#ifdef PAUSE_ABORT
                                            // Pause/Abort handling
                                            if (sp_KeyPressed (key_h))
                                                {
                                                    sp_WaitForNoKey ();
#ifdef MODE_128K
                                                    _AY_ST_ALL ();
                                                    _AY_PL_SND (8);
#endif
                                                    //saca_a_todo_el_mundo_de_aqui (0);
                                                    //pause_screen ();

                                                    while (sp_KeyPressed (key_h) == 0)
                                                        ;

                                                    sp_WaitForNoKey ();

                                                    //draw_scr_background ();
#ifdef ACTIVATE_SCRIPTING
			//run_entering_script ();
#endif
#ifdef MODE_128K
			// Play music
#ifdef COMPRESSED_LEVELS
			//_AY_PL_MUS (level_data->music_id);
#else
			//_AY_PL_MUS (1);
#endif
#endif
                                                }

                                            if (sp_KeyPressed (key_y))
                                                {
                                                    playing = 0;
                                                }

#endif

			// Win game condition
			//i think the problem is here
			//see http://www.mojontwins.com/mojoniaplus/viewtopic.php?f=16&t=1944&p=67302&hilit=WIN+GAME#p67302

#if defined (MODE_128K) && defined (COMPRESSED_LEVELS) && !(defined (SIMPLE_LEVEL_MANAGER) || defined (HANNA_LEVEL_MANAGER))
                                            // 128K
                                            if (
                                                (level_data->win_condition == 0 && p_objs == level_data->max_objs) ||
                                                (level_data->win_condition == 1 && n_pant == level_data->scr_fin)
#ifdef ACTIVATE_SCRIPTING
                                                || (level_data->win_condition == 2 && script_result == 1)
#endif
                                            )
                                                {
#elif defined (MODE_128K) && (defined (HANNA_LEVEL_MANAGER) || defined (SIMPLE_LEVEL_MANAGER))

                                            if (script_result == 1)
                                                {
/*#elif !defined (MODE_128K) && defined(COMPRESSED_LEVELS)
                                            // 48K, compressed levels.

                                            if (
                                                (win_condition == 0 && p_objs == PLAYER_NUM_OBJETOS) ||
                                                (win_condition == 1 && n_pant == SCR_FIN)
#ifdef ACTIVATE_SCRIPTING
                                                || (win_condition == 2 && script_result == 1)
#endif
                                            )*/
                                                {
#else
                                            // 48K, legacy
#if WIN_CONDITION == 0

                                            if (p_objs == PLAYER_NUM_OBJETOS)
                                                {
#elif WIN_CONDITION == 1

                                                    if (n_pant == SCR_FIN)
                                                        {
#elif WIN_CONDITION == 2

                                                            if (script_result == 1 || script_result > 2)
                                                                {
#elif WIN_CONDITION == 3

                                                                    if (flags [FLAG_SLOT_ALLDONE])
                                                                        {
#endif
#endif
                                                    success = 1;	// Next
                                                    playing = 0;
                                                }

                                            // Game over condition

                                            if (p_life == 0
#ifdef ACTIVATE_SCRIPTING
                                                    || script_result == 2
#endif
/*#if defined(TIMER_ENABLE) && defined(TIMER_GAMEOVER_0)
                                                    || ctimer.zero
#endif*/
                                               )
                                                {
                                                    playing = 0;
                                                }

                                            // Warp to level condition (3)
                                            // Game ending (4)
#if defined (COMPRESSED_LEVELS) && defined (MODE_128K)

                                            if (script_result > 2)
                                                {
                                                    success = script_result;	// Warp_to (3), Game ending (4)
                                                    playing = 0;
                                                }

#endif



#ifdef DIE_AND_RESPAWN
                                            // Respawn
                                            if (p_killme)
                                                {
                                                    p_estado = EST_PARP;
                                                    sp_UpdateNow ();
													
													//SOUND THAT PLAYS ON player DEATH
                                                    //wyz_play_sample (0);
													//_AY_PL_SND (18);
													//wyz_play_sound (9);
													_AY_PL_SND (6);
													
                                                    espera_activa (50);
#ifdef PLAYER_HAS_SWIM

                                                    if (p_engine != SENG_SWIM)
                                                        {
#endif

                                                            if (n_pant != p_safe_pant)
                                                                {
                                                                    o_pant = n_pant = p_safe_pant;
                                                                    draw_scr ();
                                                                }

#ifndef DISABLE_AUTO_SAFE_SPOT
                                                            while (
                                                                !(attr (p_safe_x, p_safe_y + 1) & 12) ||
                                                                (attr (p_safe_x, p_safe_y) & 8)
                                                            )
                                                                p_safe_x ++;

#endif
#if defined (PHANTOMAS_ENGINE) || defined (HANNA_ENGINE)
                                                            p_x = p_safe_x << 4;

                                                            p_y = p_safe_y << 4;

#else
                                                            p_x = p_safe_x << 10;

                                                            p_y = p_safe_y << 10;

#endif
                                                            p_vx = 0;

                                                            p_vy = 0;

                                                            p_saltando = 0;

#ifdef PLAYER_HAS_SWIM
                                                        }

#endif

                                                    p_killme = 0;

#ifdef MODE_128K
                                                    // Play music
#ifdef COMPRESSED_LEVELS
                                                    //_AY_PL_MUS (level_data->music_id);
#else
                                                    //_AY_PL_MUS (1);
#endif
                                                    _AY_PL_SND (18);

#endif
                                                }

#endif

                                            // Flick screen
#ifndef PLAYER_CANNOT_FLICK_SCREEN
                                            flick_screen ();

#elif defined (PLAYER_WRAP_AROUND)
                                            // Wrap around!
                                            wrap_around ();

#endif

                                            // Main loop is done!
                                        }

#ifdef MODE_128K
                                    _AY_ST_ALL ();

#endif

                                    saca_a_todo_el_mundo_de_aqui (0);

                                    sp_UpdateNow ();

#ifdef COMPRESSED_LEVELS
                                    switch (success)
                                        {

                                        case 0:
#if defined(TIMER_ENABLE) && defined(TIMER_GAMEOVER_0) && defined(SHOW_TIMER_OVER)

                                            if (ctimer.zero)
                                                time_over ();
                                            else
                                                game_over ();

#else
#ifdef MODE_128K
                                            //_AY_PL_MUS (8);
#endif
                                            //game_over ();//WAS COMMENTED
                                            print_message (" GAME OVER!");//CORRECT AREA ON GAME ENDING
											
											//This is where you LOSE the game lost all lives
											//ADDED RESET ALL Flags
											//LETS RESET ALL OF OUR FLAGS TO 0
											for (sc_c = 0; sc_c < MAX_FLAGS; sc_c ++)
											{
												flags [sc_c] = 0;
											}

#endif
                                            mlplaying = 0;

                                            espera_activa (250);

                                            break;

                                        case 1:
                                            //_AY_PL_MUS (7);
                                            //print_message (" ZONE CLEAR ");
											

                                            level ++;

                                            espera_activa (50);

                                            //do_extern_action (0);
                                            break;

                                        case 3:
                                            blackout_area ();

                                            level = warp_to_level;
                                            break;

                                        case 4:
                                            get_resource (2, 16384);

                                            espera_activa (1000);

                                            _AY_ST_ALL ();
											
											//this is where you WIN the game
											//clear out variables
											//ADDED RESET ALL Flags
											//LETS RESET ALL OF OUR FLAGS TO 0
											for (sc_c = 0; sc_c < MAX_FLAGS; sc_c ++)
											{
												flags [sc_c] = 0;
											}

                                            cortina ();

                                            //_AY_PL_MUS (12);
                                            espera_activa (130);

                                            // credits ();
                                            mlplaying = 0;
                                        }

#ifndef SCRIPTED_GAME_ENDING
                                    if (level == MAX_LEVELS)
                                        {
											//clear out variables
											//ADDED RESET ALL Flags
											//LETS RESET ALL OF OUR FLAGS TO 0
											for (sc_c = 0; sc_c < MAX_FLAGS; sc_c ++)
											{
												flags [sc_c] = 0;
											}
											
                                            game_ending ();
                                            mlplaying = 0;											
                                        }

#endif
                                }

                            cortina ();

#else

                                    if (success)
                                        {
											//clear out variables
											//ADDED RESET ALL Flags
											//LETS RESET ALL OF OUR FLAGS TO 0
											for (sc_c = 0; sc_c < MAX_FLAGS; sc_c ++)
											{
												flags [sc_c] = 0;
											}
                                            game_ending ();
                                        }

                                    else
                                        {
                                            //_AY_PL_MUS (8)
											
											//clear out variables
											//ADDED RESET ALL Flags
											//LETS RESET ALL OF OUR FLAGS TO 0
											for (sc_c = 0; sc_c < MAX_FLAGS; sc_c ++)
											{
												flags [sc_c] = 0;
											}
                                            game_over ();
                                        }

                                    espera_activa (500);
									
                                    cortina ();
#endif
                        }
                }