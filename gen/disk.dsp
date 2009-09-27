CR ."LOAD, TYPE, SAVE" CR 
 
B16 
 
[   ***********************************   ] 
[   СРЕДСТВА БУФЕРИЗАЦИИ ВВОДА ДЛЯ LOAD   ] 
[                                         ] 
[      Интерфейс:                         ] 
[      LB-MLEN, LB-LEN, LB-PTR,           ] 
[      LB-INIT, LB-ADR, LB-RDB,           ] 
[      CHLD,    EOF,    LB-RRD            ] 
[                                         ] 
 
CHANNEL CHLD 
 
[LB-MLEN - максимальная длина буфеpа загpузки 
           не должна пpевышать pазмеp сегмента 
           загpузки ] 
1000 VALUE LB-MLEN 
1000 BYTE VCTR LB-BUF [буфер загрузки] 
 
    WORD VAR LB-LEN  [сколько байтов в буфеpе загpузки] 
    WORD VAR LB-PTR  [номер текущего байта в буфеpе загpузки] 
    LONG VAR LB-ADR  [начальный адpес буфеpа загpузки] 
 
[LB-INIT - начальная подготовка буфера LOAD] 
: LB-INIT [] 0 ' LB-BUF ! LB-ADR [] !0 LB-PTR !0 LB-LEN [] ; 
 
[LB-RDB - ввод очередного байта, если больше нет - EOF] 
: LB-RDB [] LB-PTR LB-LEN < IF0 LB-NEXT [] 
         [] LB-ADR LB-PTR + @B [ZZZ] !1+ LB-PTR [b] ; 
 
: ZZZ CR ."LB-PTR=" LB-PTR . D ."      B=" . SP C TOB ; 
 
[LB-NEXT - подкачка очередной порции в буфер] 
: LB-NEXT [] [TYPEREG] 
          LENB CHLD POS CHLD - LB-MLEN MIN C IF0 EOF [L] 
          [L] LB-ADR C2 IS CHLD ! LB-LEN !0 LB-PTR [TYPEREG] [] ; 
 
[: EOF1 TEST1 EOF ;] 
 
[LB-RRD - повторное обновление буфера (ReReaD)] 
: LB-RRD 
         [] POS CHLD LB-LEN - SPOS CHLD [] 
         [] LB-ADR LB-LEN IS CHLD [] ; 
 
[                                         ] 
[   СРЕДСТВА БУФЕРИЗАЦИИ ВВОДА ДЛЯ LOAD   ] 
[   ***********************************   ] 
 
 
B16 
 
[R4L - процедура для ввода входной строки из файла] 
[::] : R4L []                        
      [проверка на исчерпание файла ввода] 
      SKBV IF0 KW 
      SISTR 0 LISTR DO R4LINB [A,L] 
      [A,DL] SKBV C2 - ! SKBV [A,L]  
      C DO R4LDEC [A,L] ; 
[R4LDEC - если последние символы упpавляющие, то выкинуть их] 
: R4LDEC [A,L] C2 C2 + 1- @B 
     BR 0A 1-  1A 1-  0D 1- ELSE EX [A,L without control symb] ; 
[R4LINB - ввод очередного байта] 
: R4LINB [A,L] EON EOF EX LB-RDB [a,l,B] C 
     BR   9 #       [Tab] 
         1A DEXEOF  [EOF] 
       ELSE C E2D   [A,L,B] 
       C C4 C4 + !TB E2 1+ E2 [A,L+1,B] 
       0A [LF] = EX+ [A,L] ; 
   : DEXEOF [A,L,B] D C ! SKBV EX ; 
 
 
[::] : LOAD [] CIWD [a,l] LOAD0 [] ; 
:: : LOAD0 [a,l] HIDE  LOAD01  RESTOR [] ; 
   : LOAD01 [A,l] 
       S( LB-LEN LB-PTR ) 
       [a,l] CONNECT CHLD 
       SIS 
       OPEN CHLD LENB CHLD LB-INIT 
            LOAD1 
       CLOSE CHLD 
       LIS 
       BELL [] ; 
: LOAD1 [SKBV] ON RESTOR LDSPLD ON HIDE SDSPLD MONON R4L ZMON ; 
 
: LDSPLD [] LDSP CHLD [] LB-RRD [] ; 
: SDSPLD [] SDSP CHLD [] ; 
 
 
[::] : TYPE [] CIWD [a,l] TYPE0 [] ; 
:: : TYPE0 [a,l] S( PTM ) 
          CONNECT CH  OPEN CH  SCON  EON ESCR TYEND 
          400 C TUSE E2 [A,L] 
          CR  LENB CH  RP TYPE1 DDD  TYEND ; 
     : TYEND SCOFF CLOSE CH ; 
     : TYPE1 [A,L,lenb] C EX0 
              C3 C3 C3 MIN [A,L,lenb,A,dL] 
              C2 C2 IS CH E2 C2 TOS [A,L,lenb,dL] - ; 
 
[*************** SAVE ***************] 
B16 
0 %IF 
'' EXEPLHEAD 1112 + VALUE 'EXPHD [адрес модифицируемой части заголовка] 
 
[SAVE - вывод системы в данной конфигурации в файл на диск] 
[::] : SAVE [] CIWD [a,l] SAVE0 [] ; 
:: : SAVE0 [a,l] 
       ON .RAS ".EXE" [a,l] CONNECT CH 
       FPPRS CH FNPRS CH E2D + OWRF? [1/0] IF+ SAVE' ENDL [] ; 
     : SAVE' [] 
       [SQVOC] 
       WOPEN CH 
       MDFEXEHD 
       '' EXEPLHEAD 1312 OS CH [вывод EXE+EXP-заголовка] 
   [вывод системы] 
       0 NV OS CH             [тела+словарь] 
       BEG_YD1 YD1 C2 - OS CH [сохраняемые данные] 
       BEG_WD1 WD1 C2 - OS CH [собственные данные] 
       CLOSE CH [] ; 
 
 
LONG VAR ADR_IMG [позиция в образе загрузки в файле, откуда брать блок] 
[MDFEXEHD - модификация образа EXE+EXP-заголовка] 
: MDFEXEHD [] 
[Запись информации о сохраняемых блоках] 
           NV ! ADR_IMG  [позиция в образе загрузки, откуда брать блок] 
           '' MEM_SIZE [''MEM_SIZE=''MS] 
           [''MS]   END_MEM C2!TL4+ 
           [''MS+4]       2 C2!TL4+ [кол-во блоков] 
[блок сохраняемых данных] 
           [am]       BEG_YD1    C2!TL4+ [нач.знач.YD1] 
           [am+4] YD1 BEG_YD1 -  C2!TL4+ [длина сохр.данных в байтах] 
           [am+8] ADR_IMG        C2!TL4+ [нач.сохр.данных в файле] 
           [am+8] YD1 BEG_YD1 - !+ ADR_IMG 
[блок собственных данных] 
           [am]       BEG_WD1    C2!TL4+ [нач.знач.WD1] 
           [am+4] WD1 BEG_WD1 -  C2!TL4+ [длина собств.данных в байтах] 
           [am+8] ADR_IMG        C2!TL4+ [нач.собств.данных в файле] 
           [am+8] WD1 BEG_WD1 - !+ ADR_IMG 
           [am] D 
[модификация образа EXE+EXP-заголовка] 
           ADR_IMG 1312 + 
           [общая длина файла=L] C 1112 - [L,длина EXP-файла=LE] 
           [L,LE]  'EXPHD 6  + !TL         [длина EXP-файла в байтах] 
           [L] '' ARSB @L [L,CSB] 'EXPHD 62 + !TL         [нач.зн.ESP] 
           [L] '' ARSB @L '' AOPSB @L - 'EXPHD  7C + !TL [длина стека] 
         [ {L} C 200 / E2 C2 IF+ 1+ {L,bytes,all pages} 
               '' EXEPLHEAD 4+ !TW '' EXEPLHEAD 2+ !TW ] 
           [L] 1312 - C 'EXPHD 2A + !TL    [разм.образа прогр.в файле] 
           [l]          'EXPHD 74 + !TL    [разм.образа прогр.в памяти] 
           [] ; 
: C2!TL4+ [a,x] C2 !TL 4+ [a+4] ; 
 
%FI 
B16 
[**************************************************] 
CHANNEL CH4 
 
[::] : OWRF? [A,L] CONNECT CH4 []  
        EON NOF 1 OPEN CH4 CLOSE CH4 
        5 DO BELL ." 
File " FNAME CH4 ." already exists! 
Overwrite" Q? [1/0] ; 
:: : Q? ."?(Y/N)-" RP Q1 [1/0] ; 
    : Q1 [] TRB C BR #Y EXT1 #y EXT1 #N EXT0 #n EXT0 
                     #Н EXT1 #н EXT1 #Т EXT0 #т EXT0 ELSE D [] ; 
    : EXT1 TOB 1 EX ; 
    : EXT0 TOB 0 EX ; 
 
0 %IF 
:: : TYPEREG [] 
     CR ."EAX=" REAX . D ."   EBX=" REBX . D ."   ECX=" RECX . D 
     CR ."EDX=" REDX . D ."   EBP=" REBP . D ."   ESI=" RESI . D 
     CR ."EDI=" REDI . D ."    DS=" RDS  . D ."    ES=" RES  . D 
     CR ." SS=" RSS  . D ."    CS=" RCS  . D ."    FS=" RFS  . D 
     CR ." GS=" RGS  . D ."   EFL=" REFL . D 
     CR ."LB-ADR=" LB-ADR . D ."   LB-LEN=" LB-LEN . D 
        ."   LB-PTR=" LB-PTR . D TRB D ; 
%FI 
 
[*************** SAVE ***************] 
B16 
1 %IF 
B10 
96 VALUE SUMHEADER         [ длина заголовка ядра = 96 в 10-ой ] 
[SUMHEADER ] 
96 BYTE VCTR Header [заголовок ядра] 
: C2!TLHEAD4+ [am,x] C2 ' Header !TL 4+ [am+4] ; 
 
CHANNEL CH3 
B16 
[SAVE - вывод системы в данной конфигурации в файл на диск] 
[::] : SAVE [] CIWD [a,l]  SAVE0 [] ; 
:: : SAVE0 [a,l] 
       ON .RAS ".DPF" [a,l] CONNECT CH3 
       FPPRS CH3   FNPRS CH3  CR E2D + OWRF? [1/0] IF+ SAVE' ENDL [] ; 
     : SAVE' []  
       SQVOC 
       WOPEN CH3 MDFHD 
   [вывод системы] 
       0 ' Header SUMHEADER OS CH3 
       0 NV OS CH3             [тела+словарь] 
       BEG_YD1 YD1 C2 - OS CH3 [сохраняемые данные] 
       BEG_WD1 WD1 C2 - E2  OWNBASE + E2 OS CH3 [собственные данные] 
       CLOSE CH3 [] ; 
 
LONG VAR ADR_IMG [позиция в образе загрузки в файле, откуда брать блок] 
 
: MDFHD [] 
[Запись информации о сохраняемых блоках] 
           #D 0 ! Header #P 1 ! Header #F 2 ! Header 0 3 ! Header [DPF\0 - начало файла] 
           [NV ! ADR_IMG]  [позиция в образе загрузки, откуда брать блок] 
           0 ' Header 4 + 
           END_MEM 	C2 	!TL 4+     	[длина памяти] 
           4     	C2	!TL 4+      [кол-во блоков] 
[Блок 1 - тела ]           
           0        C2	!TL 4+      [куда] 
           NB		C2	!TL 4+  	[длина] 
[Блок 2 - словарь ]           
           NB		C2	!TL 4+  	[куда] 
           NV NB -  	C2	!TL 4+  	[длина] 
[блок сохраняемых данных]  
           BEG_YD1 	C2     !TL 4+ 		[нач.знач.YD1] 
           YD1 BEG_YD1 - C2  	!TL 4+ 		[длина сохр.данных в байтах] 
          [ ADR_IMG 	C2     !TL 4+] 		[нач.сохр.данных в файле] 
          [ YD1 BEG_YD1 - !+ ADR_IMG    ] 
[блок собственных данных] 
           BEG_WD1 OWNBASE + C2 !TL 4+ 		[нач.знач.YD1] 
           WD1 BEG_WD1 - C2  	!TL 4+ 		[длина сохр.данных в байтах] 
          [ ADR_IMG 	C2     !TL 4+ ]		[нач.сохр.данных в файле] 
          [ WD1 BEG_WD1 - !+ ADR_IMG     ] 
           D 
[           '' PUSK  '' APUSK         !TL]     	[начало выполнения] 
           ; 
%FI 
0 %IF 
            '' ARSB 
            '' AOPSB 
[модификация образа EXE+EXP-заголовка] 
           ADR_IMG 1312 + 
           [общая длина файла=L] C 1112 - [L,длина EXP-файла=LE] 
           [L,LE]  'EXPHD 6  + !TL         [длина EXP-файла в байтах] 
           [L] '' ARSB @L [L,CSB] 'EXPHD 62 + !TL         [нач.зн.ESP] 
           [L] '' ARSB @L '' AOPSB @L - 'EXPHD  7C + !TL [длина стека] 
         [ {L} C 200 / E2 C2 IF+ 1+ {L,bytes,all pages} 
               '' EXEPLHEAD 4+ !TW '' EXEPLHEAD 2+ !TW ] 
           [L] 1312 - C 'EXPHD 2A + !TL    [разм.образа прогр.в файле] 
           [l]          'EXPHD 74 + !TL    [разм.образа прогр.в памяти] 
           [] ; 
: C2!TL4+ [a,x] C2 !TL 4+ [a+4] ; 
 
%FI 
0 %IF 
 
[LONG VAR ADR_IMG] [позиция в образе загрузки в файле, откуда брать блок] 
[MDFEXEHD - модификация образа EXE+EXP-заголовка] 
: MDFEXEHD [] 
[Запись информации о сохраняемых блоках] 
           NV ! ADR_IMG  [позиция в образе загрузки, откуда брать блок] 
           '' MEM_SIZE [''MEM_SIZE=''MS] 
           [''MS]   END_MEM C2!TL4+ 
           [''MS+4]       2 C2!TL4+ [кол-во блоков] 
[блок сохраняемых данных] 
           [am]       BEG_YD1    C2!TL4+ [нач.знач.YD1] 
           [am+4] YD1 BEG_YD1 -  C2!TL4+ [длина сохр.данных в байтах] 
           [am+8] [ADR_IMG        C2!TL4+] [нач.сохр.данных в файле] 
           [am+8] [YD1 BEG_YD1 - !+ ADR_IMG] 
[блок собственных данных] 
           [am]       BEG_WD1    C2!TL4+ [нач.знач.WD1] 
           [am+4] WD1 BEG_WD1 -  C2!TL4+ [длина собств.данных в байтах] 
           [am+8] [ADR_IMG        C2!TL4+] [нач.собств.данных в файле] 
           [am+8] WD1 BEG_WD1 - !+ ADR_IMG] 
           [am] D 
[модификация образа EXE+EXP-заголовка] 
           ADR_IMG 1312 + 
           [общая длина файла=L] C 1112 - [L,длина EXP-файла=LE] 
           [L,LE]  'EXPHD 6  + !TL         [длина EXP-файла в байтах] 
           [L] '' ARSB @L [L,CSB] 'EXPHD 62 + !TL         [нач.зн.ESP] 
           [L] '' ARSB @L '' AOPSB @L - 'EXPHD  7C + !TL [длина стека] 
         [ {L} C 200 / E2 C2 IF+ 1+ {L,bytes,all pages} 
               '' EXEPLHEAD 4+ !TW '' EXEPLHEAD 2+ !TW ] 
           [L] 1312 - C 'EXPHD 2A + !TL    [разм.образа прогр.в файле] 
           [l]          'EXPHD 74 + !TL    [разм.образа прогр.в памяти] 
           [] ; 
: C2!TL4+ [a,x] C2 !TL 4+ [a+4] ; 
 
%FI 
B16 
[**************************************************] 
 
