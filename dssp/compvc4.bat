@echo oN
if not %exist c:\msdev goto bad
REM For Visial C v 4
set home=c:\msdev
set lib=%home%\lib
set include=%home%\include
set path=%path%;%home%\bin
cl %1 /W3 /GX /D "WIN32" /D "_CONSOLE" /YX  /link kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib /subsystem:console /machine:I386
erase dssp.obj
erase vc40.pch
goto end

:bad
@echo "Error! Can't find Microsoft Visual C directory !!!"

:end