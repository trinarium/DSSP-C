CR ."Start kernel DSSP/c" CR 
[              ╔══════════════════════════════════════╗ 
               ║ Исходные тексты для компоновки DSSP  ║ 
               ╠══════════════════════════════════════╣ 
               ║   DSSP - 32 версия 4.3  i80386       ║ 
               ╠══════════════════════════════════════╣ 
               ║           декабрь 1995               ║ 
               ╚══════════════════════════════════════╝ 
] 
PBIT ''     PBIT DO     PBIT RP     PBIT IF0    PBIT IF+    PBIT IF- 
PBIT !       PBIT '      PBIT !!!    PBIT !+     PBIT !-     PBIT !1+ 
PBIT !1-    PBIT !0     PBIT !1     PBIT DIM?   PBIT SIZE?  PBIT CAP? 
1 %IF  
PBIT SOVR   PBIT SCLR   PBIT SADD   PBIT SDSB   PBIT SINS   PBIT SDEL 
PBIT SCUT   PBIT SISB   PBIT SSSB   PBIT SSSBI  PBIT SLEN   PBIT SADDB 
PBIT SCMP   PBIT SRPL   PBIT ST@B   PBIT ARG2STR   [PBIT DPFNAME] 
%FI 
 
[Установка значений некоторых переменных компоновщика. 
 Операция  __  в служит для присваивания значений переменным 
 компоновщика, тогда как ! - для присваивания значений 
 переменным формируемой системы] 
B10 
'' TOSOUT    __ ''TOSOUT 
[ ''  OUT    '' NOP __ ''OUT  ] 
[ '' .OUT    '' NOP __ ''.OUT ] 
   7        __   PRTLEN   [***BUR 15.01.1998 ] 
[ '' INTRPROC ] '' NOP __ ''INTRPROC [***BUR 15.01.1998 ] 
 
'' ;        __ ''; 
'' RAISE_   __ ''RAISE 
 
[ '' CnstBody  '' Cnst __ ''CnstBody ] 
'' codeUNDEF __ UNDEFCODE 
'' codeINT   __ INTCODE 
[ '' codeCALL  __ CALLCODE ] 
'' codeVAR   __ VARCODE 
'' codeCNST  __ CNSTCODE 
'' codeTXT   __ TXTCODE 
'' codePTXT  __ PTXTCODE 
   4         __ INTLEN 
[   8         __ CALLLEN [ длина тела вызова вместе с адресом ] 
   4         __ VARLEN 
   4         __ CNSTLEN 
 
