[***BUR 03.08.99 Механизм сопрограмм. II-часть (на ДССП) ***]
CR ." Дополнение к Механизму сопрограмм "
[ PROGRAM $CONTEXTS CR ."$CONTEXTS - Механизм сопрограмм" ]
[ USE SYSTEM ]
[ BASE@ ] B10

   [---Комп. слово для создания переменной типа CONTEXT ---]
:: : CONTEXTLEN 40 ;  [ длина блока-контекста в байтах ]
 [   : SCOMP_  !0 REGIM  0 PUSH CODE ,INT ; ]
:: : DATABLCK [Len] SCOMP '' '' ,L [Len] MUSE [a] ,L ECOMP  [] ;
:: : CONTEXT CONTEXTLEN DATABLCK ;

   LONG VAR CntxtPtr [ содержит ук-ль текущего контекста ]          

:: : RESUME [CB] CntxtPtr E2 C ! CntxtPtr [CntxtPtr:=CB]   
            [OldCntxtPtr,CB] TRANSFER D ;
:: : FORCE  [PROC,CA] C2 ?INT BR+ FORCE! DD [] ;
   : FORCE! [PROC,CA] C CntxtPtr = BR+ DEXEC FORCECONTEXT [] ;
   : DEXEC [PROC, CA] D EXEC [] ;

:: : INITCONTEXTS [CMain]  [ Инициализация сопрограммного механизма ]
	[CMain-Указатель, где будет блок-контекст главного процесса ]
        C MAINCONTEXT [ Установка контекста основного процесса ] 
        [CMain] ! CntxtPtr [ и фиксация его как текущего ]
   ;
[ ! BASE@ ]
[ CLEAR $CONTEXTS ]
[ UNDEF ]
