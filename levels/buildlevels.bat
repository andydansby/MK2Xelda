@echo off
echo BUILDING LEVELS!
echo ================
echo LEVEL 1


REM ..\utils\buildlevel.exe ..\map\mapa1.map 4 5 99 ..\gfx\font.png ..\gfx\work1.png ..\gfx\sprites1.png ..\enems\enems1.ene 14 3 8 99 2 behs1.txt level1.bin decorations1.spt
echo ..\utils\buildlevel.exe ..\map\mapa0.map 4 5 99 ..\gfx\font.png ..\gfx\work0.png ..\gfx\sprites0.png ..\enems\enems0.ene 14 3 8 99 2 behs0.txt level0.bin

..\utils\buildlevel.exe ..\map\mapa0.map 4 5 99 ..\gfx\font.png ..\gfx\work0.png ..\gfx\sprites0.png ..\enems\enems0.ene 14 3 8 99 2 behs0.txt level0.bin

echo ..\utils\apack.exe level0.bin ..\bin\level0c.bin
..\utils\apack.exe level0.bin ..\bin\level0c.bin

rem pause
REM echo move decorations0.spt ..\script
REM move decorations0.spt ..\script




echo DONE!
REM pause

REM $ buildlevel mapa.map map_w map_h lock font.png work.png spriteset.png enems.ene scr_ini x_ini y_ini max_objs enems_life behs.txt level.bin [decorations.spt]

REM where:
REM   * mapa.map is your map from mappy .map
REM   * map_w, map_h are map dimmensions in screens
REM   * lock is 15 to autodetect lock, 99 otherwise
REM     [Use 99 if you #define DEACTIVATE_KEYS]
REM   * font.png is a 256x16 file with 64 chars ascii 32-95
REM   * work.png is a 256x48 file with your 16x16 tiles
REM   * spriteset.png is a 256x32 file with your spriteset
REM   * enems.ene enems/hotspots directly from colocador.exe
REM   * scr_ini, scr_x, scr_y, max_objs, enems_life general level data header
REM   * behs.txt is a tile behaviours file
REM   * level.bin is the output filename.
REM   * decorations.spt if specified, maps are forced to 16 tiles + decorations