@echo oN
if %exist c:\msdev goto vc4
REM For Visial C v 5
if not %exist c:\PROGRA~1\DEVSTU~1
set home=c:\PROGRA~1\DEVSTU~1
set lib=%home%\vc\lib
set include=%home%\vc\include
set path=%path%;%home%\vc\bin;%home%\SHARED~1\BIN
cl %1 /W3 /GX /D "WIN32" /D "_CONSOLE" /YX  /link kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib /subsystem:console /machine:I386
erase dssp.obj
erase vc50.pch
goto end

:vc4
REM For Visial C v 4
set home=c:\msdev
set lib=%home%\lib
set include=%home%\include
set path=%path%;%home%\bin
cl %1 /W3 /GX /D "WIN32" /D "_CONSOLE" /YX  /link kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib /subsystem:console /machine:I386
erase dssp.obj
erase vc40.pch

:bad
@echo "Error! Can't find Microsoft Visual C directory !!!"

:end