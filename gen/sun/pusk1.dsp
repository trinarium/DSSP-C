B16 
." 
Kernel DSSP/c" 
CR 
:: 7 VALUE 'PSP                          [селектор PSP] 
 
:: FIX WORD VAR SCOLOR 2 ! SCOLOR        [цвет для служебных сообщений] 
:: OWN WORD VAR UCOLOR 7 ! UCOLOR        [цвет обычного текста] 
 
   FIX ACT VAR BYE  '' NOP ! BYE         [Процедура выхода в DOS] 
:: FIX ACT VAR AR   '' NOP ! AR          [Процедура, которую 
                                           запускает PUSK, обычно ZMON] 
 
:: FIX ACT VAR IDI  '' NOP ! IDI          [Процедура инициализации 
                                           драйвера диска] 
:: FIX ACT VAR ITE '' NOP ! ITE           [Процедура инициализации дисплея] 
 
:: OWN WORD VAR BASE@ 10 ! BASE@          [Основание текущей 
                                           системы счисления] 
             '' BASE@ __ ''BASE@ 

 
            FIX LONG VAR PTM [указатель временного стека] 
            FIX LONG VAR NB  [Указатель области тел] 
             '' NB __ ''NB 
            FIX LONG VAR D1  [Указатель области данных] 
             '' D1 __ ''D1 
            FIX LONG VAR YD1 [Указатель по таблице сохp.данных] 
             '' YD1 __ ''YD1 
            FIX LONG VAR WD1 [Указатель по таблице собств.данных] 
             '' WD1 __ ''WD1 
 
 
:: END_MEM  VALUE END_MEM  [верхняя граница памяти DSSP386] 
:: BEG_YD1  VALUE BEG_YD1  [ук-ль начала области сохраняемых данных] 
:: END_YD1  VALUE END_YD1  [ук-ль конца области сохраняемых данных] 
:: BEG_WD1  VALUE BEG_WD1  [ук-ль начала области собственных данных] 
:: END_WD1  VALUE END_WD1  [ук-ль конца области собственных данных] 
:: BEG_PTM  VALUE BEG_PTM  [ук-ль начала временного стека] 
:: END_PTM  VALUE END_PTM  [ук-ль конца временного стека] 
:: BEG_D1   VALUE BEG_D1   [ук-ль начала области данных] 

 
:: FIX WORD VAR REENT  0 ! REENT          [Шкала повторных входов] 
 
'' WIMAGEA  ABSOLUT :: '' WIMLEN 1- BYTE VCTR WIMAGE 
                                      [Образ слова из входной строки] 
:: WIMLEN  VALUE WIMLEN  [длина образа слова] 
 
:: FIX LONG  VAR SWIM   '' SWIMA ! SWIM   [Адрес образа слова] 
 
1E VALUE DBIT  [номеp D-бита в 32-битном слове] 

 
 
[значения, подлежащие восстановлению при RESTART] 
[FIX] WORD VAR UCOLOR1        [копия пользовательского цвета] 
[FIX] LONG VAR TOBOLD         [копия TOB для RESTART        ] 
 
 
[н] : ''NOP '' NOP ; 
[н] :: : R? [M] REENT  C2 C2 &0 ! REENT  & [M&REENT] ; 
 
[  !!!DF!!!  ] 
 
:: : PUSK [1/0] 
  DSRESET [подготовка для работы с данными] [TUNE] [win32 only] 
  BR0 FPUSK NPUSK  
  LDINTS
  BEG_PTM ! PTM 
  IDI [] ITE [] 
  SCOFF [] 
  AR [] 
  BYE [----------> выход в DOS] 
  ; 
'' PUSK __ ''PUSK 
 
: NPUSK [] ' REENT 0F !BI1 UCOLOR1 ! UCOLOR TOBOLD ! TOB TTO [] ; 
: FPUSK [] !0 REENT UCOLOR ! UCOLOR1 ' TOB @L ! TOBOLD SVINTS [] ; 
: THR YD1 0 THRNAME ; 
 
:: FIX ACT VAR SVINTS [сохpанение значений вектоpов пpеpываний] 
:: FIX ACT VAR LDINTS [установка  ----------"-----------------] 
:: FIX ACT VAR RSINTS [восстановл ----------"-----------------] 
  '' NOP ! SVINTS 
  '' NOP ! LDINTS 
  '' NOP ! RSINTS 
 
: BYE0 HALT ; 
: NBYE RSINTS BYE0 ; 
 
  '' NBYE ! BYE 
 
: TSPUST ." 
stack empty" ; 
 
[пpоцедуpы для pаботы с PTM - вpеменной памятью] 
:: : !TM [A,DL] E2 C2 [DL,A,DL] C TUSE !SB [DL] PUSH [] ; 
:: : TUSE [n] C SSTSTM [n] PTM E2 ADDPTM [A] ; 
:: : @TM [A] POP 3+ 3 INV & [A,DL] PTM C2 - C ! PTM E3 [PTM,DL,A] !SB [] ; 
:: : @PTM PTM 4- @L ; 
:: : PUSH [l] 4 SSTSTM PTM !TL 4 !+ PTM [] ; 
:: : POP [] 4 !- PTM PTM @L [x] ; 
:: : ADDPTM [n] 3+ 3 INV & !+ PTM [] ; 
   : SSTSTM [n] PTM + END_PTM < IF0 NOMEM! [] ; 
