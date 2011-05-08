@echo off
echo Make sure GCC is in your PATH!

gcc -o kdgsubcalc ..\src\kdgsubcalc.c
cls
if ERRORLEVEL 9009 GOTO nogcc
if ERRORLEVEL 0 GOTO success
echo Problem while compiling kdgsubcalc.
GOTO end
:nogcc
echo No GCC has been found in path.
GOTO end
:success
echo kdgsubcalc has been succesfuly compiled.
GOTO end
:end
pause
