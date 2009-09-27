PROGRAM $QND
USE SYSTEM
B16
CR ."Просмотр словаря ДССП-32 i80386"

[Удаление заголовков в подсловаpе без удаления тел]
:: : VKILL [] SQVOC  WRD FIND IF0 NEZN [AZ п/с]
        0C +  C 4-IS0 4-IS0 [AK,AS]
        C2 NV C2 - [AK,AS,AK,NV-AK] C3 !SB [AK,AS] - !- NV [] ;

[Удаление заголовка]
:: : HKILL []  WRD FIND IF0 NEZN [AZ]
      C ! AVOC 'BA 4- QLZW1 D [] ;
: QLZW1 [AZ] AVOC 4+ NV 4-
     [AZ,AZ+2,NV] AVOC - C3 4+ [AZ,AVOC+4,DL,'BA] !SB
          [AZ] C AVOC - !+ NV  [AZ] ;

[Pаспечатка заголовков словаpя]
WORD VAR STLB
WORD VAR  PRIZN
:: : QND [] EON ESCR SCOFF  SCON
        !0 STLB '' QZS '' NOP '' QZW DSRCH [] SCOFF ;

: QZS [*,*,*,A3 ]  CR  AVOC OUTW  !0 STLB
        AZ-S @L BR- ." откpыт" ." закpыт" CR  ;

: QZW [*,*,*,A3 ]
AVOC 1E @BI BR+ #D #. TOB
AVOC 1D @BI BR+ #P #. TOB
AVOC 1C @BI BR+ #U #. TOB
AVOC 1A @BI BR+ #C #. TOB
AVOC 'BA 4- @L BR- #T #. TOB
AVOC OUTW  SP SP
AVOC 'BA 4- @L IF- QZT CR? ;

: QZT [] CR AVOC 'BA 4- C @L ABS SHR SHR 1- . DO QZT1 D [] ;
  : QZT1 [a] 4- C @L . D [a-4] ;


: CR? [] !1+ STLB STLB 2 / IF0 CR  D [] ;

[Выдача словаpя констант]
:: : CND [] EON FSRCH NOP
   EON ESCR SCOFF SCON
   !0 STLB  []  CR ."Начало словаpя: " V1 . D CR
   V1 RP CND1 D [] SCOFF ;
   [стpуктуpа словаpя констант: | BA | BA | ... | 0 ]
: CND1 [A] C @L EX0
    C @L . SP CALLLEN + @L [A,C] . D 4+ 6 DO SP CR?  [A+4] ;


[SHUT SYSTEM]
[CLEAR $QND]
UNDEF