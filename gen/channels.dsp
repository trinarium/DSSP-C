B16
CR ."Файловая система. Каналы. Драйвер принтера."

[***********************************************************]
[           ПРЕОБРАЗОВАНИЕ СТРОК ДЛЯ ИМЕН ФАЙЛОВ            ]


:: : ASCIIZ [A,DL] @R S( PTM ) !R
        C 1+ TUSE   C2 ENDZ [PTM] ;
  : ENDZ [A,DL,AS,DL0] C2 C PUSH + 0 [#$] E2  !TB  !SB POP [PTM] ;

:: 3 FIX BYTE VCTR .EXT
#. 0 ! .EXT
#D 1 ! .EXT
#S 2 ! .EXT
#P 3 ! .EXT
: GEXT 0 ' .EXT 4 ;
:: TRAP .RAS GEXT

:: : DSP [A,DL] @R S( PTM ) !R
        C 5 + TUSE [A,DL,AS]
        .RAS [A,DL,AS,AR,DLR] C PUSH C4 C4 +  !SB
        [A,DL,AS] C3 C3  SRCH. C3 =
        POP E2 IF0 T0 [A,DL,PTM,SM0]  C3 + ENDZ [PTM] ;
: SRCH. [поиск точки с конца до символа \]
  [A,L] E2 C2 + 1- [L,AEND] 0 C3 DO SRCH.1 [L,AEND,dL] E2D - [L'] ;
  : SRCH.1 [AEND,0] C2 @B BR #\ T0EX
                             #. T1EX
                             ELSE NOP E2 1- E2 [AEND,0] ;

            : T1EX T1 EX ;
            : T0EX T0 EX ;


[*******************************************************]
[          ОПРЕДЕЛЕНИЕ КАНАЛОВ И ИХ СВЯЗЫВАНИЕ          ]

[::] : CHANNEL [] CODE ,NOP [,NEXT] MLPATHNAME 2+ MUSE [A] ,L [] ;

[CONNECT - connect channel with file.]
[::] : CONNECT [A,L] GTP !AFM Sconnect [] ;
     PBIT CONNECT

[Sconnect: with filename transform]

     : Sconnect [A,L]
       TFN [] [приписали устройство и директорий]
       AFM 2+ 0 C2 MLPATHNAME SRCHB [A,L] 1+
       DEL\   [удалили вхождения типа \.\ и \###\..\]
       [A,L] AFM 2+ !SB [] ;

     : TFN [преобразование имени файла]
       [A,L] DSP [AS] AFM 2+ [AS,AD]
       C2 1+ @B #: =  BR0 TFN1 TFN2 [AS,AD] 
       CDRIVE #A 1- + C2 !TB 1+
       #: C2 !TB 1+ [AS,AD]
       C2 @B #\ = IF0 TFN3 [AS,AD]           
       MLPATHNAME C2 AFM 2+      1 NEG D   - -   
       [AS,AD,L] E2      
       !SB [] ;
    : TFN1 [AS,AD] [дописать текущее устройство]
       GCDRIVE ! CDRIVE [AS,AD,drv] ;
    : TFN2 [AS,AD]  C2 @B #A - 1+ ! CDRIVE
       E2 2+ E2 [AS,AD] ;
    : TFN3 [AS,AD] [приформировать директорий]
       CDRIVE GCDIRDRV  [AS,AD,A,L] 2- E2 2+ [AS,AD,L,A]
       C2 C4 [AS,AD,L,A,L,AD] !SB [AS,AD,L] + ;
    BYTE VAR CDRIVE

    : DEL\ [A,L] 0
      [Aначала, сколько осталось, сколько просмотрено]
      RP DEL\1
      [Aначала, сколько осталось, сколько просмотрено]
      + [A,L] ;
    : DEL\1
      [Aнач, ост, просм] C2 4 < EX+  [Aнач, ост, просм]
      C3 C2 +
            C @B #\ =
            C2 1+ @B #. = &
            E2 2+ @B #\ = &
      [Aнач, ост, просм,1/0] BR+ DEL\.\  DEL\2 [Aнач, ост, просм] ;
    : DEL\2  [Aнач, ост, просм] C2 5 < BR0 DEL\3 DEL\NXT [Aнач, ост, просм] ;
    : DEL\3  [Aнач, ост, просм]
      C3 C2 +
            C @B #\ =
            C2 1+ @B #. = &
            C2 2+ @B #. = &
            E2 3+ @B #\ = &
      [Aнач, ост, просм,1/0] BR+ DEL\..\ DEL\NXT  [Aнач, ост, просм] ;
    : DEL\NXT [Aнач, ост, просм]
      E2 1- E2 1+
      [Aначала, сколько осталось, сколько просмотрено] ;
    : DEL\..\
      [Aн, ост, просм]
      [нашли подстроку \..\]
      C3 C C3 + [Aн, ост, просм,A,A\..]
      RP DEL..3 [Aн, ост, просм,A,A\\..]
      C2 C2 = BR+ DD DEL..4  [Aн, ост, просм] ;
    : DEL..3 [A,A1] C2 C2 = EX+
      C 1- @B #\ = EX+
      1- [A,A1] ;
    : DEL..4 [Aн, ост, просм,A,A\\..]
      [нашли, надо удалять]
      E2 C3 +  4+ [Aн, ост, просм,A\\..=AD,AS]
      C C3 - PUSH C4 4- [Aн, ост, просм,A\\..=AD,AS,Ltail]
      E2 E3 !SB [Aн, ост, просм] POP -  [Aн, ост, просм]
      E2 3+ E2 3- ;
    : DEL\.\
      [Aн, ост, просм]
      [нашли подстроку \.\]
      C3 C2 + [Aн, ост, просм, AD]
      C 2+ [Aн, ост, просм, AD, AS] C4 2- E2 E3 !SB
      [Aн, ост, просм] E2 2- E2  [Aн, ост, просм] ;



:: LONG VAR AFM [адрес FCB текущего канала]
     : !AFM [A] [NEXTLEN] NOPLEN + @L ! AFM [] ;

[Структура FCB канала:
AFM ->    0  +--------+
          1  +--------+   handle
          2  +--------+
             I        I   полное имя файла (ASCIIZ) в форме
             I        I   d:filename или  d:\path\filename
             +--------+             ]


:: : SDSP GTP !AFM AFM MLPATHNAME 2+ !TM ; PBIT SDSP
:: : LDSP GTP !AFM AFM @TM ;               PBIT LDSP

: @handle [] AFM @W [handle] ;
: !handle [handle] AFM !TW [] ;

[*****************************************************]
[               ОШИБКИ И ИХ ОБРАБОТКА                 ]

[DERR - ситуация, возникающая при ошибках ВУ.
        Конечная реакция - выдача соответствующей информации
        (имени файла, расшифровки ошибки, номера ошибки) и
        возбуждение ситуации EXERR. Номер ошибки в интервале
        0 .. 0FFH соответствует номеру в MS-DOS, >100H - ошибки
        выявленные в ДССП (пока используется только
        101H - конец файла при чтении).
        Номер ошибки хранится в переменной K_FS_M]
   TRAP _DERR FS_M0 [промежуточная реакция для прекращения работы
                     процедур обмена]
:: TRAP DERR FS_M1
:: WORD VAR K_FS_M

: FS_MIST [error code] ! K_FS_M _DERR  [] ;
: FS_M0 [] K_FS_M [error code]
     BR
       1 NOF
      50 ISF
      15 DNR
     101 EOF
         ELSE DERR [] ;
: FS_M1 []
     CR SFNAME
     CR
     K_FS_M BR
           2 ."Файл не найден"
           3 ALPNF [."Каталог не найден"]
           4 ."Слишком много откpытых каналов"
           6 ."Канал не открыт"
          11 ."Нет устpойства"
          13 ."Диск защищен по записи"
          14 ."Bad disk unit"
          15 ."Устройство не готово"
          50 ."Такой файл уже есть"
         101 ."Конец файла"
             ELSE FS_M2 EXERR [] ;
: FS_M2 [] ."Ошибка ФС N" K_FS_M .B10 D [] ;


[Вынесенные на пользователя особые ситуации:
  конечная реакция DERR, но можно их
  отлавливать самостоятельно]

[NOF - файл не найден]
:: TRAP NOF NOFR
   : NOFR 2 ! K_FS_M DERR ;

[ISF - такой файл уже есть]
:: TRAP ISF ISFR
   : ISFR 50 ! K_FS_M DERR ;

[DNR - устройство не готово]
:: TRAP DNR DNRR
   : DNRR 15 ! K_FS_M DERR ;

[EOF - достигнут конец файла]
:: TRAP EOF REOF
   : REOF []  101 ! K_FS_M  DERR [] ;



[ ****************************************************]
[ ПРОЦЕДУРЫ, ОБЕСПЕЧИВАЮЩИЕ НАРЯДУ С INTERR ИНТЕРФЕЙС С
           MS-DOS ДЛЯ ОБМЕНА С ДИСКОМ]

0 %IF [часть реализовано на СИ в виде ]
[FSFUN - вызов функции DOS и возбуждение DERR если CF=1]
: FSFUN [AX] ! REAX 21 INTERR REAX [LO] ?CF
        [AX,CF] IF+ FS_MIST [AX] ;
%FI


0 %IF 
[CLOSA1 - ЗАКРЫТИЕ КАНАЛА ПО handle]
: CLOSA1 [handle] ! REBX 3E00 FSFUN D [] ;

[OPENA1 - ОТКРЫТИЕ КАНАЛА И ПОЛУЧЕНИЕ handle]
: OPENA1 [AS,mode] E2 ! REDX
         [mode] 3D00 &0 FSFUN [handle] !handle [] ;

[SEEKA1 - ПОЗИЦИОНИРОВАНИЕ ПО ФАЙЛУ]
: SEEKA1 [pos,mode] @handle ! REBX E2 C LO ! REDX HI ! RECX
         [mode] 4200 &0 FSFUN [AX] REDX LO SETHI [new POS=DX:AX] ;

[DISA1 - ЧТЕНИЕ СТРОКИ ИЗ ФАЙЛА]
: DISA1 [A,L] 3F00 DIOSA1 [] ;

[DOSA1 - ВЫВОД СТРОКИ В ФАЙЛ]
: DOSA1 [A,L] 4000 DIOSA1 [] ;

[DIOSA1 - ОБМЕН С ФАЙЛОМ]
: DIOSA1 [A,L,Fn] C2 BR0 DDD DIOSA11 [] ;
   : oDIOSA11 [A,L,Fn] PUSH E2 [L,A] C2 ! RECX
         [L,dssp-A] ! REDX  [L]
         @handle ! REBX POP FSFUN [L,L'] = IF0 FS_MIST [] ;
   : DIOSA11 [A,L,Fn] E3 [Fn,L,A] C2 ! RECX
         [Fn,L,dssp-A] ! REDX  [Fn,L]
         @handle ! REBX E2 [L,Fn] FSFUN [L,L'] = IF0 FS_MIST [] ;
%FI

[новая реализация для Си]
: FSFUN [] FSFUN_C [Nerr,CF] IF+ FS_MIST [Nerr] D [] ;

[новая реализация для Си]
[CLOSA1 - ЗАКРЫТИЕ КАНАЛА ПО handle]
: CLOSA1 [handle] CLOS_C FSFUN [] ;

[OPENA1 - ОТКРЫТИЕ КАНАЛА И ПОЛУЧЕНИЕ handle]
: OPENA1 [AS,mode] OPEN_C FSFUN [handle] !handle [] ;

[SEEKA1 - ПОЗИЦИОНИРОВАНИЕ ПО ФАЙЛУ]
: SEEKA1 [pos,mode] @handle [pos,mode,handle] LSEEK_C FSFUN [new POS] ;

[DISA1 - ЧТЕНИЕ СТРОКИ ИЗ ФАЙЛА]
: DISA1 [A,L] 3F00 DIOSA1 [] ;

[DOSA1 - ВЫВОД СТРОКИ В ФАЙЛ]
: DOSA1 [A,L] 4000 DIOSA1 [] ;

[DIOSA1 - ОБМЕН С ФАЙЛОМ]
: DIOSA1 [A,L,Fn] C2 BR0 DDD DIOSA11 [] ;

   : oDIOSA11 [A,L,Fn] PUSH E2 [L,A] C2 ! RECX
         [L,dssp-A] ! REDX  [L]
         @handle ! REBX POP FSFUN [L,L'] = IF0 FS_MIST [] ;

   : DIOSA11 [A,L,Fn] @handle [A,L,Fn,handle] RDWR_C FSFUN [L,L'] = IF0 FS_MIST [] ;




[***********************************************]
[     БАЗОВЫЕ ОПЕРАЦИИ С КАНАЛАМИ               ]

[::] : OPEN []
       GTP !AFM Sopen [] ;
     PBIT OPEN
     : Sopen []
       EON _DERR _DERR
       AFM 2+ 2 [AS, 2=mode for write/read] OPENA1 [] ;


[FPPRS - получить строку, содержащую каталог данного файла]
[::] : FPPRS [] GTP !AFM Sfpprs [A,L] ;     PBIT FPPRS
     : Sfpprs [] Sffprs [A,L] RP FPP1 [A,L] ;
     : FPP1 [A,L] C 2 = EX+ C2 C2 + 1- @B #\ = EX+ 1- [A,L] ;

[FNPRS - получить строку, содержащую только имя данного файла]
[::] : FNPRS [] GTP !AFM Sfnprs [A,L] ;     PBIT FNPRS
     : Sfnprs [] Sfpprs E2 C2 + E2 [A,L]
                MLPATHNAME - NEG 0 E2 C3 E2 SRCHB [A,L] ;

[FFPRS - получить строку, содержащую полное имя данного файла]
[::] : FFPRS [] GTP !AFM Sffprs [A,L] ;     PBIT FFPRS
     : Sffprs [] AFM 2+ 0 C2 MLPATHNAME SRCHB [A,L] ;

[::] : FNAME [] GTP !AFM SFNAME [] ;
     : SFNAME [] Sffprs TOS [] ;    PBIT FNAME


[::] : CLOSE [] GTP !AFM [@handle 0 4 SEG IF0] Sclose [] ;
     : Sclose []
       EON _DERR _DERR
       @handle CLOSA1 [] ;
     PBIT CLOSE

0 %IF [теперь примитив]
[CLOSEALL - закрыть все каналы]
:: : CLOSEALL [] 5 100 DO CLAL1 D [] ;
   : CLAL1 [handle] 3E00 ! REAX C ! REBX 21 INTERR 1+ [next handle] ;
%FI

: CLOSEALL CLOSEALL_C ;

[POS - получить номер текущего байта]
[::] : POS [] GTP !AFM pos' [pos] ;
     : pos' []
       EON _DERR _DERR
       0 1 SEEKA1 [pos] ;
     PBIT POS

[LENB - получить длину файла в байтах]
[::] : LENB [] GTP !AFM  lenb' [lenb] ;
     PBIT LENB
     : lenb' []
       EON _DERR _DERR
       pos' [pos] 0 2 SEEKA1 [old pos, lenb] E2 Sspos [lenb] ;

[SLENB - установить длину файла равной номеру текущего байта.
         Все остальные байты до конца файла теряются.
      Эта операция очень ДОЛГАЯ! Она выполняется эффективно только
      в случае POS=0. Иногда в процессе выполнения этой операции
      могут создаваться временные файлы. ]
[::] : SLENB [] GTP !AFM pos' lenb' = IF0 SLENB' [] ;
     PBIT SLENB
     : SLENB' []
       EON _DERR _DERR
       pos' BR0 SLENB0 SLENB1 [] ;
     : SLENB0 [] Sclose  clopen [] ;

[WOPEN - открыть файл с lenb:=0]
[::] : WOPEN [] GTP !AFM clopen ; PBIT WOPEN

0 %IF
     : clopen [] AFM 2+ ! REDX  0 ! RECX
                 3C00 FSFUN [handle] !handle [] ;
%FI
     : clopen [] AFM 2+ [name] 0 [name,attr] CRT0_C FSFUN [handle] !handle [] ;


     : SLENB1 [] S( D1 ) empl [a,l]  C BR0 NOPL SLENB1' [] ;
     : NOPL DD CR ."Не хватает памяти!" 0 FS_MIST ;
     : SLENB1' [a,l] pos' C2 > BR+ SLENB2 SLENB3 [] ;
 [case: not enough empty place in memory for saving file]
     : SLENB2 [a,l]
        [a,l] pos' [a,l,btc] 0 Sspos
        AFM ! AFM0 [save current AFM]
        Sfpprs ASCIIZ [,,,AS] '' CH2 !AFM MLPATHNAME AFM 2+ !SB
        
        [AFM 2+ ! REDX  0 ! RECX 5A00 FSFUN] []
         
        AFM 2+  0 CRT_TMP_C FSFUN [ret_adr] AFM !TW []

        RP COPYPR1 DD D []
        CLOSE CH2
        AFM0 ! AFM Sclose Sdel Sfpprs Sfnprs E2D + REN CH2
        AFM0 ! AFM Sopen lenb' Sspos [] ;
     LONG VAR AFM0
     : COPYPR1 [a,l,btc] C EX0
        C2 C2 MIN C4 C2 [a,l,btc,dl,a,dl]
        AFM0 ! AFM C2 C2 Sis' OS CH2 - [a,l,btc] ;
 [case: enough empty place in memory for saving file]
     : SLENB3 [a,l]
        D pos' 0 Sspos C2 C2 Sis' [a,l] SLENB0 [a,l] DOSA1 [] ;


     [empl - взять адрес и длину макс. своб. места в обл. данных]
     : empl [] END_MEM D1 - 10 - [l] C MUSE E2 [a,l] ;
          : Byt->Par [B] 1- LO -4 SHT 1+ [P] ;
          : Par->Byt [P] 4 SHT [B] ;


[SPOS - установить номер текущего байта в файле равным
        значению из вершины стека]
[::] : SPOS [new pos] GTP !AFM Sspos [] ;
     : Sspos [new pos]
       EON _DERR _DERR
       0 SEEKA1 D [] ;
     PBIT SPOS

0 %IF

:: : CRT []
        GTP !AFM
        EON _DERR _DERR
        AFM 2+ [AS] ! REDX  0 ! RECX
        5B00 FSFUN [EAX] CLOSA1 [] ;
     PBIT CRT

:: : DEL []
        GTP !AFM Sdel [] ;
     : Sdel []
       EON _DERR _DERR
       AFM 2+ ! REDX 4100 FSFUN [eax] D [] ;
     PBIT DEL

:: : REN [A,L new]
        GTP !AFM  AFM ! AFM0
        ON DERR REN2
        CONNECT CH1
        AFM 2+ ! REDI
        AFM0 2+ ! REDX 5600 FSFUN [EAX]
        REN1 [] ;
     : REN1 [EAX] D AFM 2+ MLPATHNAME AFM0 2+ !SB [] ;
     PBIT REN
     : REN2 [] AFM0 ! AFM DERR [] ;
%FI

[новая реализация для Си]
:: : CRT []
        GTP !AFM
        EON _DERR _DERR
        AFM 2+ [AS]  0 [AS,attr]
        CRT_C FSFUN [ch] CLOSA1 [] ;
     PBIT CRT

:: : DEL []
        GTP !AFM Sdel [] ;
     : Sdel []
       EON _DERR _DERR
       AFM 2+ DEL_C FSFUN [retC] D [] ;
     PBIT DEL

:: : REN [A,L new]
        GTP !AFM  AFM ! AFM0
        ON DERR REN2
        CONNECT CH1
        AFM 2+ [new name]
        AFM0 2+ [new name,old name] REN_C FSFUN [retC]
        REN1 [] ;
     : REN1 [retC] D AFM 2+ MLPATHNAME AFM0 2+ !SB [] ;
     PBIT REN
     : REN2 [] AFM0 ! AFM DERR [] ;



[::] : DELETE [A,L] CONNECT CH2 DEL CH2 ;
[::] : DELF [] CIWD DELETE ;

[::] : CREATE [A,L] CONNECT CH2 CRT CH2 ;
[::] : CRTF [] CIWD CREATE ;

[::] : RENAME [Anew,Lnew,Aold,Lold] CONNECT CH2 REN CH2 ;
[::] : RENF [] ON ZAPROS ." в " CIWD
               ON ZAPROS ." из " CIWD CONNECT CH2 REN CH2 ;

       CHANNEL CH2  [ИСПОЛЬЗУЕТСЯ В SLENB]
       CHANNEL CH1  [ИСПОЛЬЗУЕТСЯ В REN]
:: CHANNEL CH [канал для пользователей]

[::] : IS [A,L] GTP !AFM Sis' [] ;  PBIT IS
     : Sis' [A,L] C pos' + lenb' > BR+ DDEOF DISA1 [] ;
     : DDEOF DD EOF ;

[::] : OS [A,L] GTP !AFM DOSA1 [] ;  PBIT OS

[::] : OB [b] GTP !AFM ' BBBUF E2 C2 !TB 1 DOSA1 [] ;  PBIT OB

[::] : IB [] GTP !AFM _IB [b] ; PBIT IB
     : _IB [] ' BBBUF C 1 Sis' @B  [b] ;

[::] : OW [w] GTP !AFM ' BBBUF E2 C2 !TW  2 DOSA1 [] ; PBIT OW

[::] : IW [ ] GTP !AFM ' BBBUF C 2 Sis' @W  [w] ;  PBIT IW


[::] : OLW [l] GTP !AFM ' BBBUF E2 C2 !TL 4 DOSA1 [] ;  PBIT OLW

[::] : ILW [ ] GTP !AFM ' BBBUF C 4 Sis' @L  [l] ;  PBIT ILW

   OWN LONG VAR BBBUF [buffer for i/o Bytes, Words, Double words]

B10
OWN LONG VAR IRL
OWN LONG VAR ARL
OWN LONG VAR LRL
: READLN [A,L] ! LRL ! ARL 0 ! IRL GTP !AFM
         EON EOF EOFRL
         RP RL-LOOP
         ARL IRL 0 [A,L,0/1] ;
: RL-LOOP [] _IB C 13 = IF+ EX13 [B] IRL LRL < BR+ ADDRL D [] ;
: ADDRL [B] ARL IRL + !TB IRL 1+ ! IRL [] ;
: EX13 [13] D _IB D EX [] ;
: EOFRL [] ARL IRL 1 [] ;

CR ."Драйвер АЦПУ"
B16
[::] : LPB' [b] ! REDX 500 ! REAX 21 INTERR [] ;
[::] FIX ACT VAR LPB  '' LPB' ! LPB
B8
[::] : LPN [Ч,ВСЕГО] TON0 [КОДЫ,N] DO LPB [] ;
[::] : LPCR 12 LPB ;
[::] : LPFF 14 LPB ;
[::] : LPSP #  LPB ;
[::] : LPT [N] 15 LPB DO LPSP [] ;
[::] : LPS [a,l] DO LPS1 D [] ;
 : LPS1 [a] C @B LPB 1+ [a+1] ;

[::] : LPF CIWD CONNECT CH OPEN CH LENB CH DO LPF1 CLOSE CH ;
     : LPF1 IB CH LPB ;
