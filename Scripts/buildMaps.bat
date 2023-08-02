
@echo off

call env.bat

echo Building maps...
echo.

cd ..\..\..

%UEEditorCmd% %ProjectPath% %UnitTestMap% -run=resavepackages -buildhlod -buildlighting -quality=production -allowcommandletrendering -unattended -map=%AllMaps%
