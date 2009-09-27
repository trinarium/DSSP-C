B16 CR ."System interruptions" 
 
[?CF - взять флаг Carry] 
:: : ?CF [] REFL 1 & [CF] ; 
 
1 %IF 
[===================== Системные прерывания ========================] 
[||                                                               ||] 
[||                                                               ||] 
[||                                                               ||] 
 
[INTERR - вызов системного прерывания. 
          вх.пар.:  (N) - номер прерывания; 
          вых.пар.: (). 
 Переменные REAX,REBX,RECX,REDX,REBP,RESI,REDI,RESP,RDS,RES,RGS,REFL,RFS 
                   соответствуют регистрам процессора] 
[ '' REAXA  ABSOLUT ] :: LONG    VAR REAX 
[ '' REBXA  ABSOLUT ] :: LONG    VAR REBX 
[ '' RECXA  ABSOLUT ] :: LONG    VAR RECX 
[ '' REDXA  ABSOLUT ] :: LONG    VAR REDX 
[ '' REBPA  ABSOLUT ] :: LONG    VAR REBP 
[ '' RESIA  ABSOLUT ] :: LONG    VAR RESI 
[ '' REDIA  ABSOLUT ] :: LONG    VAR REDI 
[ '' RESPA  ABSOLUT ] :: LONG    VAR RESP 
[ '' RDSA   ABSOLUT ] :: WORD    VAR RDS 
[ '' RESA   ABSOLUT ] :: WORD    VAR RES 
[ '' RSSA   ABSOLUT ] :: WORD    VAR RSS 
[ '' RCSA   ABSOLUT ] :: WORD    VAR RCS 
[ '' RFSA   ABSOLUT ] :: WORD    VAR RFS 
[ '' RGSA   ABSOLUT ] :: WORD    VAR RGS 
[ '' REFLA  ABSOLUT ] :: LONG    VAR REFL 
 
[--- Временная заглушка для вызова системного прерывания ---] 
:: : INTERR [NInt] D [] ; 
 
[||                                                               ||] 
[||                                                               ||] 
[||                                                               ||] 
[===================== Системные прерывания ========================] 
 
%FI 
 
 
0 %IF 
[############### УСТАНОВКА НОВЫХ ВЕКТОРОВ ПРЕРЫВАНИЙ ##################] 
[##                                                                  ##] 
[##                                                                  ##] 
 
[***BUR --- 14.01.1998 ------------------------------------------------] 
 
[--- Ситуация, возбуждаемая в качестве реакции на прерывание по CTRL_C ] 
:: FIX TRAP CCSIT! CCSIT!_FIN 
   [ и стандартная конечная реакция на эту ситуацию ] 
   : CCSIT!_FIN CR ."Завершение по CTRL-C !" RESTART ; 
 [ Переменная, содержащая процедуру реакции на прерывание по CTRL_C ] 
:: FIX ACT VAR CCACT  '' CCSIT! ! CCACT [ вызов ситуации CCSIT! ] 
 
[***BUR Надо ли тут INT ? и Можно! Так как компоновщик уже исправлен ] 
   INT : CTRL_C [."^C!"] CCACT ; 
   INT : CTRL_BREAK [."^BREAK!"] CCACT ; 
   INT : NOPINTHNDLR  ; 
 
[*** endBUR -----------------------------------------------------------] 
 
B16 
       [ для запоминания векторов прерываний 23h ] 
FIX LONG VAR RM_CC_VEC 
FIX WORD VAR PM_CC_SEL 
FIX LONG VAR PM_CC_OFF 
       [ для запоминания векторов прерываний 1Bh ] 
FIX LONG VAR RM_1B_VEC 
FIX WORD VAR PM_1B_SEL 
FIX LONG VAR PM_1B_OFF 
 
FIX LONG VAR ETXFLAG  [флаг выключения прерывания по ^C] 
                      [0 - включено, >0 - выключено] 
  1 ! ETXFLAG 
 
[Включение и выключение прерывания по ^C] 
 
  : ETXON  []  ETXFLAG BR0 NOP ETXON1 [] ; 
    : ETXON1  []  !1- ETXFLAG ETXFLAG IF0 ETXON2  [] ; 
    : ETXON2  []  [ 23 '' CTRL_C SET_LINK ]  [Seg:Off, Off, Sel] 
                  [  23 LINK INTRPROC ]      [Seg:Off, Off, Sel] 
                     23 LINK CTRL_C          [Seg:Off, Off, Sel] 
                  ! PM_CC_SEL  ! PM_CC_OFF  ! RM_CC_VEC  [] 
                     1B LINK CTRL_BREAK      [Seg:Off, Off, Sel] 
                  ! PM_1B_SEL  ! PM_1B_OFF  ! RM_1B_VEC  [] ; 
 
  : ETXOFF []  ETXFLAG BR0 ETXOFF1 ETXOFF2  [] ; 
    : ETXOFF1 []  RM_CC_VEC PM_CC_OFF PM_CC_SEL 23 RESTORE_LINK 
                  RM_1B_VEC PM_1B_OFF PM_1B_SEL 1B RESTORE_LINK 
                  !1 ETXFLAG   [] ; 
    : ETXOFF2 []  !1+ ETXFLAG  [] ; 
 
     [ VARIABLES for saving interrupt vectors of int 23h ] 
FIX WORD VAR OLD23Sel 
FIX LONG VAR OLD23Off 
FIX LONG VAR OLD23SegOff 
     [ VARIABLES for saving interrupt vectors of int 1Bh ] 
FIX WORD VAR OLD1BSel 
FIX LONG VAR OLD1BOff 
FIX LONG VAR OLD1BSegOff 
 
: LDINTVS []  LD23 LD1B ETXON INTRON ION ; 
  : LD23 [] [ '' NOPINTHNDLR 000F 23 SetAPMIntVect ] 
            [ '' NOPINTHNDLR 7 - 000F 23 SetAPMIntVect ] 
      [***BUR?  А почему бы не так ? ]  23 LINK NOPINTHNDLR DD D ; 
  : LD1B [] 1B LINK NOPINTHNDLR DD D ; 
 
  '' LDINTVS ! LDINTS 
 
: SVINTVS [] SV23 SV1B ; 
  : SV23 []  23 GetRMIntVect ! OLD23SegOff 
             23 GetPMIntVect ! OLD23Sel ! OLD23Off  [] ; 
  : SV1B []  1B GetRMIntVect ! OLD1BSegOff 
             1B GetPMIntVect ! OLD1BSel ! OLD1BOff  [] ; 
 
  '' SVINTVS ! SVINTS 
 
: RSINTVS [] !0 ETXFLAG ETXOFF RS23 RS1B ; 
  : RS23 []  OLD23SegOff OLD23Off OLD23Sel 23 RESTORE_LINK  [] ; 
  : RS1B []  OLD1BSegOff OLD1BOff OLD1BSel 1B RESTORE_LINK  [] ; 
 
  '' RSINTVS ! RSINTS 
 
[##                                                                  ##] 
[##                                                                  ##] 
[######################################################################] 
%FI 
 
: ETXON ; 
: ETXOFF ; 
