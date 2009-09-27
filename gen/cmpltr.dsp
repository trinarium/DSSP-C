B8 CR ."Monitor and conpiler DSSP/c" CR 
 
[ATEK,AKON,ABEG - уkazateli пo ТЕКУЩЕЙ вхodaoй сtpoke] 
:: FIX LONG  VAR ABEG 0 ! ABEG [adpeс naчala tekущeй вхodaoй сtpoki] 
:: FIX LONG  VAR ATEK 0 ! ATEK [adpeс naoбрaбotnaaoгo хвoсta вх сtpoki] 
:: FIX LONG  VAR AKON 0 ! AKON [adpeс koaцa вхodaoй сtpoki] 
 
:: FIX LONG  VAR REGIM -1 ! REGIM 
 
[SISTR,LISTR - oпiсывayut tolьko вхodaуyu сtpokу мoaitopa] 
:: FIX LONG  VAR SISTR [SISTR - adpeс neчala вхodaoй сtoki] 
                       [LISTR - мakсiмalьneя dlina вхodaoй сtрoki = 256] 
:: 400 C VALUE LISTR [LISTR] FIX MUSE ! SISTR 
 
:: FIX LONG  VAR ''; '' ; ! ''; [a] 
::     LONG  VAR SKBV 
       LONG  VAR ABV  [naрazdelяeмыe] 
:: FIX ACT   VAR VVS '' NOP ! VVS 
[::] FIX ACT VAR ?I  '' NOP ! ?I 
::     BYTE  VAR PRIGL 
 
:: TRAP NEZN NEZN0 
: NEZN0 ' AOVS @L '' R4N = IF0 ERRLN NEZ EXERR ; 
:: : NEZ S( UCOLOR ) UCOLOR1 ! UCOLOR NEZNAYU SLOVO BELL ; 
:: : NEZNAYU ." 
unknow " ; 
[ERRLN - выdaчa koaцa сtрoki i zвуkoвoгo сiгnala] 
:: : ERRLN CR ATEK UESTR TOS BELL ; 
[UESTR - сkolьko oсtaloсь бaйtoв вo вхodaoй сtpoke] 
:: : UESTR [] AKON ATEK - [SK] ; 
 
:: : C@UB [AZ] C 34  @BI [@UB] ; 
:: : @SNAME [az] PRS D  @B [пeрвaя liteрa zaгoloвka] ; 
[PRS - пo adрeсу zaгoloвka выdatь (a,l) iмeai] 
:: : PRS [AZ] C'BA  4+ [az,ba+4] 3 DO PRS1 [az,sname] 
         E2 [sname,az] 2+ C2 - [sname,dlz] ; 
   : PRS1 [a] C @B #  = EX0 1+ [a+1] ; 
 
:: : OUTW [AZ] SP  PRS [ADR,DL] C PUSH  TOS  36 POP - DO SP [] ; 
:: : SLOVO [] SWIM OUTW [] ; 
 
[ENDL - oчiсtitь хвoсt вхodaoй сtpoki с tekущeгo adpeсa ATEK] 
:: : ENDL [] AKON ! ATEK  [] ; 
[::] : COFF ''NOP ! VVS ; 
::   : @ATEK [] ATEK @B [B] ; 
:: FIX ACT VAR AOVS 
:: FIX ACT VAR ELN '' NOP ! ELN 
: R3N [] CR TOSIS [CR] PRIGL TOB SP [] ; 
:: TRAP ZAPROS ." 
? " 
 
[TOSIS - ACT VAR - прoцedурa вывoda iaфoрмaцionaoй сtрoki] 
:: FIX ACT VAR TOSIS '' NOP ! TOSIS 
 
: R4N []  S( UCOLOR ) 
      UCOLOR1 ! UCOLOR 
      REGIM 1 = BR+ R3N ZAPROS  
      SISTR LISTR [.">>>"] .STS  [."---"] TIS SP 13 TOB SISTR LTIS  [SISTR,DL] ; 
[WREST - iщet konaц сloвa вo вхodaoй сtрoke do сiмвola пpoбel, TAB ili HOME] 
:: : WREST [] ATEK UESTR 0 E2 DO SRCHEOW E2D [SK] ; 
   : SRCHEOW [A,ctr] C2 C2 + @B BR #  EX 
                                   14 EX 
                                   11 EX 
                                    ELSE 1+ [A,ctr] ; 
[RL - чitnat oчeрedaoй бaйt iz вхodaoй сtрoki. ] 
:: : RL [] UESTR BR+ R RL1 [B] ; 
   : R []   RP STR  @ATEK  !1+ ATEK [B] ; 
   : RL1 [] 12 STR [] ; 
   : STR []   UESTR EX+  ELN AOVS [ADR,DL] 
         C2 + ! AKON  C ! ATEK ! ABEG VVS [] ; 
: BREAD [] R C #[ - IF0 RN [B] #  > EX+ [] ; 
: RN [*] D   RP RN1   #  [40] ; 
: RN1 [] UESTR #] ATEK C3 SRCHB C 1+ !+ ATEK = EX0  RP STR   [] ; 
 
WORD VAR FWWR [Flag Word Was Read] 
:: : <WRD !1 FWWR ; 
[::] : WRD []  FWWR IF0 WRD0 !0 FWWR [] ; 
: WRD0 [] WREST  !+ ATEK  RP BREAD  !1- ATEK  ATEK  WREST TW [] ; 
:: : TW [A,DL] #  0 ' WIMAGE WIMLEN !!!MB 
        WIMLEN 2- MIN C 1+ SHR SHR SWIM [n,a]
        [E2 0 E2 2 !BN] [a,n_] [E2] 2+ [!TW ] C2 C2 !TB 1+ E2D 0 E2 !TB
        [усt. kol-вo doп. dlia. сloв N i прiznaki в 0] 
        [a,l,N] SWIM 2+ C2 - [a,l,a in wimage] !SB [] ; 
PBIT WRD 
 
[BRD - прoвeрka koda сiмвola na deсяtiчaуyu цiфру] 
:: : BRD  [KOD] #0 -  C 0 11 SEG [N,1/0] ; 
[BRN - прoвeрka koda сiмвola na цiфру в сootвetсtвii с сiсteмoй счiсleaiя] 
:: : BRN  [KOD] PEREV [N] C 0  BASE@ 1-  SEG  [N,1/0] ; 
   : PEREV [kod] C #0 #9 SEG [kod,1/0] BR+ #0 PEREV1 
           [kod,dop] - [n] ; 
     : PEREV1 [kod] C #A #F SEG BR+ 67 PEREV2 [kod,dop] ; 
     : PEREV2 [kod] C #a #f SEG BR+ 127 1000 [kod,dop] ; 
[Нoвый вaрinat - oбрaбatывnat znak '+' i прoбelы do i пoсle 
 - сdelnao dlя фaйloвoгo ввoda] 
:: : T-N [A,L] PUSH EON NERR NERR POP C2 @B C #  = IF+ RPUBRSP 
     BR #- TIN>- #+ TIN- ELSE TIN+ [N] ; 
     : RPUBRSP RP UBRSP ; 
       : UBRSP D 1- E2 1+ E2 C2 @B C #  = EX0 ; 
     : TIN>- TIN- NEG [N] ; 
       : TIN- 1- E2 1+ E2 TIN+ [N] ; 
         : TIN+ [Adr,Dl] 0 E2 DO TIN2 E2D [N] ; 
           : TIN2 C2 @B #  = EX+ BASE@ * C2 1+ E3 @B BRN IF0 NERR + ; 
 
:: TRAP NERR NONUM 
: NONUM ERRLN ." not number!" EXERR ; 
20 1- BYTE VCTR TXTBUF 
[::] : TIN []  0 ' TXTBUF C 20  TIS SP LTIS [A,L] T-N [N] ; 
:: : TIN1 [] ATEK WREST T-N [N] ; 
[::] : CIN [] WRD TIN1 [N] ; 
[::] : CIWD [] WRD ATEK WREST [A,DL] ; 
:: : NINPUT [] S( BASE@ ) B10 CIN [N] ; 
 
 
[B8!] 
: RK [] RL C BR #" TRK #\ SRK ELSE NOP [B] ; 
: TRK [#"] D @ATEK #" = EX0 RL [#"] ; 
: SRK [#\] D RL C BR #\ #\  #n 12  #r 15  #t 11  #f 14  #a 7 
                     #e 33  #" #"  #z 0   #x GETNHEX 
                     ELSE SRKERR E2D [B] ; 
  : SRKERR [b] ERRLN ." na мetaсiмвol в сtрoke!" EXERR [] ; 
  : GETNHEX [] ATEK RL RL DD 2 T-N [b] ; 
 
0 %IF 
: RK [] RL C #" = IF+ TRK [B] ; 
: TRK [#"] D @ATEK #" = EX0 RL [#"] ; 
%FI 
 
: SPUST [] DS ERRLN TSPUST ; 
 
:: TRAP EXERR NOP 
: TTO ENDL TT EXERR ; 
[TT - прiaуditelьaoe пeрeklyuчeaie na ввod с teрмinala] 
: TT '' R4N ! AOVS ; 
:: TRAP HIDE NOP 
:: TRAP RESTOR NOP 
:: TRAP KW NOP 
[***BUR ] 
   TRAP KW! NOP [ doп.сitуaцiя dlя рeakцii na KW пo EON ] 
BYTE VAR KWF    [ фlaг, фikсiруyuщiй, быlo li вozбужdeao KW!, a znaчit i KW ] 
[***endBUR ] 
 
[SIS,LIS - сoхpnnaaie/вoссtnaoвleaie вхodaoй сtpoki вo вспoмoг сteke] 
:: : SIS [] ABEG AKON C2 - [ADR,SKB] 1+ !TM [] ; 
:: : LIS [] ABEG @TM [] ; 
 
:: : LON [A,DL] @R E2  MONON R4P  !R ! ABV [] ; 
 
:: : MONON [DL] @R [***BUR EONW KW NOP ] ON KW KW! 
      S( ABEG AKON ATEK AOVS VVS SKBV ABV [***BUR ] KWF ) !R 
      GTP ! AOVS 
      [DL] ! SKBV ENDL [] ; 
[::] : TEXEC [A,DL] [MONON R4P ! ABV] LON ZMON [] ; 
 : R4P []  SKBV 1- IF- KW 
      ABV @B IF0 KW 
      ABV  12 C2 SKBV SRCHB [A,DL]  C 1+ 
      C !- SKBV 
      !+ ABV 
      ABV 2- @B 15 = - [A,DL] ; 
 
[ПРОВЕРКИ НА НАЛИЧИЕ ПАМЯТИ В СООТВЕТСТВУЮЩЕЙ ОБЛАСТИ] 
:: : CSTSTM [N] C NB + V1 < BR0 FRV1 D [] ; 
:: : DSTSTM [N] D1 + END_MEM < IF0 NOMEM! [] ; 
:: : SDSTSTM [N] YD1 + END_YD1 < IF0 NOMEM! [] ; 
:: : WDSTSTM [N] WD1 + END_WD1 < IF0 NOMEM! [] ; 
:: : NVTSTM [n] NV + BEG_YD1 < IF0 NOMEM! [] ; 
:: : V1TSTM [n] C V1 V1MIN - E2 > BR0 FRV1 D [] ; 
 : FRV1 [n] C 4+ NVTSTM SHDIC [] ; 
 
[::] TRAP NOMEM! _NOMEM! 
     : _NOMEM! ." 
Not enoght memory!" RESTART ; 
 
:: : ADDNV [СК] C NVTSTM !+ NV [] ; 
 
:: : SHDIC [SK] ETXOFF  C ADDNV 
    [SK] V1 E2 [V1,SK] !+ V1 
    [OTK] NV V1 -  V1 !SB  ETXON  [] ; 
:: : SQVOC [] V1MIN V1 - SHDIC [] ; 
[*************************************************************] 
 
:: : MON1 [] 
      [***BUR ] EON KW! !1KWEX 
      UCOLOR ! UCOLOR1 S( UCOLOR )  
      '' CBUFF0 4+ S(LONG) [сoхрnnaaie ссыlki na ; ] 
      5 ! UCOLOR 
      FORGV1   [zdeсь, naвeрaoe, iz-za RESTART в мoмeat MONCOMP1] 
      DEEP   IF- SPUST 
      EON EXERR TTO  
      MONCOMP []  
      FORGV1 
   EON EXERR RESTART 
   UCOLOR1 ! UCOLOR  
   '' CBUFF  EXEC [] ; 
 
[***BUR ] 
   : !1KWEX !1 KWF [ фikсaцiя сitуaцii KW! ] 
            EX     [ И ВЫХОД iz цikla RP MON1 ] ; 
 
:: : LEAVE @R D ; [ Эkсtрenaый ВЫХОД iz tekущeй прoцedуры ] 
 
:: : ZMON [] !0 KWF [KWF=0 znaчit сitуaцii KW na быlo ] 
              RP MON1 
              KWF IF+ LEAVE [ эkсtрenaый ВЫХОД iz ZMON, ] 
                            [ eсli быla вozбужdena сitуaцiя KW ,] 
                   [ Иnaчe, эto oznaчnat, чto быl EX вna цikla ] 
               ." 
'EX' outside loop!" RESTART [] ; 
 
[***endBUR ] 
 
[COPCBUFF - koпiрoвnaie с adрeсa NB1 пo NB сkoмпiliрoвnaaoй 
            koaсtруkцii в CBUFF dlя выпolnaaiя] 
: COPCBUFF [] NB1 NB C2 - '' CBUFF0 !SB [] ; 
 
[CBUFF - oбlaсtь dlя фoрмiрoвnaiя tela вida 
         : ?tmp Proc ;  ili : ?tmp Proc1 Proc2 ; 
         i выпolnaaie takoгo tela вnaшaiм iateрпрetatoрoм] 
CODE CBUFF            [zdeсь выzoв iateрпрetatoрa] 
,INT 
CODE CBUFF0 24 CNSTLEN + BUSE D  [otсyuda naчinnatся telo] 
 
: MONCOMP S( D1 YD1 NB REGIM PTM ) 
   NB->NB1 
   !1 REGIM 
   [,INT]  
   MONCOM1 [] [в NB1 сtaрoe znaчeaie NB] COPCBUFF [] ; 
 
: @PBIT [a] [3+] 35 @BI [Pbit/0] ;
: @UBIT [a] [3+] 34 @BI [Ubit/0] ;

[MONCOM1 - фoрмiрoвnaie tela в CBUFF] 
: MONCOM1 [] 
   0 PUSH  10 CSTSTM 
   BODY 
   !1+ REGIM AVOC  @PBIT   IF+ BODY  [] 
   ECOMP  [] ; 
 
[FORGV1 - вoссtnaoвleaie V1 пoсle выпolnaaiя сloвa iz вхodaoгo пotoka] 
: FORGV1 [] V1 RP CLE3 ! V1 [] ; 
: CLE3 [A] C @L EX0 
   [сtpуktуpa сloвapя koaсtnat: | BA | ... | BA |] 
   C @L [A,BA] 
   INBD EX+  4+  [A+4] ; 
: INBD [BA] NB < [INBD?] ; 
 
: SETTT [] 
  REENT IF- ENDL  TT  COFF  #* ! PRIGL  !0 FWWR 
  REGIM BR+ NB1->NB NB->NB1   -1 ! REGIM 
  ?I 
  ZMON  [] ; 
: NB1->NB NB1 ! NB ; 
: NB->NB1 NB ! NB1 ; 
 
'' SETTT ! AR 
 
LONG VAR NB1 [пeрeмenaaя dlя сoхрnnaaiя na врeмя znaчeaiя NB] 
             [iспolьzуetся в сlучna, koгda nado koмпiliрoвatь 
              dвa сloвa в мoaitoрe i na вtoрoм сloвe пeрezaпусk] 
 
B16 
 
 [ ПРОДУБЛИРУЕМ РЯД ПЕРЕМЕННЫХ koмпoaoвщika i dlя koмпilяtoрa ] 
 
 [  Ряd вaжaых kodoв dlя koмпilяцii: ] 
 [ ( znaчeaiя эtiх kodoв koпiруyutся у naaloгiчaых пeрeмennых koмпonoвщika ) ] 
 NOPCODE    :: VALUE  NOPCODE   [ kod koмandы NOP] 
 INTCODE    :: VALUE  INTCODE   [ kod прoloгa inteпрetiруeмoгo tela] 
 UNDEFCODE  :: VALUE  UNDEFCODE [ kod neoпрedelennoй koмandы ] 
 VARCODE    :: VALUE  VARCODE   [ kod прoloгa tela пeрeмennoй ] 
 CNSTCODE   :: VALUE  CNSTCODE  [ kod прoloгa tela чiсloвoй konсtantы ] 
 TXTCODE    :: VALUE  TXTCODE   [ kod прoloгa tela tekсtoвoй konсtantы ] 
 PTXTCODE   :: VALUE  PTXTCODE  [ kod прoloгa tela пeчati tekсtoвoй konсtantы ] 
[ CALLCODE   :: VALUE  CALLCODE  [ kod koмandы CALL] 
 
 [ Дlinы эtiх kodoв уznnaм iz пeрeмennых koмпonoвщika : ] 
 NOPLEN     :: VALUE NOPLEN   [dlina koмnadы NOP] 
 INTLEN     :: VALUE INTLEN   [dlina прoloгa iateрпрetiруeмoгo tela ] 
 VARLEN     :: VALUE VARLEN   [dlina прoloгa tela пeрeмenaoй ] 
 CNSTLEN    :: VALUE CNSTLEN  [dlina прoloгa tela чiсloвoй koaсtnatы ] 
[ CALLLEN    :: VALUE CALLLEN  [dlina koмnadы CALL] 
[ NEXTLEN    :: VALUE NEXTLEN  [dlina koмnadы NEXT] 
 
:: : ?INT [A] @L INTCODE = [1/0] ; 
 
[????????] 
::  : CALLADR @L ; 
0 %IF 
:: : CALLADR [a] C@L CALLCODE = BR0 T0 CALLA1 [dest/0] ; 
   : CALLA1 [a] 4+ @L [dest] ; 
%FI 
[*************************************************************] 
 
:: : ,INT [] INTCODE ,L [] ; 
   : ,NOP [] NOPCODE ,L [] ; 
[ :: : ,NEXT ; ] 
0 %IF :: : ,CALL [A] CALLCODE ,L [A] ,L [] ; %FI 
:: : ,CNSTBODY [] CNSTCODE ,L [] ; 
:: : ,VARBODY  ['TYPE] VARCODE  ,L ,L [] ; 
   : VARTYPELEN VARLEN 4+ ; [ dlina VARTELO+'TYPE] 
:: : ,TXTCALL [ TXTCODE(''OUT) / PTXTCODE(''.OUT) ] ,L [] ; 
 
[*************************************************************] 
 
 
B8 
CR ."Compiler" 
 
 B8 
[::] LONG VAR AVOC   [adрeс iмeai в сloвaрe] 
 
[::] : AVOCBA AVOC BA ; 
[::] : C!AVOC'BA [az] C ! AVOC 'BA ; 
 
[::] FIX WORD VAR DELHD 1 ! DELHD 
[::] : :: !0 DELHD ; 
 
   : C,L C ,L ; 
:: : ,L [ZN] 4 BUSE !TL [] ; 
:: : ,B [ZN] 1 BUSE !TB [] ; 
:: : ,W [ZN] 2 BUSE !TW [] ; 
[,SB - koпiрoвatь бlok бaйtoв в oбlaсtь tel ] 
:: : ,SB [a,n] DO ,SB1 D [] ; 
   : ,SB1 [a] C @B ,B 1+ [a+1] ; 
 
:: : ,V1 [ZN] 10 V1TSTM [ZN] V1 4- !TL 4 !- V1 [] ; 
:: : ,NV [X] 4 NVTSTM NV !TL 4 !+ NV [] ; 
 
:: FIX ACT VAR ZAGL  '' RESTART ! ZAGL 
[::] : TRAP [] SCOMP '' RAISE_ ,L   BODY  ECOMP  [] ; 
: CMPLTR [] SCOMP    RP BODY   ECOMP [] ; 
: SCOMP  !0 REGIM  0 PUSH [ПР. КОНЦА РАСКРУТКИ] CODE ,INT [] ; 
[::] COPYW CMPLTR : 
'' :  :: VALUE '': [adрeс koмпilяtoрa : ] 
 
: C? REGIM NOT ; 
LONG  VAR VYK  [adрeс всtaвki dlя ,V] 
:: : !V [ADR] ! VYK [] ; 
:: : ,V [ZN] VYK C  NV C2 -   4 ADDNV  4 !+ VYK VYK !SB !TL [] ; 
LONG VAR AT 
:: : CODE [] WRD [] NB ! AT CODW [] ; 
: CODW [] 50 NVTSTM DELHD  FIND  [0/1,AZ,AT] BR0 HDN HDY [0/1,AZ] 
  C'BA AT <!TL 
  [0/1,AZ] DBIT !BI !1 DELHD [] ; 
:: : HDN [*] ETXOFF 
   D NV 4-IS0 !V    WTS 
   '' ZAGL ,V  DO ,V  VYK 4- [НОВОЕ AZ] 
   ETXON ; 
: HEADCNT [] SWIM 2+ @B [сч. dlia. сloв в zaгoloвke] ; 
:: : WTS [] SWIM HEADCNT 1+ DO ZAGON D  HEADCNT 1+ [LWI,..,LW3,N] ; 
 : ZAGON [A] C @L E2 4- [A-4] ; 
 
:: TRAP KNOWED HDD 
: HDY [AZ] C @UBIT BR+ HDU KNOWED [AZ] ; 
: HDD [AZ] CR ."Redefine word " SLOVO HDN [AZ] ; 
: HDU [AZ] NV 4-IS0  4-IS0 C2 -  [AZ,PR] BR+ 
  HDOT [ЗАГ-ВОК В ДР. СЕКЦИИ]  HDTH [ЗАГОЛОВОК В ТЕК. СЕКЦИИ]   [AZ] ; 
: FIXTB [AZ] C 32 [CBIT] !BI1 
   [AZ] C'BA 4- [AZ,ATAB]   DLTAB [AZ,ATAB,DL] DO HDTH1 D [AZ] ; 
: HDOT [AZ] FIXTB [AZ]  HDN [НОВОЕ AZ] ; 
:: : DLTAB [ATAB] C @L NEG SHR SHR 1- [ATAB,DL] ; 
: HDTH [AZ] C 34 [UBIT] !BI0 C'BA C 4- [AZ,'BA,ATB] 
   C @L C PUSH [AZ,'BA,ATB,-/DL/] ADDNV 
   [AZ,'BA,ATB] DLTAB DO HDTH1 [AZ,'BA,AKTBL] 
   NV C2 - E2 [AZ,'BA,DL,AKTBL] !SB [AZ] POP + [NAZ] ; 
: HDTH1 [ATABL] 4- C @L [ссыlka iz taбliцы ] AT <!TL [ATABL-4] ; 
 
:: : ?; [] SWIM @L 0 2 !BN 0 3 !BN [#; SWB #  +]  0 #; 1 !BN # 0 !BN = [1/0] ; 
 
:: : BODY [] 
    EON NEZN NEZN  WRD  
    ?; C? &  EX+ OBR [] ; 
 
[Пoiсk сloвa пo сloвapyu сiсteмы] 
[::] : FIND [] NV FIND0  [a0/az,0/ba] ; 
      : FIND0 [NV]  RP FIND1 [az/a0] CBA [az/a0,ba/0] ; 
       : FIND1 [a0 пeрed zaг. п/с] 4- 4- C @L EX0 [az п/с] ISK [az/a0,ba/0] EX+ 
          [a0] C 4+ @L VOCBODYLEN + [4+] @L @L 
          [a пeрed zaг. пeрв. сloвa в п/с,шkala п/с] 3 @BN E2D 200 &  
          [BR-] [zaмena tak kak teпeрь срaбatывnat пo 7 бitу 3 бaйta шkalы]  
          BR+ FIND2 4-IS0 [a0 пeрed zaг. сled. п/с] ; 
       : FIND2 [a пeрed zaг. пeрв. сloвa в п/с]  4- ISK 
          [az/a0,ba/0] NOT EX0 [naйdeao] 
          [a0 в koaцe п/с] ; 
 
[Пoiсk koaсtnatы пo сloвapyu сiсteмы] 
[::] : CFIND [c] V1 RP CFIND1 [c,a in table] C @L [c,a in table/a0,ba/0] ; 
  : CFIND1 [c,a] C @L EX0 [c,a] C @L CNSTLEN + @L [c,a,c'] C3 = 
      EX+ 4+ [c,a+2] ; 
[DEFINE? - вozврaщnat 0 eсli сled сloвo iz вх пotoka na oпрedeleao, inaчe 1] 
[::] : DEFINE? [] WRD FIND [az/a0,BA/0] BR0 T0 DEF?1 [1/0] ; 
     : DEF?1 [az] C @UBIT E2D NOT [1/0] ; 
 


[OBR - koмпilяцiя oчeрedaoгo сloвa] 
:: : OBR [] FIND [AZ/a0,0/BA]   E2 ! AVOC BR0 CRH BODY1 [] ; 
:: : BODY1 [] AVOCBA  '': = C? & IF+ BDV 
           [] AVOCBA [BA] AVOC C @UBIT  BR+ BODYU D [BA] ,L [] ; 
: BODYU [''ZAGL,AZ] C? IF0 NEZN 'BA 4- 
        [''ZAGL,ATAB] C @L 4- C2 !TL  !V 
        NB ,V [''ZAGL]  [ДОБАВЛЕНИЕ ЭЛЕМЕНТА К ТАБЛИЦЕ] [] ; 
: BDV [AF] ERRLN ." 
not found ;" EXERR [] ; 
:: TRAP WNEZN UNW 
:: : CRH [] SWIM @SNAME BR 
         #" BTEXK  #. BTEX.  ## BTEX#   #- CNH  ELSE CRH1 [] ; 
: CRH1 [] SWIM @SNAME BRD E2D BR+ CNH WNEZN [] ; 
:: : UNW []  C? IF0 NEZN 1 HDN [AZ] CBA [AZ,''ZAGL] E2 [BA,AZ] 
         C'BA !V [BA,AZ] 34 [UBIT] !BI1 
         [BA] NB ,V -10 ,V [ТАБЛИЦА] [BA] ,L [] ; 
: BTEXK [] TXTCODE BTEX1 [] ; 
: BTEX. [] ATEK 1+ @B #" =  BR+ BTEX.K WNEZN [] ; 
: BTEX.K [] RL D PTXTCODE BTEX1 [] ; 
: BTEX1 [aOUT] PUSH  YD1 PUSH 
        RL T0  RP BTEX2 [N]  PUSH [] 
        0 ,YD1B [сtрoki koaчayutся aуleвым бaйtoм, в dlina oa na учitывnatся] 
        '' RSKK NB PUSH ,L [] ; 
: BTEX2 [N] RK ,YD1B 1+ [N+1] ; 
: BTEX# [] ATEK 1+ @B ,C [] ; 
:: : CNH [] TIN1 ,C [] ; 
: ,C [C]  PUSH '' RSKC NB PUSH ,L [] ; 
 
[::] : TEXT [] !0 REGIM 
            CODE WRD SWIM @SNAME BR #" TXK #. TX. ELSE ERRTXT 
            -1 ! REGIM [] ; 
     : TX. [] ATEK 1+ @B #" = BR+ TX.K ERRTXT [] ; 
     : ERRTXT [] ERRLN ." 
No text!"  EXERR [] ; 
     : TXK [] TXTCODE ['' OUT] TXK1 [] ; 
     : TX.K [] RL D PTXTCODE ['' .OUT] TXK2 [] ; 
[doбaвlea TOS dlя okрaсki вывodiмых сooбщeaiй в цвeta UCOLOR, в яdрe убрna EXEC 
 na TrTOS ] 
     : TXK1 [''OUT] ,TXTCALL YD1 ,L RL T0 RP BTEX2 [N] ,L 
            0 ,YD1B [] ; 
     : TXK2 [''.OUT] ,TXTCALL YD1 ,L RL T0  RP BTEX2 [N] ,L '' TOS ,L ''; ,L 
            0 ,YD1B [] ; 
 
:: : ECOMP [] ''; ,L RP RSK1 D -1 ! REGIM [] ; 
: RSK1 [] POP C EX0  [AU] C@L EXEC [] ; 
 
: RSKK [AU] NB <!TL [] 
       POP POP POP ,TXTCALL [zaпiсna выzoв OUT ili .OUT] 
       [n,YD1] ,L [zaпiсna YD1] 
       [n]  ,L [zaпiсnaa dlina tekсtoвoй koaсtnatы] [] ; 
: RSKC [AU]  POP [AU,C] C-BA [AU,ATC] [WCS_] <!TL [] ; 
:: : C-BA [C] CFIND BR0 NWCNST ATCNST [ATC] ; 
: NWCNST [C,ADR] D NB ,V1 [C] NWCNST1 [ATC] ; 
[NWCNST1 - фoрмiрoвnaie tela koaсtnatы бez сloвaрaoгo вхoda] 
: NWCNST1 [C] NB E2 ,CNSTBODY [ATC,C] ,L [ATC] ; 
: ATCNST [C,ADR] E2D [ADR] @L [ATC] ; 
 
[::] : VALUE [c] NWCNST1 [AT] HEAD [] ; 
 
:: : BUSE [B] C CSTSTM NB E2 !+ NB [NB] ; 
 
:: : HEAD [BA] ! AT WRD CODW [] ; 

1 %IF 
[::] : COPYW [] WRD ON KNOWED HDN FIND BR0 NEZN1 COPW1 [] ; 
     : NEZN1 [A0] D NEZ WRD [] ; 
     : COPW1 [AZ1] [. CR] C @UBIT [. CR] [AZ1,U-bit] BR+ NEZN1 COPW2 [] ; 
     : COPW2 [AZ1] CODE FIND D [. CR] [AZ1,AZ2] 
       E2 CBA [.. CR] [AZ2,AZ1,BA1] C3 'BA [.. CR] !TL @L [AZ2,@AZ1]  
       [40 SWB SWW] [. CR] 3 @BN 40 & E2D 0 E2 3 !BN C2 [.. CR] @L [.. CR] &0 [.. CR] <!TL  [] ; 
%FI

0 %IF

[::] : COPYW [] WRD ON KNOWED HDN FIND BR0 NEZN1 COPW1 [] ;
     : NEZN1 [A0] D NEZ WRD [] ;
     : COPW1 [AZ1] [. CR] C@UB [. CR] [AZ1,U-bit] BR+ NEZN1 COPW2 [] ;
     : COPW2 [AZ1] CODE FIND D [. CR] [AZ1,AZ2]
       E2 CBA [.. CR] [AZ2,AZ1,BA1] C3 'BA [.. CR] !TL @L [AZ2,@AZ1] [. CR]
       40 SWB SWW [. CR] & C2 [.. CR] @L [.. CR] &0 [.. CR] <!TL  [] ;

%FI 
 
 
 
B16 
 
[пodрaвaiвnaм na чetaуyu грnaiцу] 
: ADR2 [ADR] 1+ 1 INV & [ADR2] ; 
 
[пodрaвaiвnaм na грnaiцу kрataуyu 4] 
: ADR4 [ADR]  3+ 3 INV & [ADR4] ; 
 
[: ADR1 ."1!" ; 
: ADR0 ."0!" ;] 
 
 
[Выбoр пodрaвaiвnaiя] 
: DLBR [ADR,DL]  4 2 DO DIVS E2D  BR 1 NOP 2 ADR2 4 ADR4 ELSE NOP [ADR_] ; 
   : DIVS [DL,N] C2 C2 / E2D EX0 SHR ; 
 
 
FIX LONG VAR TEMP 
[Выdeleaie n-гo бaйta в dв. сloвe l] 
: @BN [l,n_byte] C2 ! TEMP ' TEMP + @B [l,b] ; 
 
[Зaпiсь n-гo бaйta в dв. сloвo l] 
: !BN [l,a,n_byte] C3 ! TEMP ' TEMP + [l,a,adr] !TB TEMP E2D [l_] ; 
 
B8 
 
FIX WORD VAR FIX? 0 ! FIX? 
FIX WORD VAR OWN? 0 ! OWN? 
: FIX [] !1 FIX? [] ; 
: OWN [] !1 OWN? [] ; 
:: : MUSE [DL] OWN? BR0 NOWN YOWN [a] ; 
   : NOWN [DL] FIX? BR0 NFIX YFIX [a] ; 
   : YFIX [DL] YD1    C2 DLBR C ! YD1   E2 C SDSTSTM !+ YD1 !0 FIX? [a] ; 
   : NFIX [DL] D1     C2 DLBR C ! D1    E2 C DSTSTM !+ D1 [a] ; 
   : YOWN [DL] WD1    C2 DLBR C ! WD1   E2 C WDSTSTM !+ WD1 !0 OWN? !0 FIX? [a] ; 
 
: ,D1L [L] 4 DSTSTM D1 !TL 4 !+ D1 [] ; 
: ,D1H [W] 2 DSTSTM D1 !TW 2 !+ D1 [] ; 
: ,D1B [B] 1 DSTSTM D1 !TB !1+ D1 [] ; 
: ,YD1B [B] 1 SDSTSTM YD1 !TB  !1+ YD1 [] ; 
 
 
WORD VAR YVIF 
[::] : %IF [N] IF0 IF1 [] ; 
: IF1 [] !1 YVIF  RP WIF [] ; 
: WIF []  WRD FIND '' %IF  =  !+ YVIF 
          [AZ]  BA '' %FI  =  !- YVIF  YVIF EX0 [] ; 
: %FI ;
