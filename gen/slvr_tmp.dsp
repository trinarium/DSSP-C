[Временно написанные на ДССП процедуры поиска по словарю] 
[И некоторые процедуры из словаря SYSTEM] 
B16 
: 'BA [az] C @L C BR0 D 'BA1 ['ba/a0] ; 
   : 'BA1 [az,@az] [SWW 7 & SHL SHL] 2 @BN 7 & SHL SHL E2D [az,nbyte]  
                   4+ - ['ba] ; 
: CBA [az] C 'BA @L [az,ba] ; 
: C'BA [az] C 'BA [az,'ba] ; 
: BA [az] 'BA @L [ba] ; 
 
: IS0 [a] RP IS01 [a0] ; 
   : IS01 [a] C @L EX0 4- [a-4] ; 
 
: 4-IS0 [a] 4- IS0 [a0] ; 

0 %IF 
: ISK [az] RP ISRP [az/a0] CBA [az/a0,ba/0] ; 
%FI

  : ISRP [az] C @L EX0 
         [az] C @L 3 @BN E2D 80 & [BR-] BR+ ISRPT ISRPZ [az next] ; 
  : ISRPT [atbl] C @L  + [az next]   ; 
  : ISRPZ [az] [."iz" CR] C @L  [az,@az] 3 @BN 4 & [az,@az,a] 3 !BN [az,@az_] 
               2 @BN 7 & 2 !BN	[407FFFF] [Cbit+NNN+2посл.буквы] 
         [&] SWIM @L = BR0 ISRPN ISRPY [az next]  ; 
  [не совпало] 
  : ISRPN [az] [."in" CR] 'BA 4- [az next] [."iin" CR] ; 
  [первое слово совпало, проверяем дальше] 
  : ISRPY [az]   C 4- SWIM 4- C3 @L [SWW  7 &] 2 @BN 7 & E2D [az,az-4,swim-4,n] 
           DO ISRP1 [сюда попадаем если совпали заголовки] 
          [az,a1,a2] D EX+ [т.к. a1>0] [az/az next]   [."iiy" CR] ; 
  : ISRP1 [az,a1,a2]  C2 @L C2 @L = IF0 ISRP1N 4- E2 4- [az,a2-4,a1-4] ; 
  : ISRP1N [az,a1,a2] DD 'BA 4- 0 C EX [из ISRP1] [az next,0,0] ; 
