B8 CR ."Min. debug tools DSSP-32" CR 
 
[::] : CON '' VSTR ! VVS VSTR ; 
   : VSTR [A,DL] CR ATEK UESTR TOS [A,DL] ; 
 
:: : PBIT WRD FIND  IF0 NEZN [AZ] 35 [PBIT] !BI1 [] ; 
 
LONG VAR OTL7 [УПР.СТЕК ПРИ ОСТАНОВЕ] 
LONG VAR OTL8 [СЛЕД.КОМ.] 
:: FIX ACT VAR APO '' NOP ! APO [ДОП. ДЕЙСТВИЯ ПРИ ОСТАНОВЕ] 
 
[::] : \G [] PRIGL #. = IF+ \G1 #? TOB [] ; 
     : \G1 [] #* ! PRIGL OTL8 OTL7 \G11 ; 
 
:: TRAP FSRCH DDDD 
:: : DSRCH [*,''ZS,''TAB,''ZW] NV 4- DSRCA [*] ; 
:: : DSRCA [.................,V2] EON FSRCH DDDD    RP DSRCH0   DDDD [*] ; 
: DSRCH0 [*,''ZS,''TAB,''ZW,НА яч. перед ZS] 
         RP DSRCH1 4- C @L EX0 @L 
         [*,''ZS,''TAB,''ZW,НА яч. перед след. ZS] ; 
: DSRCH1 [*,''ZS,''TAB,''ZW,НА яч. перед ZS] 
       4- C!AVOC'BA 10 - 
       [*,''ZS,''TAB,''ZW,a0/azw/atbl] 
       C4 EXEC 
       RP DSRH2 [*,''ZS,''TAB,''ZW,A0] 
       4- C @L EX0 [*,''ZS,''TAB,''ZW,A0] ; 
: DSRH2 [*,''ZS,''TAB,''ZW,a0/azw/atbl] 
       C @L BRS DSRCHT DSRH3 DSRCHZ [*,''ZS,''TAB,''ZW,AZ] ; 
: DSRH3 [a0] C!AVOC'BA  EX [0] ; 
: DSRCHT [*,''ZS,''TAB,''ZW,atbl] C3 EXEC  C @L + 
       [*,''ZS,''TAB,''ZW,az/a0] ; 
: DSRCHZ [*,''ZS,''TAB,''ZW,AZ]  C!AVOC'BA 4- 
       C2 EXEC   [*,''ZS,''TAB,''ZW,AZ next] ; 
 
:: FIX ACT VAR V_PROC '' D ! V_PROC [истинное значение 
                                     подставляется в отладчике!] 
 : ZAGL1 ." 
Halt before "  NEZNAYU SP CPC [ук-ль стека возвр.=sp] 
       C 4+ @L [Цикл]  BR0 14 0  + @L RP ZAG1 [AU] 
       [AU] C SPOISK [AZ] OUTW [AU] V_PROC [] 
       ''NOP CPC  ZMOTL [] ; 
 
'' ZAGL1 ! ZAGL 
: ZAG1 [AT] 4-  C@L '' ZAGL = EX+ ; 
:: : ZMOTL [''P,CPC] ! OTL7 ! OTL8 
           APO [ВЫПОЛНЕНИЕ ДОП. ДЕЙСТВИЙ, СВЯЗАННЫХ С ОСТАНОВОМ] 
           #. ! PRIGL  ENDL  ON EXERR NOP  BELL ZMON ; 
 
:: : SPOISK [AU] '' NOP '' SPT '' NOP DSRCH D AVOC [AZ] ; 
: SPT [AU,*,*,*,ATAB] C DLTAB 
       [AU,*,*,*,ATAB,ATAB,N]  DO SPT1 D [AU,*,*,*,ATAB] ; 
: SPT1 [AU,*,*,*,ATAB,ADR] 4- C @L 7 CT = IF+ FSRCH [AU,...,ATAB,ADR-4] ; 
 
 
 
CR .".. ?I'" 
B10 
: .. [] DEEP [d] .B10 ." pos." CR #[ TOB [d] C DO .S D  SP #] TOB [] ; 
 : .S [d] .S1  C 1+ CT  . D 1- [d-1] ; 
 : .S1 [d] DEEP C2 - 1-  BASE@ BR 2 2  16 8 ELSE 6 / E2D IF0 CR [] ; 
 
:: FIX WORD VAR NOMVERS  [номер версии DSSP]     5 ! NOMVERS 
:: FIX WORD VAR NOMRED   [номер редакции DSSP]   0 ! NOMRED 
:: FIX WORD VAR PLATFORM [Hомеp платфоpмы генеpации:
                          1-IBM PC, 2-SUN, 3-LINUX, 4-R3000] 1 ! PLATFORM
 
: ?I' [] SQVOC S( BASE@ ) B10  ." 
DSSP-32/C VERSION " NOMVERS 2 TON #. TOB NOMRED C 9 > BR+ 2 1 TON 
   .".   Free"  BEG_YD1 NV - 30 + . D #B TOB  [] ; 
    
'' ?I' ! ?I 
 
B16 
[ Распределение памяти DSSP-386 
                                        YD1              WD1 
|------>-----<------->- - - - - |------->-------|------->-------| 
0       NB   V1       NV        BEG_YD1  END_YD1 BEG_WD1  END_WD1 
 
     rEBP rESP            PTM          D1 
|---->----<----|-|--------->--|--------> - - - | 
OPSB         CSB BEG_PTM      BEG_D1     END_MEM   ] 
 
[::] : @@ [] S( BASE@ ) B16 
 CR ." 
                                             YD1=" YD1 8 TON 
 ."   WD1=" WD1 8 TON ." 
 |------>--------<-------->- - - - - - |------->------|------->--------| 
 0      NB       V1       NV         BEG_YD1   END_YD1 BEG_WD1   END_WD1" 
  CR  NB 0D TON  V1 . D  NV . D BEG_YD1 0E TON END_YD1 09 TON 
      BEG_WD1 9 TON  END_WD1 0B TON 
  CR CR ." 
       rEBP rESP            PTM=" PTM 8 TON ."       D1=" D1 8 TON ."
  -|---->----<----|-|--------->------|-|--------> - - - |
  OPSB          CSB BEG_PTM    END_PTM BEG_D1         END_MEM"
  CR '' AOPSB @L . D  '' ARSB @L 0A TON BEG_PTM . D END_PTM 0A TON
     BEG_D1 . D END_MEM 0F TON [] ;