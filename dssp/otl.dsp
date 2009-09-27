1 %IF
PROGRAM $DEBUG B8 USE SYSTEM
%FI

CR ."Отладчик  ДССП-32  i80386"
B16

LONG     VAR OTL1  [АДРЕС Т.О.]
LONG     VAR OTL2  [СОДЕРЖИМОЕ ПО АДРЕСУ Т.О.]
LONG     VAR OTL6  [КРАТНОСТЬ Т.О.]

[MON3 - ОБРАБОТКА ОСТАНОВА]
 : MON3 [] !1- OTL6  OTL6  BR0 OSTANOV \G [] ;
 : OSTANOV []  OTL1 ."
останов перед "   [OTL1] VYDAT D    1 \M   OTL2 CPC 4+ ZMOTL [] ;

[\SE - назначение дополнительных действий при останове]
:: : \SE [] GTP ! APO [] ;  PBIT \SE
[\P - ПОСТАНОВКА Т.О.]
:: : \P [I]  \C  ON VYPL SBP  \E  1 \M   [] ;
[\C - ОТКАЗ ОТ Т.О.]
:: : \C [] 2 R?   IF+ \C1  !0 OTL1 [] ;
 : \C1 []  OTL1 MON3? <!TL [] ;
[\M - ЗАДАНИЕ КРАТНОСТИ Т.О.]
:: : \M [I]   ! OTL6   [] ;

TRAP VYPL   EW
:: : \E [I-номер команды] PUSH []
     EON EXERR NEZ [реакция на неопределенное слово]
     EON NINT OOTL2 [реакция на неинтерпретируемое слово]
     POP [I]
     WRDF E2D [I,AT] INTLEN + [I,ADR - адрес первой команды]
     E2 1 MAX
     [at,max(i,1)] 1- DO \P3 [ADR+4*I] [движение на I-ую команду]
     VYDAT  VYPL [] ;
: \P3 [ADR] MON3?  ''; = EX+  4+ [ADR+4] ;
[SBP - ПОСТАНОВКА Т.О.]
: SBP [ADR] C@L ! OTL2   ! OTL1  '' MON3 OTL1 !TL [] ;

:: : \T [n]  OTL1 \C E2 DO \P3 [ADR] SBP  \G [] ;

[\+ - перенос точки останова на 1 слово вперед без выполнения]
:: : \+ [] 1 OTL1 \C E2 DO \P3 [ADR] SBP  [] ;
[\,\\,\\\ - перенос точки останова на 1,2,3 слова вперед с выполнением]
:: : \ 1 \T ;
:: : \\ 2 \T ;
:: : \\\ 3 \T ;

[EW - ЗАМЕНА СЛОВА ПО АДРЕСУ УПОТРЕБЛЕНИЯ СЛОВОМ ИЗ ВХОДНОЙ СТРОКИ]
: EW [KUDA] EON NUMBER NNUM  ON NINT NOP
            WRDF E2D [KUDA,AT] <!TL [] ;
[Подстановка константы (ее генерация в случае отсутствия)]
: NNUM [KUDA] TIN1 [KUDA,C] C-BA [KUDA,ATC] <!TL [] ;

: ZGL [BA,AZ] CR #: TOB SP C BR0 *TON OUTW CR [BA] ;
: *TON [BA,0]  D SP #* TOB C 8 TON 17 DO SP [BA] ;

[VYDAT - ВЫДАЧА ИМЕНИ СЛОВА ПО АДРЕСУ УПОТРЕБЛЕНИЯ]
: VYDAT [ADR] MON3? C '' ZAGL =  BR+ BYA HPOISK [ADR,az/a0]
         C@L  BR0 NET0 OUTW1 [ADR] ;
: OUTW1 OUTW SP SP ;
[Поиск ссылки на неопpеделенное слово]
: BYA [ADR,*] D C SPOISK [ADR,AZ] ;

[выдача содержимого по адресу с учетом наличия точки останова]
: MON3? [ADR] C@L '' MON3 = [адрес точки останова?]
    BR0 C@L OTL2 [ADR,@ADR] ;

[Слова в словаpе нет, может быть константа?]
: NET0 [ADR,A0] D MON3? [ADR,BA]
      [тело константы Call CnstBody | Lo | Hi ]
      C  CALLADR [ADR,BA,?''CnstBody] '' CnstBody =
      C2 CALLLEN + @L E2 [ADR,@ADR,C,1/0] BR0 *TON VYDC  D [ADR] ;
: VYDC [ADR,@ADR,C] SP 8 TON 18 DO SP  [ADR,@ADR] ;

: OOTL2 [*,*] ."
примитив" ENDL [] ;

[ПОИСК ПО АДРЕСУ ТЕЛА АДРЕСА 1-ГО СЛОВА ЗАГОЛОВКА]
:: : HPOISK [AT]
     S( AVOC )
     HPSK [AT,AF] [Поиск в текущем словаpе]
     [AT,AF] E2D [AF] ;

[Поиск в словаpе ДССП-пpоцессоpа]
: HPSK [AT] S( V1 NV ) [LDVOC]
     ''NOP C '' HP1 DSRCH AVOC [AT,AZ/0] ;
: HP1 [AT,*,*,*,AZ] AVOCBA 6 CT = IF+ FSRCH [AT,*,*,*,A3] ;

[Новая реакция на ситуации NOIND и ERSTR]
: NOINDM ."
 Индекс? " .B16 D  CPC 8 +  @L  \B0 [BA,n]
   .", слово" .B10 D ." в процедуре " [BA] D1 4+ !TL D1 4+ VYDAT D
   EXERR [] ;


[Подстановка прямо в тело!!!]
'' NOINDM '' NOIND INTLEN + 4+ C NUSE !TL
'' NOINDM '' ERSTR INTLEN + 4+ C NUSE !TL

[Новая процедура для V_PROC в MDT.DSP]
: V_PROC0 [A в теле] \B0 [BA,n] D ." в процедуре "
          [BA] D1 4+ !TL D1 4+ VYDAT D [] ;

' V_PROC NUSE '' V_PROC0 ! V_PROC


[По адресу слова в процедуре выдает адрес тела процедуры
 и номер слова в нем]
: \B0 [a в проц.] C RP \B1 INTLEN - [a,BA процедуры]
      E2 C2 [BA,a,BA] INTLEN + - SHR SHR
      [BA,n=номер слова в процедуре] ;
   : \B1 [a] C 2- @W '' \B1 @W  = EX+ 2- [a-2] ;

TRAP NUMBER NOP
TRAP NINT NOP
[поиск слова,
 проверка на интерпретируемое (если неинтерпретируемое, то NINT),
 если не нашли, то если константа, то NUMBER]
: WRDF []   WRD FIND BR0 ADRS CBA [AZ,AT]
   C ?INT IF0 NINT [AZ,AT] ;
: ADRS [*] T0 [0]  @ATEK #- =  @ATEK BRD E2D  &0
       IF+ NUMBER
       [] #* RL = IF0 EXERR    TIN1 [0,AT] ;

'' *TON ,L '' HPOISK ,L
[\L - РАСПЕЧАТКА ТЕКСТА СЛОВА ИЗ ВНУТРЕННЕГО ПРЕДСТАВЛЕНИЯ]
:: : \L []  EON EXERR NEZ   EON NINT OOTL2
       WRDF [AZ,BA] E2 ZGL [AT] INTLEN + 0 RP \L1 [*] DD [] ;
: \L1 [ADR,N] SP E2 C@L '' MON3 = BR+ TOB177 SP
      VYDAT [N,ADR] \P3 [N,ADR+4]
      E2 1+ C 2 / IF0 CR D [ADR+4,N+1] ;
: TOB177 #▄ TOB ;


0 %IF
SHUT SYSTEM
CLEAR $DEBUG
UNDEF
  %FI
