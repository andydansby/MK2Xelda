@echo off

rem set here the game name (used in filenames @ %game%)
SET game=xelda

echo ----------------------------
echo    TESTING SCRIPT for %game%
echo ----------------------------

echo ### MAKING SCRIPT ###
cd ..\script
echo ..\utils\msc3.exe %game%.spt 20 rampage
rem ..\utils\msc3.exe %game%.spt 20 rampage
..\utils\msc3.exe %game%.spt 20 rampage

cd ..\dev