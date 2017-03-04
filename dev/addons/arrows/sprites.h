// MT Engine MK2
// Copyleft 2014 the Mojon Twins

// arros/sprites.h
// Frame 0: arrow left. Frame 1: arrow right

extern unsigned char arrow_sprites [0];
#asm
	defb 0, 255
	defb 0, 255
	defb 0, 255
	defb 0, 255
	defb 0, 255
	defb 0, 255
	defb 0, 255
	defb 0, 255
._arrow_sprites
	BINARY "sparrow.bin"
#endasm
