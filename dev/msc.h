// msc.h
// Generado por Mojon Script Compiler de la MT Engine MK2
// Copyleft 2014 The Mojon Twins

#ifdef CLEAR_FLAGS
void msc_init_all (void) {
    for (sc_c = 0; sc_c < MAX_FLAGS; sc_c ++)
        flags [sc_c] = 0;
}
#endif

unsigned char read_byte (void) {
    unsigned char sc_b;
#ifdef MODE_128K
    #asm
        di
        ld b, SCRIPT_PAGE
        call SetRAMBank
    #endasm
#endif
    sc_b = *script ++;
#ifdef MODE_128K
    #asm
        ld b, 0
        call SetRAMBank
        ei
    #endasm
#endif
    return sc_b;
}

unsigned char __FASTCALL__ read_vbyte (void) {
    sc_c = read_byte ();
    return (sc_c & 128) ? flags [sc_c & 127] : sc_c;
}

void __FASTCALL__ readxy (void) {
    sc_x = read_vbyte ();
    sc_y = read_vbyte ();
}

#if !(defined (PHANTOMAS_ENGINE) || defined (HANNA_ENGINE))
void __FASTCALL__ stop_player (void) {
    p_vx = p_vy = 0;
}
#endif

void __FASTCALL__ reloc_player (void) {
#if defined (PHANTOMAS_ENGINE) || defined (HANNA_ENGINE)
    p_x = read_vbyte () << 4;
    p_y = read_vbyte () << 4;
#else
    p_x = read_vbyte () << 10;
    p_y = read_vbyte () << 10;
    stop_player ();
#endif
}

void run_script (unsigned char whichs) {
    unsigned char *next_script;

    // main_script_offset contains the address of scripts for current level
    asm_int [0] = main_script_offset + whichs + whichs;

#ifdef MODE_128K
    #asm
        di
        ld b, SCRIPT_PAGE
        call SetRAMBank
    #endasm
#endif
    #asm
        ld hl, (_asm_int)
        ld a, (hl)
        ld (_asm_int_2), a
        inc hl
        ld a, (hl)
        ld (_asm_int_2 + 1), a
    #endasm

    script = (unsigned char *) (asm_int_2 [0]);

#ifdef MODE_128K
    #asm
        ld b, 0
        call SetRAMBank
        ei
    #endasm
#endif

    if (script == 0)
        return;

    script += main_script_offset;


    while ((sc_c = read_byte ()) != 0xFF) {
        next_script = script + sc_c;
        sc_terminado = sc_continuar = 0;
        while (!sc_terminado) {
            switch (read_byte ()) {
                case 0x10:
                    // IF FLAG sc_x = sc_n
                    // Opcode: 10 sc_x sc_n
                    readxy ();
                    sc_terminado = (flags [sc_x] != sc_y);
                    break;
                case 0x11:
                    // IF FLAG sc_x < sc_n
                    // Opcode: 11 sc_x sc_n
                    readxy ();
                    sc_terminado = (flags [sc_x] >= sc_y);
                    break;
                case 0x12:
                    // IF FLAG sc_x > sc_n
                    // Opcode: 12 sc_x sc_n
                    readxy ();
                    sc_terminado = (flags [sc_x] <= sc_y);
                    break;
                case 0x13:
                    // IF FLAG sc_x <> sc_n
                    // Opcode: 13 sc_x sc_n
                    readxy ();
                    sc_terminado = (flags [sc_x] == sc_y);
                    break;
                case 0x20:
                    // IF PLAYER_TOUCHES sc_x, sc_y
                    // Opcode: 20 sc_x sc_y
                    readxy ();
#if defined (PHANTOMAS_ENGINE) || defined (HANNA_ENGINE)
                    sc_terminado = (!(p_x >= (sc_x << 4) - 15 && p_x <= (sc_x << 4) + 15 && p_y >= (sc_y << 4) - 15 && p_y <= (sc_y << 4) + 15));
#else
                    sc_terminado = (!((p_x >> 6) >= (sc_x << 4) - 15 && (p_x >> 6) <= (sc_x << 4) + 15 && (p_y >> 6) >= (sc_y << 4) - 15 && (p_y >> 6) <= (sc_y << 4) + 15));
#endif
                    break;
                case 0x21:
                    // IF PLAYER_IN_X x1, x2
                    // Opcode: 21 x1 x2
                    sc_x = read_byte ();
                    sc_y = read_byte ();
#if defined (PHANTOMAS_ENGINE) || defined (HANNA_ENGINE)
                    sc_terminado = (!(p_x >= sc_x && p_x <= sc_y));
#else
                    sc_terminado = (!((p_x >> 6) >= sc_x && (p_x >> 6) <= sc_y));
#endif
                    break;
                case 0x22:
                    // IF PLAYER_IN_Y y1, y2
                    // Opcode: 22 y1 y2
                    sc_x = read_byte ();
                    sc_y = read_byte ();
#if defined (PHANTOMAS_ENGINE) || defined (HANNA_ENGINE)
                    sc_terminado = (!(p_y >= sc_x && p_y <= sc_y));
#else
                    sc_terminado = (!((p_y >> 6) >= sc_x && (p_y >> 6) <= sc_y));
#endif
                    break;
                case 0xF0:
                     // IF TRUE
                     // Opcode: F0
                     break;
                case 0xFF:
                    // THEN
                    // Opcode: FF
                    sc_terminado = 1;
                    sc_continuar = 1;
                    break;
            }
        }
        if (sc_continuar) {
            sc_terminado = 0;
            while (!sc_terminado) {
                switch (read_byte ()) {
                    case 0x01:
                        // SET FLAG sc_x = sc_n
                        // Opcode: 01 sc_x sc_n
                        readxy ();
                        flags [sc_x] = sc_y;
                        break;
                    case 0x10:
                        // INC FLAG sc_x, sc_n
                        // Opcode: 10 sc_x sc_n
                        readxy ();
                        flags [sc_x] += sc_y;
                        break;
                    case 0x11:
                        // DEC FLAG sc_x, sc_n
                        // Opcode: 11 sc_x sc_n
                        readxy ();
                        flags [sc_x] -= sc_y;
                        break;
                    case 0x15:
                        // FLIPFLOP sc_x
                        // Opcode: 15 sc_x
                        sc_x = read_vbyte ();
                        flags [sc_x] = 1 - flags [sc_x];
                        break;
                    case 0x20:
                        // SET TILE (sc_x, sc_y) = sc_n
                        // Opcode: 20 sc_x sc_y sc_n
                        readxy ();
                        sc_n = read_vbyte ();
                        update_tile (sc_x, sc_y, behs [sc_n], sc_n);
                        break;
                    case 0x50:
                        // PRINT_TILE_AT (sc_x, sc_y) = sc_n
                        // Opcode: 50 sc_x sc_y sc_n
                        readxy ();
                        draw_coloured_tile (sc_x, sc_y, read_vbyte ());
                        break;
                    case 0x51:
                        // SET_FIRE_ZONE x1, y1, x2, y2
                        // Opcode: 51 x1 y1 x2 y2
                        fzx1 = read_byte ();
                        fzy1 = read_byte ();
                        fzx2 = read_byte ();
                        fzy2 = read_byte ();
                        f_zone_ac = 1;
                        break;
                    case 0x69:
                        // WARP_TO_LEVEL
                        // Opcode: 69 l n_pant x y silent
                        warp_to_level = read_vbyte ();
                        n_pant = read_vbyte ();
                        o_pant = 99;
                        reloc_player ();
                        silent_level = read_vbyte ();
                        sc_terminado = 1;
                        script_result = 3;
                        return;
                    case 0x6D:
                        // WARP_TO sc_n
                        // Opcode: 6D sc_n
                        n_pant = read_vbyte ();
                        o_pant = 99;
                        reloc_player ();
                        sc_terminado = 1;
                        return;
                    case 0x6E:
                        // REDRAW
                        // Opcode: 6E
                        sc_x = sc_y = 0;
                        for (sc_c = 0; sc_c < 150; sc_c ++) {
                            update_tile (sc_x, sc_y, map_attr [sc_c], map_buff [sc_c]);
                            sc_x ++; if (sc_x == 15) { sc_x = 0; sc_y ++; }
						}
#ifdef ENABLE_FLOATING_OBJECTS
                        FO_paint_all ();
#endif
						break;
                    case 0x6F:
                        // REENTER
                        // Opcode: 6F
                        do_respawn = 0;
                        o_pant = 99; 
                        sc_terminado = 1;
                        return;
                    case 0x70:
                        // SET_TIMER a, b
                        // Opcode: 0x70 a b
                        ctimer.t = read_vbyte ();
                        ctimer.frames = read_vbyte ();
                        ctimer.count = ctimer.zero = 0;
                        break;
                    case 0x71:
                        // TIMER_START
                        // Opcode: 0x71
                        ctimer.on = 1;
                        break;
                    case 0x73:
                        // REHASH
                        // Opcode: 73
                        do_respawn = 0;
                        o_pant = 99; 
                        sc_terminado = 1;
                        no_draw = 1;
                        return;
                    case 0x80:
                        // ENEM n ON
                        // Opcode: 0x80 n
                        baddies [enoffs + read_vbyte ()].t &= 0x7F;
                        break;
                    case 0xE2:
                        // RECHARGE
                        p_life = PLAYER_LIFE;
                        break;
                    case 0xE4:
                        // EXTERN sc_n
                        // Opcode: 0xE4 sc_n
                        do_extern_action (read_byte ());
                        break;
                    case 0xE6:
                        // MUSIC n
#ifdef COMPRESSED_LEVELS
                        level_data->music_id = read_vbyte ();
                        wyz_play_music (level_data->music_id);
#else
                        wyz_play_music (read_vbyte ());
#endif
                        break;
                    case 0xF2:
                        // BREAK
                        sc_terminado = 1;
                        return;
                    case 0xF3:
                        // Final del todo
                        script_result = 4;
                        sc_terminado = 1;
                        break;
                    case 0xF4:
                        // DECORATIONS
                        while (0xff != (sc_x = read_byte ())) {
                            sc_n = read_byte ();
                            update_tile (sc_x >> 4, sc_x & 15, behs [sc_n], sc_n);
                        }
                        break;
                    case 0xFF:
                        sc_terminado = 1;
                        break;
                }
            }
        }
        script = next_script;
    }
}
