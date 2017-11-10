rem pause
REM echo move decorations0.spt ..\script
REM move decorations0.spt ..\script


@echo off
echo BUILDING LEVELS!
echo ***********************************
echo CONCATENATING SCRIPTS
echo because the script file is large
echo we write them in seperate files
echo and then will combine all of them
echo ***********************************

cd ..\script
call combinelevels.bat

copy variables.spt + level0.spt + level1.spt + level2.spt + level3.spt + level4.spt + level5.spt + level6.spt + level7.spt + level8.spt + level9.spt + level10.spt + level11.spt + level12.spt   xelda.spt

cd ..\levels

echo ***********************************
echo FINISH CONCATENATING SCRIPTS
echo ***********************************




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

echo ================
echo LEVEL 0

..\utils\buildlevel.exe ..\map\map0.map 4 4 99 ..\gfx\font.png ..\gfx\work0.png ..\gfx\sprites0.png ..\enems\enems0.ene 10 9 8 99 2 behs0.txt level0.bin decorations0.spt

move decorations0.spt ../script

..\utils\apack.exe level0.bin ..\bin\level0c.bin


echo ================
echo LEVEL 1

..\utils\buildlevel.exe ..\map\map1.map 4 4 99 ..\gfx\font.png ..\gfx\work1.png ..\gfx\sprites1.png ..\enems\enems1.ene 16 2 5 99 2 behs1.txt level1.bin decorations1.spt
move decorations1.spt ../script
..\utils\apack.exe level1.bin ..\bin\level1c.bin


echo ================
echo LEVEL 2

..\utils\buildlevel.exe ..\map\map2.map 4 4 99 ..\gfx\font.png ..\gfx\work2.png ..\gfx\sprites2.png ..\enems\enems2.ene 16 2 5 99 2 behs2.txt level2.bin decorations2.spt
move decorations2.spt ../script
..\utils\apack.exe level2.bin ..\bin\level2c.bin


echo ================
echo LEVEL 3

..\utils\buildlevel.exe ..\map\map3.map 4 4 99 ..\gfx\font.png ..\gfx\work3.png ..\gfx\sprites3.png ..\enems\enems3.ene 16 2 5 99 2 behs3.txt level3.bin decorations3.spt

move decorations3.spt ../script
..\utils\apack.exe level3.bin ..\bin\level3c.bin



echo ================
echo LEVEL 4

..\utils\buildlevel.exe ..\map\map4.map 4 4 99 ..\gfx\font.png ..\gfx\work4.png ..\gfx\sprites4.png ..\enems\enems4.ene 16 2 5 99 2 behs4.txt level4.bin decorations4.spt

move decorations4.spt ../script
..\utils\apack.exe level4.bin ..\bin\level4c.bin



echo ================
echo LEVEL 5

..\utils\buildlevel.exe ..\map\map5.map 4 4 99 ..\gfx\font.png ..\gfx\work5.png ..\gfx\sprites5.png ..\enems\enems5.ene 16 2 5 99 2 behs5.txt level5.bin decorations5.spt

move decorations5.spt ../script
..\utils\apack.exe level5.bin ..\bin\level5c.bin

REM pause 

echo ================
echo LEVEL 6

..\utils\buildlevel.exe ..\map\map6.map 4 4 99 ..\gfx\font.png ..\gfx\work6.png ..\gfx\sprites6.png ..\enems\enems6.ene 16 2 5 99 2 behs6.txt level6.bin decorations6.spt

move decorations6.spt ../script

..\utils\apack.exe level6.bin ..\bin\level6c.bin


echo ================
echo LEVEL 7

..\utils\buildlevel.exe ..\map\map7.map 4 4 99 ..\gfx\font.png ..\gfx\work7.png ..\gfx\sprites7.png ..\enems\enems7.ene 16 2 5 99 2 behs7.txt level7.bin decorations7.spt

move decorations7.spt ../script

..\utils\apack.exe level7.bin ..\bin\level7c.bin


echo ================
echo LEVEL 8

..\utils\buildlevel.exe ..\map\map8.map 4 4 99 ..\gfx\font.png ..\gfx\work8.png ..\gfx\sprites8.png ..\enems\enems8.ene 16 2 5 99 2 behs8.txt level8.bin decorations8.spt

move decorations8.spt ../script

..\utils\apack.exe level8.bin ..\bin\level8c.bin


echo ================
echo MAIN WORLD DONE


echo ================
echo LEVEL 9 INSIDE HOUSES

..\utils\buildlevel.exe ..\map\map9.map 4 4 99 ..\gfx\font.png ..\gfx\work9.png ..\gfx\sprites9.png ..\enems\enems9.ene 1 2 5 99 2 behs9.txt level9.bin decorations9.spt

move decorations9.spt ../script

..\utils\apack.exe level9.bin ..\bin\level9c.bin


echo ================
echo LEVEL 10 Dungeon 1

..\utils\buildlevel.exe ..\map\map10.map 4 4 99 ..\gfx\font.png ..\gfx\work10.png ..\gfx\sprites10.png ..\enems\enems10.ene 15 2 5 99 2 behs10.txt level10.bin decorations10.spt

move decorations10.spt ../script

..\utils\apack.exe level10.bin ..\bin\level10c.bin

echo ================
echo LEVEL 11 Dungeon 2

..\utils\buildlevel.exe ..\map\map11.map 4 4 99 ..\gfx\font.png ..\gfx\work11.png ..\gfx\sprites11.png ..\enems\enems11.ene 15 2 5 99 2 behs11.txt level11.bin decorations11.spt

move decorations11.spt ../script

..\utils\apack.exe level11.bin ..\bin\level11c.bin


echo ================
echo LEVEL 12 Dungeon 3

..\utils\buildlevel.exe ..\map\map12.map 4 4 99 ..\gfx\font.png ..\gfx\work12.png ..\gfx\sprites12.png ..\enems\enems12.ene 15 2 5 99 2 behs12.txt level12.bin decorations12.spt

move decorations12.spt ../script

..\utils\apack.exe level12.bin ..\bin\level12c.bin



REM pause

