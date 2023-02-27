@echo off

powershell -Command "Expand-Archive -Path ..\deps\premake\premake5.zip -DestinationPath ..\deps\premake"

mkdir ..\bin
mkdir ..\build
call ..\deps\premake\premake5 vs2022
del ..\deps\premake\premake5.exe

pause
