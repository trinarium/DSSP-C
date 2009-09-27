B16 CR ."DIR and find files" CR
[::] : DIR [] S( BASE@ ) B10 EON ESCR SCOFF SCON NMS? BR0 PDN0 PDN1 FFF
       [code] BR 0 DIR1 2 FILENF 3 PATHNF 12 FILENF ELSE SCOFFDERR SCOFF ;
: SCOFFDERR SCOFF DERR ;
: FILENF ."
Files not found" ;

[PATHNF - процедура для возбуждения ситуации DERR при неправильном каталоге]
: PATHNF [] CR ALPNF EXERR [] ;
   : ALPNF ."Каталог не найден" ;

   : PDN0 [] PDN01 ."*.*" [] ;
     : PDN01 [] CR ."Текущий каталог " PCDIR [] ;
   : PDN1 [] SPATH? IF0 PDN10 ;
     : PDN10 [] PDN01 ALPNAME TOS [] ;
     : PCDIR GCDIR  [A,L] TOS [] ;
     : SPATH? [] ALPNAME [A,L]
       #\ C3 C3 SRCHB [A,L,L1]   C2 = [A,L,i]
       #: C4 C4 SRCHB [A,L,i,L1] C3 = & [A,L,i]
       E2D E2D NOT [1-path is set, 0-isn't] ;
   : DIR1 [] .DIR RP DIR2 [] ;
     : DIR2 [] FNF [code] NOT EX0 .DIR [] ;
       : .DIR [nnnnnnnn    sd
               nnnnnnnn eee r   .length.   dd mmm year    hh.mm ? ]
         [исследование первого символа имени]
         @S_ALFNAME D @B 
         BR 0 NOP 5 .DIR0PFS [2E NOP] 0E5 NOP ELSE .DIR0 [] ;

   : .DIR0PFS [] 0E5 @S_ALFNAME D !TB [] .DIR0 [] ;
   : .DIR0 [] @B_ATR 10 & BR0 .DIR1 .DIRSD [] ;

  : .DIRSD [] 2 ! UCOLOR .DIR1 [] 7 ! UCOLOR ;

   : .DIR1 [] CR @B_ATR 10 & BR0 .DIR1SIZE .">SUB-DIR<"
     SP .DIR1DATE SP .DIR1TIME SP .DIR1NAME SP .DIR1r [] ;

0 %IF
   : .DIR1 []  [] @B_ATR 10 & BR0 .DIR1SIZE .">SUB-DIR<" 
     SP SP .DIR1DATE SP .DIR1TIME SP .DIR1NAME SP .DIR1r [] ;
%FI

:: : .DIR1NAME [] @S_ALFNAME #. C3 C3 SRCHB [A,L,L.]
     0  C4 C4 SRCHB MIN [A,L,L.] C3 C2 TOS [A,L,L.]
     9 C2 - DO SP E2D + [A.] 0 C2 4 SRCHB [AE,LE]
     C2 C2 TOS 4 C2 - DO SP DD [] ;
   : .DIR1r [READ ONLY] @B_ATR 1 & BR0 SP ."r" [] ;
:: : .DIR1SIZE [] @_SIZE 9 TON [] ;
:: : .DIR1DATE [] @_DATE  1F & 2 TON [] SP @_DATE -5 SHT 0F & 
     BR  1 ."jan"  2 ."feb"  3 ."mar"  4 ."apr"  5 ."may"  6 ."jun"
         7 ."jul"  8 ."aug"  9 ."sep" 0A ."okt" 0B ."nov" 0C ."dec"
     ELSE ."..." [] SP @_DATE -9 SHT 07BC [=1980] + 4 TON [] ;
:: : .DIR1TIME [] @_TIME -0B SHT 2 TON [] #. TOB
     @_TIME -5 SHT 3F & 2 TON [] ;

[SCDIR - Set Current DIRectory]
[::] : CD [] PATH CD' [] ;
     : CD' [] !0 NMS? SCDIR' ?CF [1/0] IF+ DERR [] ;
     : SCDIR' [] ALPNAME C2 1+ @B #: = BR+ SCDIR1 SCDIR3 [] ;
     : SCDIR1 [A,L] C2 C2 SCDRIVE C 2 = BR0 SCDIR3 DD [] ;
0 %IF
     : SCDIR3 [A,L] ASCIIZ ! REDX 3B00 ! REAX 21 INTERR [] ;
     : SCDRIVE [a,l] D @B #A - ! REDX 0E00 ! REAX 21 INTERR ?CF IF+ DERR [] ;
%FI
     : SCDIR3 [A,L] ASCIIZ CHDIR_C [] ;
     : SCDRIVE [a,l] D @B #A - SETDRV_C IF+ DERR [] ;


[::] : CDIR [A,L] PATH' [] CD' [] ;

0 %IF
[GCDIR - Get Curreent DIRectory]
  :: : GCDIR [] GCDRIVE [drv] GCDIRDRV [A,L] ;
     : GCDIRDRV [drv] [справочник для данного устройства]
       ! REDX @R S( PTM ) !R MLPATHNAME TUSE [A]
       REDX 1- #A + C2 !TB #: C2 1+ !TB [A]
       C 3+ [A,A+3] ! RESI 4700 ! REAX [A] 21 INTERR
       ?CF BR+ 2 GCDIRYES [A,L] ;
     : GCDIRYES [A] C 3+ @B BR0 GCDIR3 GCDIR2 [A,L] ;
     : GCDIR2 [A] #\ C2 2+ !TB  0 C2 MLPATHNAME SRCHB [A,L]
       #\ C3 C3 + !TB 1+ [A,L] ;
     : GCDIR3 [A] #\ C2 2+ !TB 3 [A,L] ;
  :: : GCDRIVE [] 1900 ! REAX 21 INTERR REAX 0FF & 1+ [drv] ;
%FI


[Версия для Си]
LONG VAR EDX

[GCDIR - Get Curreent DIRectory]
  :: : GCDIR [] GCDRIVE [drv] GCDIRDRV [A,L] ;
     : GCDIRDRV [drv] [справочник для данного устройства]
       ! EDX @R S( PTM ) !R MLPATHNAME TUSE [A]
       EDX 1- #A + C2 !TB #: C2 1+ !TB [A]
       C 3+ [A,A+3] EDX [A,A+3,DRV] GETCHDIR_C [A,CF] BR+ 2 GCDIRYES [A,L] ;
     : GCDIRYES [A] C 3+ @B BR0 GCDIR3 GCDIR2 [A,L] ;
     : GCDIR2 [A] #\ C2 2+ !TB  0 C2 MLPATHNAME SRCHB [A,L]
       #\ C3 C3 + !TB 1+ [A,L] ;
     : GCDIR3 [A] #\ C2 2+ !TB 3 [A,L] ;
  :: : GCDRIVE [] GETDRV_C [drv] 0FF & NOP [1+] [drv] ;



[!!!!!!! далее не переделано !!!!!!!]
[FFF - найти первый файл (directory entry)
       возвращает 0, если файл найден, или код ошибки <>0, если
       не найден DTA файла.
       FATRS - атрибуты поиска файла.
       NMS? - point that pathspec set]
400 BYTE VCTR DTA
   FIX WORD VAR NMS?   0 ! NMS?
:: FIX WORD VAR FATRS 17 ! FATRS
:: 0 ' DTA VALUE ASDTA

0 %IF
:: : FFF [] [Set DTA] 1A00 ! REAX  ASDTA ! REDX 21 INTERR
     [FFF] NMS? !0 NMS? [NMS?] BR0 "*.*" ALPNAME [A,L]
     ASCIIZ [AS] ! REDX FATRS ! RECX 4E00 ! REAX 21 INTERR REAX [code] ;

[FNF - find next file (directory entry).
       returns 0, if file found, or error code <>0, if not found.
       Filles DTA.]
:: : FNF [] ASDTA ! REDX 4F00 ! REAX 21 INTERR REAX [code] ;

[Процедуры, выдающие атрибуты, время, дату, размер и имя файла из DTA (new format)]
:: : @B_ATR [] ASDTA 15 + @B [atrs] ;
:: : @S_ALFNAME [] ASDTA 1E + 0 C2 0D SRCHB [A,L] ;
:: : @_TIME [] ASDTA 16 + @W [time] ;
:: : @_DATE [] ASDTA 18 + @W [date] ;
:: : @_SIZE [] ASDTA 1A + @L [size] ;

%FI

[C version]
:: : FFF [] ASDTA NMS? !0 NMS? [NMS?] BR0 "*.*" ALPNAME [A,L]
     ASCIIZ ['DTA,AS] FATRS ['DTA,AS,FATR] FFF_C [code] ;

[FNF - find next file (directory entry).
       returns 0, if file found, or error code <>0, if not found.
       Filles DTA.]
:: : FNF [] ASDTA ['DTA] FNF_C [code] ;



[PATH - set pathspec in form:
        "b:\path\filename" or "filename" (in current dir)
       Symbols *,? are aiiowed. Example: PATH *.DSP  DIR]
[300 - AVR possible pathspec length (Win95+ restriction)]
 300 C VALUE MLPATHNAME
  BYTE VCTR PATHNAME  [buffer for pathspec]
  WORD VAR  LPATHNAME [current length of pathspec]
[::] : PATH [] CIWD [A,L] PATH' [] ;
:: : PATH' [A,L] C ! LPATHNAME 0 ' PATHNAME !SB [] !1 NMS? [] ;
:: : ALPNAME [] 0 ' PATHNAME LPATHNAME [A,L] ;

[ DTA_new
   0  4h  attr
   4  4h  T_create
   8  4h  T_access
   C  4h  T_write
  10  4h  size
  14  4h  handle findfirst
  18  260 name ]

[Процедуры, выдающие атрибуты, время, дату, размер и имя файла из DTA (new format)]
:: : @B_ATR [] ASDTA @L [atrs] ;
:: : @S_ALFNAME [] ASDTA 18 + 0 C2 100 SRCHB [A,L] ;
:: : @_TIME [] ASDTA 6  + @W [time] ;
:: : @_DATE [] ASDTA 4  + @W [date] ;
:: : @_SIZE [] ASDTA 10 + @L [size] ;
