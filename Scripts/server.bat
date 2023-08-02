
@echo off

call env.bat

echo Running dedicated server...
echo.

cd ..\..\..

%UEEditor% %ProjectPath% %MainGameMap% -server -log=DedicatedServer.log

