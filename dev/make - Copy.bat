@echo off

rem set here the game name (used in filenames @ %game%)
SET game=xelda

echo ----------------------------
echo    BUILDING %game%
echo ----------------------------

rem ###########################
rem ## LEVELS
rem ###########################

rem we will delegate on buildlevels.bat - if your game supports several levels,etc
echo ### BUILDING LEVELS
cd ..\levels
call buildlevels.bat
cd ..\dev

rem pause


rem but for 48K/single level games... 
rem echo ### MAKING MAPS ###

rem the "Force" parameter is to force 16 tiles maps even if the actual map data
rem has more tan 16 tiles. Extra tiles are written to extra.spt. Me have to move
rem that file to the script folder.

REM this was commented out before
rem ..\utils\map2bin.exe ..\map\mapa.map 4 4 99 map.bin bolts.bin force
rem move map.bin.spt ..\script

REM this was commented out before
REM echo ### MAKING ENEMS ###
REM ..\utils\ene2bin.exe 4 4 1 ..\enems\enems.ene enems.bin hotspots.bin

REM this was commented out before
REM echo ### MAKING TILESET ###
REM ..\utils\ts2bin.exe ..\gfx\font.png ..\gfx\work.png ts.bin

REM this was commented out before
REM echo ### MAKING SPRITESET ###
REM ..\utils\sprcnv.exe ..\gfx\sprites.png sprites.h

rem If you use arrows and/or drops this will make the sprites binary:

..\utils\spg2bin.exe ..\gfx\drop.png spdrop.bin
..\utils\spg2bin.exe ..\gfx\arrow.png sparrow.bin


rem ###########################
rem ## GAME TEXT
rem ###########################

rem Each line in text.txt contains a text string for the game.
rem textstuffer.exe will compress and pack all text strings in
rem a binary file called texts.bin. The parameter "24" define how
rem many chars per line. Word wrapping is automatic.

echo ### MAKING TEXTS ###
cd ..\texts
echo ..\utils\textstuffer.exe texts.txt texts.bin 24
..\utils\textstuffer.exe texts.txt texts.bin 24
copy texts.bin ..\bin
REM pause

rem ###########################
rem ## GAME SCRIPT
rem ###########################

rem The game script is compiled by msc3.exe. For 128K games use "rampage" at
rem the end so the script compiler generates code to stuff everything in
rem extra pages; the second parameter is the # of screens in your game.
rem i.e. "msc3.exe ninjajar.spt 21 rampage

echo ### MAKING SCRIPT ###
cd ..\script
echo ..\utils\msc3.exe %game%.spt 20 rampage
rem ..\utils\msc3.exe %game%.spt 20 rampage
..\utils\msc3.exe %game%.spt 20 rampage


rem If scripts and texts are going to share the same RAM page, use this line
rem (for 128K games)
rem This calculates an offset for the scripts binary automaticly.

..\utils\sizeof.exe ..\bin\texts.bin 49152 "#define SCRIPT_INIT" >> msc-config.h
echo "..\utils\sizeof.exe ..\bin\texts.bin 49152 "#define SCRIPT_INIT" >> msc-config.h"

echo copy msc.h ..\dev
move msc.h ..\dev
rem copy msc.h ..\dev

rem pause

echo copy msc-config.h ..\dev
move msc-config.h ..\dev
REM copy msc-config.h ..\dev
rem copy scripts.bin ..\dev
cd ..\dev
REM pause

rem For 128K games with text + script sharing the same page, use this to
rem bundle both binaries...
rem echo ### BUNDLING TEXT + SCRIPT ###
echo copy /b ..\texts\texts.bin + ..\script\scripts.bin ..\bin\preload3.bin
copy /b ..\texts\texts.bin + ..\script\scripts.bin ..\bin\preload3.bin

rem ###########################
rem ## FIXED SCREENS
rem ###########################

echo ### MAKING FIXED SCREENS ###

echo #### TITLE
echo ..\utils\png2scr.exe ..\gfx\title.png ..\gfx\title.scr
..\utils\png2scr.exe ..\gfx\title.png ..\gfx\title.scr
echo ..\utils\apack.exe ..\gfx\title.scr ..\bin\title.bin
..\utils\apack.exe ..\gfx\title.scr ..\bin\title.bin

echo #### ENDING
echo ..\utils\png2scr.exe ..\gfx\ending.png ..\gfx\ending.scr
..\utils\png2scr.exe ..\gfx\ending.png ..\gfx\ending.scr
echo ..\utils\apack.exe ..\gfx\ending.scr ..\bin\ending.bin
..\utils\apack.exe ..\gfx\ending.scr ..\bin\ending.bin

echo #### MARCO
echo ..\utils\png2scr.exe ..\gfx\marco.png ..\gfx\marco.scr
..\utils\png2scr.exe ..\gfx\marco.png ..\gfx\marco.scr
echo ..\utils\apack.exe ..\gfx\marco.scr ..\bin\marco.bin
..\utils\apack.exe ..\gfx\marco.scr ..\bin\marco.bin
echo #### POLO

del ..\gfx\*.scr
REM pause

echo ### MAKING LOADING ###
echo ..\utils\png2scr.exe ..\gfx\loading.png loading.bin
..\utils\png2scr.exe ..\gfx\loading.png loading.bin
REM pause


rem ###########################
rem ## LIBRARIAN
rem ###########################
echo ### BUILDING RAMS ###
cd ..\bin
..\utils\librarian.exe
copy ram?.bin ..\dev
copy librarian.h ..\dev
cd ..\dev
REM pause

rem ###########################
rem ## MUSIC
rem ###########################
REM echo ### BUILDING ARKOS ###
REM cd ..\mus
REM if [%1]==[nomus] goto :nomus
REM ..\utils\build_mus_bin.exe ram1.bin
REM :nomus
REM copy ram1.bin ..\dev
REM copy arkos-addresses.h ..\dev\sound
REM cd ..\dev

echo ### COMPILING WYZ PLAYER ###
cd ..\mus
echo ..\utils\pasmo WYZproPlay47aZX.ASM ram1.bin
REM ..\utils\pasmo WYZproPlay47aZX.ASM ram1.bin
pasmo WYZproPlay47aZX.ASM ram1.bin
REM pause

echo copy ram1.bin ..\dev
copy ram1.bin ..\dev
cd ..\dev
echo echo -------------------------
REM pause



rem ##################################
rem ## COMPILATION AND TAPE BUILDING
rem ##################################

echo ### COMPILING ###
zcc +zx -vn %game%.c -o %game%.bin -lsplib2 -zorg=24200

echo ### MAKING TAPS ###
..\utils\bas2tap -a10 -sFINAL loader\loader.bas loader.tap
..\utils\bin2tap -o loading.tap -a 16384 loading.bin
..\utils\bin2tap -o reubica.tap -a 25000 loader\reubica.bin
..\utils\bin2tap -o ram1.tap -a 32768 ram1.bin
..\utils\bin2tap -o ram3.tap -a 32768 ram3.bin
..\utils\bin2tap -o main.tap -a 24200 %game%.bin
copy /b loader.tap + loading.tap + reubica.tap + ram1.tap + ram3.tap + main.tap %game%.tap

echo ### LIMPIANDO ###
del loader.tap
del main.tap
del loading.tap
del %game%.bin
del ram1.tap
del ram1.bin
del ram3.tap
del ram3.bin
del ram4.bin
del ram6.bin
del ram7.bin
del sparrow.bin
del spdrop.bin
del reubica.tap
echo ### DONE ###
