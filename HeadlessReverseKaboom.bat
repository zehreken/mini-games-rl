@echo off
echo Starting MiniGamesRL in Headless Mode...
echo.

set BUILD_PATH=C:\Users\zehre\Documents\Dev\Unreal Projects\MiniGamesRL\Saved\StagedBuilds\Windows\MiniGamesRL\Binaries\Win64
set EXE=%BUILD_PATH%\MiniGamesRL.exe

if not exist "%EXE%" (
    echo ERROR: MiniGamesRL.exe not found at:
    echo %BUILD_PATH%
    echo.
    echo Please build/cook the project first before running headless mode.
    echo.
    pause
    exit /b 1
)

cd /d "%BUILD_PATH%"

.\MiniGamesRL.exe L_ReverseKaboom -nullrhi -nosound -log -log=kaboom.log

echo.
pause