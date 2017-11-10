// MT Engine MK2
// Copyleft 2014 the Mojon Twins

// enems.h
// General enemy definitions. Incluses enems.bin and hotspots.bin as generated by ene2bin.exe

// This is BROKEN. For the time being.
#define BADDIES_COUNT 33
// End of BROKEN stuff

typedef struct {
	int x, y;
	unsigned char x1, y1, x2, y2;
	signed char mx, my;
	char t;
	unsigned char life;
} BADDIE;

typedef struct {
	unsigned char xy, tipo, act;
} HOTSPOT;

extern BADDIE baddies [0];
extern HOTSPOT hotspots [0];

#asm
	._baddies
		BINARY "enems.bin"
	._hotspots
		BINARY "hotspots.bin"
#endasm
