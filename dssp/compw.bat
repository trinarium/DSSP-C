set home=c:\watcom
set watcom=%home%
set path=%path%;%home%\bin;%home%\binb
wcl386 %1 -i=%home%\h /cc++ /bt=dos /j %2 %3 %4 %5 %6