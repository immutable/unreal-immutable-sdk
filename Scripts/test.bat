
@echo off

call env.bat

echo Running plugin tests...
echo.

set TestScope=Immutable

cd ..\..\..

%UEEditor% ^
  %ProjectPath% %UnitTestMap% ^
  -AutomatedMapBuild=%UnitTestMap% ^
  -buildmachine -NullRHI -unattended -NoSound -NoP4 -nosplash -nopause ^
  -ExecCmds="Automation RunTests %TestScope%; Quit" ^
  -TestExit="Automation Test Queue Empty" ^
  -log=Test.log

