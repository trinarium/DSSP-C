[*** INTERRUPT MEHANISM  DSSP. DSP-part. last modification 23.08.1999 ***]
[*** МЕХАНИЗМ ПРЕРЫВАНИЙ ДССП. DSP-часть. посл.модификация 23.08.1999 ***]

CR
." Дополнение Механизма прерываний ДССП (для ДССП/C Win95 i80386 )"

[*******---------- Команды УПРАВЛЕНИЯ РЕЖИМОМ ----------------------*******]
[-------    ОТКРЫТИЯ/ЗАКРЫТИЯ обработки ПРЕРЫВАНИЯ в ДССП           -------] 

OWN LONG VAR INTRCNT  
     [ Счетчик уровней "открытости" режима обработки прерывания]    
     [ Если он > 0, то в ДССП вызываются процедуры обработки прерывания  ]
     [ Если он <=0, то ДССП-процедуры обработки прерываний НЕ вызываются,]
     [ НО прерывания фиксируются (не пропадают), и могут быть обработаны ]
     [ после очередной команды разрешения обработки прерываний в ДССП    ]
 1 ! INTRCNT [ Надо для Корректной Инициализации , т.к. ]
             [ при запуске ДССП IntrFlag будет открыт ! ]

[ Узнать состояние уровня "открытости" режима обработки прерывания в ДССП ]    
:: : INTR? [ ] INTRCNT [INTRCNT] ;
[ Закрыть режим обработки прерываний в ДССП : ]
:: : INTROFF xINTROFF !0 INTRCNT ; 
[ Открыть режим обработки прерываний в ДССП : ]
:: : INTRON xINTROFF [(*!*)]  !1 INTRCNT xINTRON ; 
 [ ЗАМЕЧАНИЕ: Действия (*!*) надо ставить в начале каждой такой команды, 
              чтобы саму эту команду делать в закрытом режиме ]

[ Повысить уровень "открытости" режима обработки прерываний в ДССП : ]
:: : INTR+ xINTROFF [(*!*)]  !1+ INTRCNT INTRCNT IF+ xINTRON ; 
[ Понизить уровень "открытости" режима обработки прерываний в ДССП : ]
:: : INTR- xINTROFF [(*!*)]  !1- INTRCNT INTRCNT IF+ xINTRON ; 

[ Закрыть режим обработки прерываний в ДССП, сохранив в стеке 
  прежний уровень "открытости" режима обработки прерываний в ДССП : ]
:: : INTR?OFF [ ] xINTROFF INTRCNT !0 INTRCNT [прежний INTRCNT] ; 
 [ Восстановить из стека прежний режим обработки прерываний в ДССП : ]
:: : INTR! [INTRCNT] xINTROFF ! INTRCNT INTRCNT IF+ xINTRON [ ] ; 

[*******------------------------------------------------------------*******]

[************************************************************************]
[          СРЕДСТВА ДЛЯ ПОДВЕШИВАНИЯ ДССП-ПРОГРАММ К ПРЕРЫВАНИЯМ         ]
[************************************************************************]


:: : INT [] ;

[ Привязка процедуры обработки прерывания ]
:: : LINK [nV]  GTP [ PRTELOLEN - ] 
          SET_LINK [ oldDsspIV,oldSysIVLo,oldSysIVHi ]
                   [ Segment:Offset, Offset, Selector] ;
     PBIT LINK

  : E3-2 [x1,x2,y1,y2,y3]  
        E3 5 ET [y1,x2,y3,y2,x1] E2 E4       
        [y1,y2,y3,x1,x2] ;
: SET_LINK [nV, newDsspIntVect]  E2  [newDsspIV, nV]
       C GetIntVect [ newDsspIV,nV,  oldDsspIV,oldSysIVLo,oldSysIVHi ] 
       E3-2         [ oldDsspIV,oldSysIVLo,oldSysIVHi,  newDsspIV,nV ]
       SetIntVect   [ oldDsspIV,oldSysIVLo,oldSysIVHi ]         ;

:: : RESTORE_LINK [ oldDsspIV,oldSysIVLo,oldSysIVHi, nV] RestoreIntVect [] ;

0 %IF

B16
: INT []
        200 C MUSE
        0E8 ,B  '' IntHndl  NB  4+  -  ,L
        [200, A]  C ,L + ,L   [] ;

: LINK [INT]  GTP  0D  -  [INT, A_PR]
       SET_LINK [Segment:Offset, Offset, Selector]
; PBIT LINK

: PRTELOLEN 07 [ 7 ] ; [ длина пролога прерывания ]

[ Префикс процедуры обработки прерывания ]

:: : INT []  '' INTRPROC PRTELOLEN - PRTELOLEN ,SB [] ;

: SET_LINK [INT, A_PR]  E2  [A_PR, INT]
       GET_INT_NUM [A_PR, IV]
       C GetRMIntVect [A_PR, IV, Segment:Offset]
       E3  E2 [Segment:Offset, A_PR, IV]
       C GetPMIntVect [Segment:Offset, A_PR, IV, Offset, Selector]
       E3  E2 [Segment:Offset, A_PR, Selector, IV, Offset]
       E4  E2 [Segment:Offset, Offset, Selector, A_PR, IV]
       000F  [Code segment selector]
       E2    [Segment:Offset, Offset, Selector, A_PR, Seg, IV]
       SetAPMIntVect [Segment:Offset, Offset, Selector] ;

B10

: GET_INT_NUM [IV]  16 C2 - IF+ INT_NUM<16 [IV'] ;
: INT_NUM<16  [IV]  8  C2 - BR+ INT_NUM<8 INT_NUM>=8 [IV'] ;
: INT_NUM<8   [IV]  GetHWIVBase [IV, IV1, IV2]  D +  [IV'] ;
: INT_NUM>=8  [IV]  GetHWIVBase [IV, IV1, IV2]  E2  D  E2 8 -  + [IV'] ;

B16

: RESTORE_LINK [Segment:Offset, Offset, Selector, IV]
       GET_INT_NUM  C4  C2
       IOFF SetRMIntVect  SetPMIntVect  ION [Segment:Offset]  D  []
;

%FI


