@echo off

rem set here the game name (used in filenames @ %game%)
SET game=xelda

echo ----------------------------
echo    TESTING SCRIPT for %game%
echo ----------------------------

echo ### Building Levels ###

cd ..\levels
call buildlevels.bat
cd ..\dev
