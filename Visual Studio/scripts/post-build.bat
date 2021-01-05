:: x86 / x64
set target=%1

:: Debug / Release
set configuration=%2

set fileDirectory=%~dp0
set libs=%fileDirectory%..\..\libs

set dest=%fileDirectory%..\..\bin\%target%\%configuration%

mkdir "%dest%"

:: Copy Engine dependencies
xcopy /Y /D /S "%libs%\GL\%target%\Release\*.dll" "%dest%"
xcopy /Y /D /S "%libs%\GLFW\%target%\%configuration%\*.dll" "%dest%"
xcopy /Y /D /S "%libs%\assimp\%target%\%configuration%\*.dll" "%dest%"
xcopy /Y /D /S "%libs%\Engine\%target%\%configuration%\*.dll" "%dest%"

rmdir /S /Q "%dest%\Resources"
mklink /J "%dest%\Resources" "%fileDirectory%..\..\Resources"
rmdir /S /Q "%dest%\Source"
mklink /J "%dest%\Source" "%fileDirectory%..\..\Source"
