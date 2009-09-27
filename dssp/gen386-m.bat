echo on
REM call compile dssp
CD DSSP
CALL COMPW.BAT DSSP.C
CD ..\GEN
CALL COMPW.BAT NG.C
dssp.EXE -v
cd ..\gen
md temp
copy *.* temp
copy d386\*.* temp
copy ..\dssp\dssp.voc .\temp
cd temp
ng tg.dsp
copy dssp.thr ..\..\dssp
echo Y|del *.*
cd ..
rd temp
cd ..\dssp
move dssp.exe dssp386.exe
move dssp.thr dssp386.thr
