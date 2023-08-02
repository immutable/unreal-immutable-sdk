
@echo off

call env.bat

echo Building...
echo.

cd ..\..\..

%RunUAT% BuildCookRun -project=%ProjectPath% -map=%UnitTestMap% -build -unattended -nullrhi -ExecCmds="Automation Quit"

