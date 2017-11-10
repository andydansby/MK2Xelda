// MT Engine MK2
// Copyleft 2014 the Mojon Twins

// extern.h
// External custom code to be run from a script

// =======[CUSTOM MODIFICATION]=======
unsigned char *textbuff = 23458;//23458
unsigned char *extaddress;
unsigned char exti, extx, exty, stepbystep, keyp;
unsigned char is_cutscene = 0;
extern unsigned char textos_load [0];

void do_extern_action (unsigned char n) {
	// Add custom code here.

	if (n == 0) 
	{
		saca_a_todo_el_mundo_de_aqui (0);
		for (exti = 0; exti < 10; exti ++) {
			for (extx = exti; extx < 30 - exti; extx ++) {
				sp_PrintAtInv (VIEWPORT_Y + exti, VIEWPORT_X + extx, 0, 0);
				sp_PrintAtInv (VIEWPORT_Y + 19 - exti, VIEWPORT_X + extx, 0, 0);
				if (extx < 19 - exti) {
					sp_PrintAtInv (VIEWPORT_Y + extx, VIEWPORT_X + exti, 0, 0);
					sp_PrintAtInv (VIEWPORT_Y + extx, VIEWPORT_X + 29 - exti, 0, 0);
				}
			}
#ifdef MODE_128K
			#asm
				halt                    ; esperamos retrazo. 
			#endasm
#endif
			sp_UpdateNow ();
		}
	}
	
	
	else if (n == 243)
	{
		//BREAKABLE_WALLS_LIFE = 3;//increase breakable wall
		//MAX_BREAKABLE = 3;
	}
	
	//1 stops enemies & 0 enemies walk thru walls
	else if (n == 244)
	{
		wall_stopping = 1;//stops enemies from walking thru walls
	}
	
	else if (n == 245)
	{
		wall_stopping = 0;//enemies can walk thru walls
	}
	
	
	//lights on
	else if (n == 246)
	{
		lit = 1;
		*((unsigned char *) (24999)) = 1;
		//#asm
		//ld hl, 24999
		//ld (hl), 1
		//#endasm
	}	
	//lights off
	else if (n == 247)
	{
		lit = 0;
		*((unsigned char *) (24999)) = 0;
		//#asm
		//ld hl, 24999
		//ld (hl), 0
		//#endasm
	}
	
	else if (n == 249)
	{
		//script_result = 1;
		//sc_terminado = 1;
		//game_ending ();
		//mlplaying = 0;

	}
	
	
	else if (n < 240) 
	{
		
		// Show text n
		stepbystep = 1;
		
		asm_int [0] = (n - 1) << 1;
		#asm
			; First we get where to look for the packed string
			jr aaaa
			defb 1,2,3,4,5,6,7,8
		.aaaa
			
			ld a, (_asm_int)
			ld e, a
			ld a, (_asm_int + 1)
			ld d, a
			
			ld hl, _textos_load
			add hl, de
			ld c, (hl)
			inc hl
			ld b, (hl)
			push bc
			pop de
			ld hl, _textos_load
			add hl, de
			
			ld de, 23458
	
			; 5-bit scaped depacker by na_th_an
			; Contains code by Antonio Villena
			
			ld a, $80
	
		.fbsd_mainb
	        call fbsd_unpackc
	        ld c, a
	        ld a, b
	        and a
	        jr z, fbsd_fin
	        call fbsd_stor
	        ld a, c
	        jr fbsd_mainb	
	
		.fbsd_stor
	        cp 31
	        jr z, fbsd_escaped
	        add a, 64
	        jr fbsd_stor2
		.fbsd_escaped
	        ld a, c
	        call fbsd_unpackc
	        ld c, a
	        ld a, b
	        add a, 32
		.fbsd_stor2
	        ld (de), a
	        inc de
	        ret
	
		.fbsd_unpackc
	        ld      b, 0x08
		.fbsd_bucle
	        call    fbsd_getbit
	        rl      b
	        jr      nc, fbsd_bucle
	        ret
	
		.fbsd_getbit
	        add     a, a
	        ret     nz
	        ld      a, (hl)
	        inc     hl
	        rla
	        ret        
	        
		.fbsd_fin
			ld (de), a	
			;
			;		
			
		#endasm	
		if (is_cutscene == 0) {
			// Show
			exti = textbuff [0] - 64;
			
			// Draw empty frame
			extx = 3 + exti + exti;
			exti = !((VIEWPORT_Y + (p_y >> 9)) < extx);	
			saca_a_todo_el_mundo_de_aqui (exti);
	
			print_str (3, 3, 6, "#$$$$$$$$$$$$$$$$$$$$$$$$%");
			for (exty = 4; exty < extx; exty ++) print_str (3, exty, 6, "&                        '");
			print_str (3, extx, 6, "())))))))))))))))))))))))*");
			
			exty = 4;
		} else {
			saca_a_todo_el_mundo_de_aqui (0);
			exty = 13;
		}
		
		espera_activa (10);
		
		// Draw text
		extx = 4; 
		extaddress = textbuff + 1;
		keyp = 1;
		while (exti = *extaddress ++) {
			if (exti == '%') {
				extx = 4; exty += 2;
			} else {
				sp_PrintAtInv (exty, extx, 71, exti - 32);
				extx ++;
				if (extx == 28) {
					extx = 4; exty += 2;
				}
			}
			if (stepbystep) {
#ifdef MODE_128K
	#asm
				halt
	#endasm
				if (exti != 32 && is_cutscene == 0) wyz_play_sound (9);//3 is not bad
	#asm
				halt
				halt
	#endasm
#endif
				//peta_el_beeper (6);
				sp_UpdateNow ();
			}
			
			if (button_pressed ()) {
				if (keyp == 0) {
					stepbystep = 0;
				} 
			} else {
				keyp = 0;
			}
		}
		sp_UpdateNow ();
		sp_WaitForNoKey ();
		while (button_pressed ());
		espera_activa (5000);
		
		if (is_cutscene) {
			for (exti = 11; exti < 24; exti ++) {
				print_str (3, exti, 71, "                          ");
				sp_UpdateNow ();
			}
		}
	}
	
	else if (n == 251) 
	{
		is_cutscene = 1;
		saca_a_todo_el_mundo_de_aqui (0);
		sp_UpdateNow ();
	} 
	
	else if (n == 250) 
	{
		is_cutscene = 0;
	} 
	else 
	{
		cortina ();
		//unpack ((unsigned int) (ims [n - 252]), 16384);
	}
}

#asm
	._textos_load
		BINARY "texts.bin"
#endasm
