@echo off
cd   dssp
call compw dssp
erase dssp.obj
dssp -v
copy *.voc ..\gen
cd   ..\gen
REM call compw ng
REM erase ng.obj
..\DSSP\DSSP.EXE -C tg.dsp
move dssp.dpf ..\dssp
cd   ..\dssp