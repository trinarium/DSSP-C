@echo off
if "%1"== ""      goto noparam
if "%2"== "Sun"   goto Sun
if "%2"== "W32"   goto W32
if "%2"== "386"   goto D386
if "%2"== "Lite"  goto Lite
goto notype

:Sun
md %1
md %1\dssp
copy dssp\*.c %1\dssp
copy dssp\sun\*.cfg %1\dssp
copy dssp\*.cmd %1\dssp
copy dssp\*.h?? %1\dssp
copy dssp\*.h %1\dssp
copy dssp\*.slv %1\dssp
copy dssp\*.mmp %1\dssp
md %1\gen
copy gen\ng.c %1\gen
copy gen\*.bat %1\gen
copy gen\*.dsp %1\gen
copy gen\lite\*.dsp %1\gen
goto sux

:W32
md %1
md %1\dssp
copy dssp\W32\*.bat %1
copy dssp\*.c %1\dssp
copy dssp\w32\*.cfg %1\dssp
copy dssp\*.cmd %1\dssp
copy dssp\*.h?? %1\dssp
copy dssp\*.h %1\dssp
copy dssp\*.slv %1\dssp
copy dssp\*.mmp %1\dssp
copy dssp\comp32.bat %1\dssp
md %1\gen
copy gen\ng.c %1\gen
copy gen\*.dsp %1\gen
copy gen\ng32.exe %1\gen
copy gen\W32\*.dsp %1\gen
copy gen\*.bat %1\gen
md %1\gen\load
copy gen\W32\Load\*.* %1\gen\load
goto sux

:Lite
md %1
md %1\dssp
copy dssp\D386\comp.bat %1\comp386.bat
copy dssp\W32\comp.bat %1\comp32.bat
copy dssp\*.c %1\dssp
copy dssp\lite\*.cfg %1\dssp
copy dssp\*.cmd %1\dssp
copy dssp\*.h?? %1\dssp
copy dssp\*.h %1\dssp
copy dssp\*.slv %1\dssp
copy dssp\*.mmp %1\dssp
copy dssp\*.bat %1\dssp
md %1\gen
copy gen\ng.c %1\gen
copy gen\*.dsp %1\gen
copy gen\*.bat %1\gen
copy gen\lite\*.* %1\gen
copy gen\ng*.exe %1\gen
goto sux

:D386
md %1
md %1\DSSP
copy dssp\D386\COMP.bat %1
copy dssp\GEN386.bat %1
copy dssp\*.c %1\dssp
copy dssp\d386\*.cfg %1\dssp
copy dssp\*.cmd %1\dssp
copy dssp\*.h?? %1\dssp
copy dssp\*.h %1\dssp
copy dssp\*.slv %1\dssp
copy dssp\*.mmp %1\dssp
copy dssp\compw.bat %1\dssp
md %1\GEN
copy gen\ng.c %1\gen
copy gen\*.dsp %1\gen
copy gen\compw.bat %1\gen
rem copy gen\ng386.exe %1\gen
copy gen\*.dsp %1\gen
MD %1\EXAMPLE
copy example\*.* %1\example
MD %1\EXAMPLE\NEWSCAN
copy example\NEWSCAN\*.* %1\example\NEWSCAN
copy example\NEWSCAN\*.txt %1\example\NEWSCAN

MD %1\DOC
copy doc\*.doc %1\doc
copy doc\*.txt %1\doc
copy doc\*.bbs %1\doc
MD %1\LIB
copy lib\*.dsp %1\lib
copy lib\*.txt %1\lib
copy lib\*.set %1\lib
copy lib\*.bbs %1\lib
md %1\lib\PS
copy lib\ps\*.dsp %1\lib\ps
copy lib\ps\*.bbs %1\lib\ps
CD %1\dssp
REM CALL GEN386.BAT
CALL %1\COMP.BAT
goto sux

:notype
echo Invalid install type.
goto noparam

:nodir
echo Target directory not found.

:noparam
echo Use command   "inst  [dir] [type]".
echo [dir]  - target directory
echo [type] = Sun,W32,386,Lite
goto end

:sux
echo Install complete

:end