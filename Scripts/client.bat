
@echo off

call env.bat

echo Running client and connecting to %LocalIP% ...
echo.

cd ..\..\..

%UEEditor% %ProjectPath% %LocalIP% -game -log

