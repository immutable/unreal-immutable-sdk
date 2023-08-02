
@echo off

if "%UEBatchScriptEnvLoaded%"=="true" (
    exit /B 0
)
set UEBatchScriptEnvLoaded=true

set PluginScriptDir=%~dp0
cd ..\..\..

for /F "tokens=4 delims= " %%i in ('route print ^| find " 0.0.0.0"') do set LocalIP=%%i

for %%f in (*.uproject) do (
    set ProjectPath="%cd%\%%f"
    for /F "eol=, tokens=1,2*" %%i in (%%f) do (
        if %%i==^"EngineAssociation^"^: (
            if %%j==^"4.27^"^, (
                set UEVersion=4.27
                set UEEnginePath=%UE427EnginePath%
            )
            if %%j==^"5.0^"^, (
                set UEVersion=5.0
                set UEEnginePath=%UE50EnginePath%
            )
            if %%j==^"5.1^"^, (
                set UEVersion=5.1
                set UEEnginePath=%UE51EnginePath%
            )
            if %%j==^"5.2^"^, (
                set UEVersion=5.2
                set UEEnginePath=%UE52EnginePath%
            )
            if %%j==^"5.3^"^, (
                set UEVersion=5.3
                set UEEnginePath=%UE53EnginePath%
            )
            if %%j==^"5.4^"^, (
                set UEVersion=5.4
                set UEEnginePath=%UE54EnginePath%
            )
            if %%j==^"5.5^"^, (
                set UEVersion=5.5
                set UEEnginePath=%UE55EnginePath%
            )
        )
    )
)

if "%UEEnginePath%"=="" (
    echo ERROR: Could not find Unreal Engine v%UEVersion% for project %ProjectPath%, please add your Unreal engine paths to your system environment variables in the format UE52EnginePath=C:\path\to\Unreal_5.2\Engine .
    exit 0
)

set RunUAT="%UEEnginePath%\Build\BatchFiles\RunUAT.bat"

if %UEVersion%==4.27 (
    set UEEditor="%UEEnginePath%\Binaries\Win64\UE4Editor.exe"
    set UEEditorCmd="%UEEnginePath%\Binaries\Win64\UE4Editor-Cmd.exe"
) else (
    set UEEditor="%UEEnginePath%\Binaries\Win64\UnrealEditor.exe"
    set UEEditorCmd="%UEEnginePath%\Binaries\Win64\UnrealEditor-Cmd.exe"
)

:: TODO: set these up for the plugin
set UnitTestMap=/Game/Maps/AutomatedTestMap
set MainMenuMap=/Game/Maps/MainMenuMap
set MainGameMap=/Game/Maps/PlayMap
set AllMaps=%UnitTestMap%+%MainMenuMap%+%MainGameMap%

echo Running on %COMPUTERNAME%
echo UEVersion: %UEVersion%
echo UEEnginePath: %UEEnginePath%
echo UEEditor: %UEEditor%
echo RunUAT: %RunUAT%
echo ProjectPath: %ProjectPath%
echo.

cd %PluginScriptDir%
