
@echo off

call env.bat

echo Running dedicated server...
echo.

cd ..\..\..

%RunUAT% BuildCookRun -project=%ProjectPath% -map=%UnitTestMap% -nop4 -build -cook -cookall -unattended
