// msc-config.h
// Generado por Mojon Script Compiler de la MT Engine MK2
// Copyleft 2014 The Mojon Twins

unsigned char sc_x, sc_y, sc_n, sc_m, sc_c;
unsigned char script_result = 0;
unsigned char sc_terminado = 0;
unsigned char sc_continuar = 0;
unsigned int main_script_offset;
#ifndef MODE_128K
extern unsigned char main_script [0];
#asm
    ._main_script
        BINARY "scripts.bin"
#endasm
#endif
unsigned char warp_to_level;
extern unsigned char *script;
#asm
    ._script defw 0
#endasm

#define SCRIPT_0 0x0000
#define SCRIPT_1 0x0401
#define SCRIPT_2 0x06CA
#define SCRIPT_3 0x0BD4
#define SCRIPT_4 0x1061
#define SCRIPT_5 0x1823
#define SCRIPT_6 0x1F1E
#define SCRIPT_7 0x213B
#define SCRIPT_8 0x24B1
#define SCRIPT_9 0x2754
#define SCRIPT_10 0x3075
#define SCRIPT_11 0x35BB
#define SCRIPT_12 0x3B29
#define SCRIPT_INIT 0xC000 
