@echo off
setlocal

REM --- Configuration ---
set EXE_PATH=C:\Users\sd43247\CLionProjects\Mygale\cmake-build-release
set EXE_NAME=Mygale.exe
set MINGW_BIN=C:\Qt\6.7.0\mingw_64\bin
set DEPLOY_PATH=C:\Users\sd43247\CLionProjects\Mygale\release

REM --- Prompt user to confirm ---
echo This script will deploy your application to the "%DEPLOY_PATH%" directory. This will overwrite any existing files in that directory.
set /p confirm="Do you want to proceed? (Y/N): "
if /i "%confirm%" neq "Y" (
    echo Aborted.
    exit /b
)

REM --- Create deployment folder ---
if exist "%DEPLOY_PATH%" (
    echo "The folder '%DEPLOY_PATH%' already exists."
    echo Removing old deployment folder...
    rd /s /q "%DEPLOY_PATH%"
)

echo Creating new deployment folder...
mkdir "%DEPLOY_PATH%"

REM --- Deploy executable ---
echo Copying executable to deployment folder...
copy "%EXE_PATH%\%EXE_NAME%" "%DEPLOY_PATH%\"

REM --- Deploy Qt libraries ---
echo Copying Qt libraries...
"%MINGW_BIN%\windeployqt.exe" --dir "%DEPLOY_PATH%" --no-translations "%EXE_PATH%\%EXE_NAME%"

REM --- Deploy MinGW libraries ---
echo Copying MinGW libraries...
copy "%MINGW_BIN%\libstdc++-6.dll" "%DEPLOY_PATH%"
copy "%MINGW_BIN%\libgcc_s_seh-1.dll" "%DEPLOY_PATH%"
copy "%MINGW_BIN%\libwinpthread-1.dll" "%DEPLOY_PATH%"

REM --- Final message ---
echo Deployment complete. All files have been copied to "%DEPLOY_PATH%".
exit /b
