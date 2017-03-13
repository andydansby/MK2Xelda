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
#define SCRIPT_1 0x02CB
#define SCRIPT_2 0x03FB
#define SCRIPT_3 0x0482
#define SCRIPT_4 0x04FA
#define SCRIPT_5 0x055B
#define SCRIPT_6 0x05BC
#define SCRIPT_7 0x061D
#define SCRIPT_8 0x067E
#define SCRIPT_9 0x06DF
#define SCRIPT_10 0x0749
#define SCRIPT_INIT 49152
