// librarian.h
// Generated by The Librarian
// Copyleft 2012 The Mojon Twins
 
typedef struct {
   unsigned char ramPage;
   unsigned int ramOffset;
} RESOURCE;
 
RESOURCE resources [] = {
   {4, 49152},   // 0: title.bin
   {4, 51160},   // 1: marco.bin
   {4, 51345},   // 2: ending.bin
   {4, 52652},   // 3: level0c.bin
   {4, 55855},   // 4: level1c.bin
   {4, 59178},   // 5: level2c.bin
   {4, 62357},   // 6: level3c.bin
   {6, 49152},   // 7: level4c.bin
   {6, 52253},   // 8: level5c.bin
   {6, 55252},   // 9: level6c.bin
   {6, 58243},   // 10: level7c.bin
   {6, 61137},   // 11: level8c.bin
   {7, 49152},   // 12: level9c.bin
   {7, 51318},   // 13: level10c.bin
   {7, 54187},   // 14: level11c.bin
   {7, 57100},   // 15: level12c.bin
   {7, 60023}    // 16: texts.bin
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
#define LEVEL11C_BIN        	14
#define LEVEL12C_BIN        	15
#define TEXTS_BIN           	16
