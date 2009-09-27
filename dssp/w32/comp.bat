cd   dssp
call comp32 dssp.c
dssp -v
copy *.voc ..\gen
REM call compw ng
REM erase ng.obj
..\DSSP\DSSP.EXE -C tg.dsp
move dssp.dpf ..\dssp
cd   ..\dssp
