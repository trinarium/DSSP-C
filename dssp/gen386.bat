echo on
call compile dssp
dssp -v
cd ..\gen
md temp
copy *.* temp
copy d386\*.* temp
copy ..\dssp\dssp.voc .\temp
cd temp
ng tg.dsp
copy dssp.dpf ..\..\dssp
echo Y|del *.*
cd ..
rd temp
cd ..\dssp
move dssp.exe dssp386.exe
move dssp.dpf dssp386.dpf
