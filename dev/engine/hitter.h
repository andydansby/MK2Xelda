// MT Engine MK2
// Copyleft 2014 the Mojon Twins

// hitter_h
// Hitter (punch/sword/whip) helper functions

#ifdef PLAYER_HAZ_SWORD
	//                                     H   H   H   H
	//unsigned char hoffs_x [] = {10, 12, 14, 15, 15, 14, 13, 10};
	unsigned char hoffs_x [] = {10, 12, 14, 15, 15, 14, 12, 10};
	unsigned char hoffs_y [] = {4 ,  4,  4,  4,  4,  4,  4,  4};
	//unsigned char hoffs_y [] = {4 ,  4,  4,  4,  4,  4,  4,  4};
	//unsigned char hoffs_y [] = {2,  2,  2, 3,  4,  4,  5,  6,  7};//sword wiggle
	#define HITTER_MAX_FRAME 8
#endif

void __FASTCALL__ render_hitter (void) {
	
	gpy = (p_y >> 6);
	gpx = (p_x >> 6);

// Sword main code

#ifdef PLAYER_HAZ_SWORD

	//works ok
	if (p_facing == FACING_UP) 
	{		
		//gpy = (p_y >> 6);
		//gpx = (p_x >> 6);
		
		hitter_x = 3 + gpx;
		//how the sprite travels width from character		
		hitter_y = (gpy + 1) - hoffs_x[hitter_frame];
		//hitter_y = gpy - 1 - hoffs_x[hitter_frame];
		//how the sprite travels height from character		
		hitter_next_frame = sprite_sword_u;//which sprite to use

		gpxx = (hitter_x + 4) >> 4;
		gpyy = (hitter_y) >> 4;
	}


	//works ok
	if (p_facing == FACING_DOWN) 
	{
		//gpy = (p_y >> 6);
		//gpx = (p_x >> 6);
		
		hitter_x = 3 + gpx;// + hoffs_x[hitter_frame];
		//how the sprite travels width from character
		hitter_y = gpy + 6 + hoffs_x[hitter_frame];
		//how the sprite travels height from character
		hitter_next_frame = sprite_sword_d;//which sprite to use
		
		gpxx = (hitter_x + 4) >> 4;		
		gpyy = (hitter_y) >> 4;
	}

	//works ok
	if (p_facing == FACING_LEFT) 
	{
		//gpy = (p_y >> 6);
		//gpx = (p_x >> 6);	
	
		hitter_x = 6 + gpx - hoffs_x [hitter_frame];
		//how the sprite travels width from character
		
		hitter_y = (gpy + 4) + hoffs_y [hitter_frame];
		//how the sprite travels height from character
		
		hitter_next_frame = sprite_sword_l;//which sprite to use
		
		gpxx = (hitter_x - 6) >> 4; //gpxx = (hitter_x - 7) >> 4; 
		gpyy = (hitter_y) >> 4;
	}

	//works ok
	if (p_facing == FACING_RIGHT) 
	{
		//gpy = (p_y >> 6);
		//gpx = (p_x >> 6);
		
		hitter_x = 6 + gpx + hoffs_x [hitter_frame];		
		//how the sprite travels width from character
		
		hitter_y = (gpy + 4) + hoffs_y [hitter_frame];
		//how the sprite travels height from character
		
		hitter_next_frame = sprite_sword_r;		
		
		gpxx = (hitter_x + 5) >> 4;//gpxx = (hitter_x + 7) >> 4;
		gpyy = (hitter_y) >> 4;
	}

	
		//print_number2(1,1,gpx);		
		//p_x hex, cannot determine pattern
		//gpx = 00 to 255? getts odd after 99

		
#if defined (BREAKABLE_WALLS) || defined (BREAKABLE_WALLS_SIMPLE)
	if (hitter_frame > 0 && hitter_frame < 7)
		break_wall (gpxx, gpyy);
#endif
#endif

// End of main codes.

	sp_MoveSprAbs (sp_hitter, spritesClip,
		hitter_next_frame - hitter_current_frame,
		VIEWPORT_Y + (hitter_y >> 3), VIEWPORT_X + (hitter_x >> 3),
		hitter_x & 7, hitter_y & 7);
	hitter_current_frame = hitter_next_frame;

	hitter_frame ++;
	if (hitter_frame == HITTER_MAX_FRAME) {
		hitter_on = 0;
		sp_MoveSprAbs (sp_hitter, spritesClip, 0, -2, -2, 0, 0);
		p_hitting = 0;
	}
}
