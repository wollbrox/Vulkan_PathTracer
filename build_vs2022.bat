@echo off

powershell -Command "Expand-Archive -Path .\deps\genie\genie.zip -DestinationPath .\deps\genie"

mkdir bin
mkdir build
call deps\genie\genie vs2022
del deps\genie\genie.exe

echo "build successfully"
pause
