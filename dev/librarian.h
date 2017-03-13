// librarian.h
// Generated by The Librarian
// Copyleft 2012 The Mojon Twins
 
typedef struct {
   unsigned char ramPage;
   unsigned int ramOffset;
} RESOURCE;
 
RESOURCE resources [] = {
   {3, 51114},   // 0: title.bin
   {3, 53764},   // 1: marco.bin
   {3, 53964},   // 2: ending.bin
   {3, 56648},   // 3: level0c.bin
   {3, 59291},   // 4: level1c.bin
   {3, 62065},   // 5: level2c.bin
   {4, 49152},   // 6: level3c.bin
   {4, 51418},   // 7: level4c.bin
   {4, 51418},   // 8: level5c.bin
   {4, 51418},   // 9: level6c.bin
   {4, 51418},   // 10: level7c.bin
   {4, 51418},   // 11: level8c.bin
   {4, 51418},   // 12: level9c.bin
   {4, 51418}    // 13: level10c.bin
};
 
void get_resource (unsigned char res, unsigned int dest) {
   unpack_RAMn (resources [res].ramPage, resources [res].ramOffset, dest);
}
 
// Defines
#define TITLE_BIN           	0
#define MARCO_BIN           	1
#define ENDING_BIN          	2
#define LEVEL0C_BIN         	3
#define LEVEL1C_BIN         	4
#define LEVEL2C_BIN         	5
#define LEVEL3C_BIN         	6
#define LEVEL4C_BIN         	7
#define LEVEL5C_BIN         	8
#define LEVEL6C_BIN         	9
#define LEVEL7C_BIN         	10
#define LEVEL8C_BIN         	11
#define LEVEL9C_BIN         	12
#define LEVEL10C_BIN        	13
