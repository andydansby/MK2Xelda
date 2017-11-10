@echo off

rem set here the game name (used in filenames @ %game%)
SET game=xelda

echo ###########################
echo    BUILDING %game%
echo ###########################

echo ###########################
echo ## LEVELS
echo ###########################
rem we will delegate on buildlevels.bat - if your game supports several levels,etc
cd ..\levels
call buildlevels.bat
cd ..\dev

REM pause


echo ###########################
echo ## GAME TEXT
echo ###########################
rem Each line in text.txt contains a text string for the game.
rem textstuffer.exe will compress and pack all text strings in
rem a binary file called texts.bin. The parameter "24" define how
rem many chars per line. Word wrapping is automatic.

cd ..\texts
echo ..\utils\textstuffer.exe xeldatexts.asc texts.bin 24
rem ..\utils\textstuffer.exe texts.txt texts.bin 24
..\utils\textstuffer.exe xeldatexts.asc texts.bin 24

copy texts.bin ..\bin
copy texts.bin ..\dev

REM pause


echo ###########################
echo ## GAME SCRIPT
echo ###########################
rem The game script is compiled by msc3.exe. For 128K games use "rampage" at
rem the end so the script compiler generates code to stuff everything in
rem extra pages; the second parameter is the # of screens in your game.
rem i.e. "msc3.exe ninjajar.spt 21 rampage

cd ..\script
echo ..\utils\msc3.exe %game%.spt 16 rampage

rem ..\utils\msc3.exe %game%.spt 20 rampage
..\utils\msc3.exe %game%.spt 16 rampage

rem If scripts and texts are going to share the same RAM page
rem use this line (for 128K games)
rem This calculates an offset for the scripts binary automaticly.

REM ..\utils\sizeof.exe ..\bin\texts.bin 49152 "#define SCRIPT_INIT" >> msc-config.h
REM echo "..\utils\sizeof.exe ..\bin\texts.bin 49152 "#define SCRIPT_INIT" >> msc-config.h"
echo #define SCRIPT_INIT 0xC000 >> msc-config.h

echo copy msc.h ..\dev
move msc.h ..\dev
rem copy msc.h ..\dev

REM pause

echo copy msc-config.h ..\dev
move msc-config.h ..\dev
REM copy msc-config.h ..\dev
rem copy scripts.bin ..\dev
cd ..\dev
REM pause

rem For 128K games with text + script sharing the same page, use this to
rem bundle both binaries...
rem echo ### BUNDLING TEXT + SCRIPT ###
rem echo copy /b ..\texts\texts.bin + ..\script\scripts.bin ..\bin\preload3.bin
rem echo which preload takes the text
rem copy ..\texts\texts.bin ..\bin\preload7.bin


REM copy ..\texts\texts.bin ..\bin\preload1.bin


copy ..\script\scripts.bin ..\bin\preload3.bin
rem PAUSE

rem PROBLEM with above as we are still combining the script and texts into the same ram page.



rem If you use arrows and/or drops this will make the sprites binary:
..\utils\spg2bin.exe ..\gfx\drop.png spdrop.bin
..\utils\spg2bin.exe ..\gfx\arrow.png sparrow.bin




echo ###########################
echo ## FIXED SCREENS
echo ###########################

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

echo ### MAKING LOADING SCREEN###
echo ..\utils\png2scr.exe ..\gfx\loading.png loading.bin
..\utils\png2scr.exe ..\gfx\loading.png loading.bin



echo ###########################
echo ## LIBRARIAN
echo ###########################
echo ### BUILDING RAMS       ###
echo ###########################
cd ..\bin
..\utils\librarian.exe
copy ram?.bin ..\dev
copy librarian.h ..\dev
cd ..\dev

rem pause

echo ###########################
echo ## MUSIC
echo ###########################


echo ############################
echo ### COMPILING WYZ PLAYER ###
echo ############################
cd ..\mus
echo ..\utils\pasmo WYZproPlay47aZXc.ASM ram1.bin

rem pause
rem ..\utils\pasmo WYZproPlay47aZX.ASM ram1.bin
rem pasmo WYZproPlay47aZX.ASM ram1.bin

rem try using WYZ version 0.5.0.7

echo ############################
echo ###  compressing songs   ###
echo ############################
..\utils\apack.exe empty.mus empty_c.mus
..\utils\apack.exe xeldatitle.mus xeldatitle_c.mus
..\utils\apack.exe dungeon.mus dungeon_c.mus
..\utils\apack.exe upperworld.mus upperworld_c.mus
..\utils\apack.exe level0.mus level0_c.mus
..\utils\apack.exe level1.mus level1_c.mus
..\utils\apack.exe level2.mus level2_c.mus
..\utils\apack.exe level3.mus level3_c.mus
..\utils\apack.exe level4.mus level4_c.mus
..\utils\apack.exe level5.mus level5_c.mus
..\utils\apack.exe level6.mus level6_c.mus
..\utils\apack.exe level9.mus level9_c.mus
..\utils\apack.exe fighting.mus fighting_c.mus
..\utils\apack.exe inside.mus inside_c.mus

echo ############################
echo ###   compiling songs    ###
echo ############################

..\utils\pasmo WYZproPlay47aZXc.ASM ram1.bin
rem pause

echo move ram1.bin ..\dev
copy ram1.bin ..\dev
cd ..\dev
echo echo -------------------------
rem pause


rem ##################################################
rem echo ### COMPILANDO WYZ PLAYER ###
rem cd ..\mus
rem ..\utils\apack.exe menu.mus menu-c.mus
rem ..\utils\apack.exe ending.mus ending-c.mus
rem ..\utils\pasmo WYZproPlay47aZXc.ASM ram1.bin
rem copy ram1.bin ..\dev
rem cd ..\dev
rem ##################################################












rem cls


echo ##################################
echo ## COMPILATION AND TAPE BUILDING #
echo ##################################
echo ###         COMPILING          ###
echo ##################################
rem zcc +zx -vn %game%.c -o %game%.bin -lsplib2 -zorg=24200
rem need to comment the draw_overlay sequence in mainloop.h

rem this is the one to use with overlay
zcc +zx -vn %game%.c -o %game%.bin -lsplib2t -zorg=24200

rem zcc +zx -vn %game%.c -o %game%.bin -compiler sdcc -lsplib2 -zorg=24200

rem for the lighting effects, the compiling sting
rem zcc +zx -vn %game%.c -o %game%.bin -compiler sdcc -lsplib2t -zorg=24200



rem zcc +zx -vn %game%.c -o %game%.bin -lsplib2t -zorg=24000 -O3

REM lsplib2t WILL NEED TO BE RECOMPILED FOR THIS TO WORK PERHAPS USING 23999 AS POKE FOR LIGHTING

REM testing compile string
REM zcc +zx -vn xelda.c -o xelda.bin -lsplib2 -zorg=24200 -no-cleanup



echo ##################################
echo ###       MAKING TAPS          ###
echo ##################################




..\utils\bas2tap -a10 -sFINAL loader\loader.bas loader.tap
..\utils\bin2tap -o loading.tap -a 16384 loading.bin
..\utils\bin2tap -o reubica.tap -a 25000 loader\reubica.bin

..\utils\bin2tap -o ram1.tap -a 32768 ram1.bin
..\utils\bin2tap -o ram3.tap -a 32768 ram3.bin
..\utils\bin2tap -o ram4.tap -a 32768 ram4.bin
..\utils\bin2tap -o ram6.tap -a 32768 ram6.bin
..\utils\bin2tap -o ram7.tap -a 32768 ram7.bin
..\utils\bin2tap -o main.tap -a 24200 %game%.bin

copy /b loader.tap + loading.tap + reubica.tap + ram1.tap + ram3.tap + ram4.tap + ram6.tap + ram7.tap + main.tap %game%.tap


REM copy /b loader.tap + loading.tap + reubica.tap + ram1.tap + ram3.tap + ram4.tap + main.tap %game%.tap

REM copy /b loader.tap + loading.tap + reubica.tap + ram1.tap + ram3.tap +  main.tap %game%.tap

rem PAUSE


echo ##################################
echo ## CLEANING
echo ##################################



del loader.tap
del main.tap
del loading.tap
del %game%.bin
del ram1.tap
del ram1.bin
del ram3.tap
del ram3.bin
del ram4.tap
del ram4.bin
del ram6.tap
del ram6.bin
del ram7.bin
del ram7.tap
del sparrow.bin
del spdrop.bin
del reubica.tap

del loading.bin
del texts.bin

rem you MUST remove your preloads otherwise, they will continue to add onto the stack and will not release.
del ..\bin\preload3.bin
REM del ..\bin\preload7.bin
REM del ..\bin\preload1.bin

rem you MUST remove your preloads otherwise, they will continue to add onto the stack and will not release.

echo ##################################
echo ## DONE
echo ##################################

rem what is basicts?
rem i believe its for FONTS
rem http://www.mojontwins.com/mojoniaplus/viewtopic.php?f=9&t=1540&p=56858&hilit=basicts&sid=cb1f918130bdb3c002aa3ca5aff64399#p56858



rem as for music and sounds
rem http://www.mojontwins.com/mojoniaplus/viewtopic.php?f=9&t=1339&p=43680&hilit=music+ram#p43680
rem seems to go into ram 1
