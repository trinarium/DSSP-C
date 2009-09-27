 
B10 
CR ." Data structure DSSP/c i80386" CR 
 
 
[*********  УСТАНОВКА ПЕРЕМЕННЫХ ДЛЯ КОМПОНОВЩИКА  **********] 
'' LVAR@ __ ''LVAR@ 
'' WVAR@ __ ''WVAR@ 
'' BVAR@ __ ''BVAR@ 
'' AVAR@ __ ''AVAR@ 
'' IVAR@ __ ''IVAR@  
'' SVAR@ __ ''SVAR@  
'' LVCT@ __ ''LVCT@ 
'' WVCT@ __ ''WVCT@ 
'' BVCT@ __ ''BVCT@ 
'' AVCT@ __ ''AVCT@ 
'' GETADR __ ''GETADR 
'' GETADR_OWN __ ''GETADR_OWN 
 
'' OWN_BASE ABSOLUT :: LONG VAR OWNBASE [базовый адрес собственных данных 
                                         текущего процесса] 
MAIN_OWN ! OWNBASE
 
[EMPTY - УКАЗАНИЕ ОПРЕДЕЛИТЬ ШАБЛОН, НЕ ОТВОДЯ ПАМЯТИ] 
FIX WORD VAR TDUMMY 0 ! TDUMMY 
[::] : EMPTY !1 TDUMMY ; 
 
[***************** ВНЕКОМАНДНАЯ РАБОТА С ДАННЫМИ ****************] 
 
[EQU - настройка данного на адрес] 
[::] : EQU [addr] GTP [addr,''data] EQU' [] ;  PBIT EQU 
 
[::] : EQU' [addr,''data] VARTYPELEN + !TL [] ; 

 
[ADDROF - получение начального адреса области памяти для данного] 
[::] : ADDROF [] GTP [''data] ADDROF' [adata] ;   PBIT ADDROF 
[::] : ADDROF' [''data] C 4 - @L  
               BR ''GETADR_OWN OWNBASE [собств. данные]  
                  ''GETADR     0   [обычные данные]                  
                  ''GETADR_REL E2  [относит. данные] [base=вершина стека]  
               ELSE 0    
               E2 [base,''data] VARTYPELEN + @L + [adata] ; 


 
[*************  ОПРЕДЕЛЕНИЕ ОСНОВНЫХ ФОРМАТОВ *****************] 
 
FIX LONG VAR DTYPE [переменная для хранения типа определяемых данных ] 
                    [ В ней хранится указатель на дескриптор типа. ] 
[ Тип, как правило, подразумевает конкретный формат и информационную емкость.] 
 : LDATA 0 DTYPE C !TYPE 'Size? + @L EXEC ;  
 [ информац.емкость одной перем типа tDTYPE в байтах] 
 
 [ Смещения в дескрипторе типа для некоторых префиксных операций ] 
   -8 4 * VALUE 'Dim?   [  кол-во составляющих элементов ] 
   -9 4 * VALUE 'Size?  [  размер одного элемента ] 
 
[::] : BYTE   [] '' BVAR@ ! DTYPE [] ; 
[::] : WORD   [] '' WVAR@ ! DTYPE [] ; 
[::] : LONG   [] '' LVAR@ ! DTYPE [] ; 
[::] : ACT    [] '' AVAR@ ! DTYPE [] ; 
[::] : SBYTE  [] '' SVAR@ ! DTYPE [] ; 
[::] : SWORD  [] '' IVAR@ ! DTYPE [] ; 
 
[*** СИСТЕМНЫЕ ПЕРЕМЕННЫЕ, управляющие ОПРЕДЕЛЕНИЕМ и размещением ДАННЫХ ***] 


 
FIX WORD VAR REL? [ переменная-флажок относительной адресации для данных ] 
                  [ =1, когда требуется формировать относит. данные ] 
                  [ =0, для формирования обычных данных ] 
FIX LONG VAR RELCNT [ переменная-счетчик смещения относит. данных ] 0 ! RELCNT 
 
[***** DSRESET - инициализация данных при запуске системы *****] 
: DSRESET [] DSINIT !0 REL? !0 RELCNT ; 
[ DSINIT - инициализация данных после очередного объявления ] 
: DSINIT [] !0 TDUMMY '' WVAR@ ! DTYPE !0 FIX? !0 OWN? ; 
 
 [***** ОПЕРАЦИИ ВЫЧИСЛЕНИЯ АДРЕСА ПЕРЕМЕННОЙ *****] 
 '' GETADR     VALUE ''GETADR     [операция вычисления норм.адреса] 
 '' GETADR_OWN VALUE ''GETADR_OWN [вычисления собств.адреса] 
 '' GETADR_REL VALUE ''GETADR_REL [вычисления относит.адреса] 
  
[ ADRMODE - назначение режима (процедуры) вычисления адреса данного ]  
: ADRMODE [] REL? BR+ ''GETADR_REL oldADRMODE [AdrProc] ; 
 : oldADRMODE OWN? BR+ ''GETADR_OWN ''GETADR [AdrProc] ;  
 
[ LOCATE - размещение блока данных в памяти и получение его адреса ] 
: LOCATE [nb] REL? BR+ relLOCATE oldLOCATE [adr] ; 
  : oldLOCATE TDUMMY BR+ T0 MUSE [adr] ; [ для норм. и собств. данных ] 
  : relLOCATE [nb] RELCNT E2 !+ RELCNT [ offset] ; [ для относит.данных ] 
   
        [--- Переменные - указатели Дескрипторов  ---]      
 FIX LONG VAR TVCTR [ указатель дескриптора вектора ] '' VCTR@  !  TVCTR 
 FIX LONG VAR TARR  [ указатель дескриптора массива ] '' ARR@   !  TARR 
 FIX LONG VAR TSTRING  [ указатель дескриптора string ] '' BSTR@   !  TSTRING 
   

[*********------- Компилирующие команды  VAR  VCTR  ARR  --------********] 
 
[::] : VAR [] DTYPE [TypePtr] TstDTYPE BR0 ERDTYPE VARF1 [] ; 
     : ERDTYPE [] CR ."Uncorrect type of data!" WRD SLOVO DSINIT EXERR [] ; 
     : VARF1 [] ADRMODE [AdrProc] ,L CODE DTYPE [TypePtr] ,VARBODY  
                 LDATA [len] LOCATE [a] ,L DSINIT [] ; 
     : TstDTYPE [TypePtr] [test, is it scalar type (1) or not (0) ?] 
                 'Dim? + @L '' T1 =  [ (TypePtr^.opDim= '' T1 ) ] ; 

 
[::] : VCTR [n] DTYPE [n,TypePtr] TstDTYPE BR0 ERDTYPE VCTRF1 [] ; 
     : VCTRF1 [n] ADRMODE [AdrProc] ,L CODE  
              TVCTR [n,VctrType] ,VARBODY  
              [n] 1+ C LDATA * [n+1,lb] 
              LOCATE [n+1,a] ,L ,L  
              DTYPE [TypeItem] ,L LDATA [SizeI] ,L DSINIT [] ; 
 
[::] : ARR [...,k] DTYPE [k,TypePtr] TstDTYPE BR0 ERDTYPE ARRF1 [] ; 
     : ARRF1 [i1,...,ik,k] ADRMODE [AdrProc] ,L CODE  
          TARR [...,k,ArrType] ,VARBODY [i1,...,ik,k] 
          [ calculate n = number of items (count) ] 
          4 1 C3 DO ARRF2  E2D [i1,...,ik,k,n] C LDATA * [i1,...,ik,k,n,lb] 
          LOCATE [i1,...,ik,k,n,a] ,L [address] ,L [count] 
          DTYPE [TypeItem] ,L LDATA [SizeI] ,L [size of one item] 
          [i1,...,ik,k] C ,L DO INC, [] DSINIT [] ; 
     : ARRF2 [i1,...,ik,k,j,n] C2 CT 1+ [i1,...,ik,k,j,n,i(k-j-4)+1] 
             * E2 1+ E2 [i1,...,ik,k,j+1,n'] ; 
     : INC, [x] 1+ ,L [] ; 

 
1 %IF 
B16 
[::] : STRING [n] TSTRING STRF1 [] ; 
     : STRF1 [n,''xVCT@] ADRMODE [AdrProc] ,L CODE ,VARBODY [n] 1+ C 4+ [n,lb]  
       3+ 3 INV &  LOCATE [n,a] ,L ,L  DSINIT [] ; 
B10 
%FI 
 
 
 
[*******------- Компилирующие команды  STRUCT:     ;STRUCT ------********] 
 
 FIX LONG VAR TSTRUCT [ указатель дескриптора структуры-образца ] 
   '' STRUCT@ ! TSTRUCT 
 13  VALUE TSTRUCTlen [ длина операционной части дескриптора ] 
 FIX LONG VAR TSTRUCTNAME [ указатель тела имени типа формируемой структуры ]  
 02 4 * VALUE .TSTRUCTptr [ смещение в нем для ссылки на дескриптор типа ] 
 
 
[***** НАЧАТЬ ОБЪЯВЛЕНИЕ СТРУКТУРЫ ****** ] 
[::] : STRUCT: CODE [ далее во вх.потоке имя типа структуры STRUCTNAME ] 
         NB ! TSTRUCTNAME [ запомним адрес тела имени формируемой структуры ] 
     [ формируем тело для имени типа  : STRUCTNAME '' 'StructDesc ! DTYPE ; ] 
        ,INT  '' '' ,L  
        TSTRUCT ,L [ временная ссылка на дескриптор типа структуры-образца ]  
        '' ! ,L '' DTYPE ,L  ''; ,L []  
    [ Далее осуществляется настройка для объявления относительных данных ] 
    [ все последующие переменные и массивы будут полями новой структуры ] 
    [ пока не встретится слово ;SRUCT , завершающее объявление структуры] 
       !0 RELCNT !1 REL? ; 
        
[***** ЗАКОНЧИТЬ ОБЪЯВЛЕНИЕ СТРУКТУРЫ ****** ] 
[::] : ;STRUCT 
       [--- Формируем Дескриптор типа новой Структуры ---]      
  TSTRUCT TSTRUCTlen 1- 4 *  - TSTRUCTlen 4 * [AdrStruct,len (13*4)]  
  [ копируем операторную часть дескриптора структуры ] ,SB [ ] 
  NB 4- ['StructDesc]  RELCNT ,L [block size] 
  !0 REL? !0 RELCNT [ закончим режим объявления относит.данных ] 
  [ настроим указ-ль на дескриптор в теле имени структуры ] 
  ['StructDesc] TSTRUCTNAME .TSTRUCTptr + !TL    [ ] ; 
  
[************************************* конец определений для СТРУКТУР ******] 
 
 [ SIZEVALUE Определяет константу, равной длине переменной текущего типа ] 
[::] : SIZEVALUE LDATA VALUE [NAME] DSINIT ;  
      
OWN BYTE VAR  SYMBOL0   [для преобразования байта в строку] 
[::] : B-S [b] ! SYMBOL0 ' SYMBOL0 1 [a,1] ; 
 
[******************  СИТУАЦИЯ "ИНДЕКС?" ***********************] 
 
[::] TRAP NOIND NOINDEX 
: NOINDEX [*i] ." 
Index?" . RESTART [] ; 
 
 
'' NOIND '' ANOIND VA-FA !TL 
 
[::] TRAP ERSTR ERRSTRING 
: ERRSTRING [*i] ." 
Index > current length"  . RESTART [] ; 

 
'' ERSTR '' AERSTR VA-FA !TL 
 
 
 
[******************  ОПИСАНИЕ КОНСТАНТЫ *********************] 
WORD VAR CNSTCTR [счетчик элементов для CNST] 
 
[CNST - ФОРМИРУЕТ ВЕКТОР С ЗНАЧЕНИЯМИ, ЯВНО УКАЗАННЫМИ ЗА ИМЕНЕМ ВЕКТОРА 
        ДО ; С СООТВЕТСТВУЮЩИМ КОЛИЧЕСТВОМ ЭЛЕМЕНТОВ. КАЖДЫЙ ЭЛЕМЕНТ 
        ДОЛЖЕН ОПРЕДЕЛЯТЬСЯ ОДНИМ СЛОВОМ] 
 
[::] : CNST [] !0 OWN? LDATA YD1 E2 DLBR C ! YD1 
     NB [начальный адрес размещения элементов вектора констант 
             и адрес тела будущей константы] 
     WRD 0 ' WIMAGE WIMLEN !TM [УПРЯТАЛИ ИМЯ КОНСТАНТЫ] 
     !0 CNSTCTR 
     RP MONINC [YD1,NB] [ПОСЛЕДОВАТЕЛЬНОЕ ВЫПОЛНЕНИЕ СЛОВ ИЗ ВХ ПОТОКА 
                     С ПОДСЧЕТОМ ИХ ЧИСЛА ДО ; и записью в сохр. данные] 
     0 ' WIMAGE @TM [ВОССТАНОВИЛИ ИМЯ КОНСТ] 
     <WRD 
     CNSTCTR 1- EMPTY VCTR  [СФОРМИРОВАЛИ ВЕКТОР] 
     [YD1,NB] 4+ EQU' [установили адрес данных] [] ; 
 
 
[ВЫПОЛНЕНИЕ ОДНОГО СЛОВА, ВЫХОД ЕСЛИ ;, УВЕЛИЧЕНИЕ СЧЕТЧИКА ЭЛЕМЕНТОВ 
 и запись элемента в память] 
: MONINC [] WRD [] ?; EX+ [] <WRD 
  MON1 [x] FIX LDATA BR 1 MB1 2 MB2 ELSE MB4 !1+ CNSTCTR [] ; 
  : MB1 [x] 1 MUSE [x,a] !TB [] ; 
  : MB2 [x] 2 MUSE [x,a] !TW [] ; 
  : MB4 [x] 4 MUSE [x,a] !TL [] ; 
 
B16 
[*************  S( ******************] 
[!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!] 
[Возможно не будет работать на Sun] 
[!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!] 
1 %IF 
[::] : S( [] @R 0 [A,QW=число слов] 
             S(1  [A,QW] C 14 > IF+ S(mist 
             0C * 4+ 0FFFF8200 &0 !R 0 !R '' LVOST !R [A] 4+ !R [] ; 
 
[S(1 - рекурсивное, так как нельзя использовать циклы - 
       они помещают нечто в стек возвратов, помещение 
       адресов и значений в стек возвратов] 
: S(1 [A,QW] @R E3 E2 [A1,A,QW] C2 @L 
      [A1,A,QW,''nextword] 
      C ''; = IF+ S(mist [A1,A,QW,''nextword] 
      C '' ) = BR0 S(2 D [A1,A,QW] 
      E2 E3 [A,QW,A1] !R [A,QW] ; 
[S(2 - помещение в стек возвратов информации о данном базового формата] 
: S(2 [A1,A,QW,''nextword] @R [A1,A,QW,''nw,A2] E4 E3 E2 
      [A1,A2,A,QW,''nextword] 
      C 
      ADDROF' [A1,A2,A,QW,''nextword,adrof_data] 
      C2 
      SIZE?' [A1,A2,A,QW,''nextword,adata,size] 
      C3 
      DIM?'  [A1,A2,A,QW,''nextword,adata,size,dim] 
      C 1 = IF0 S(mist 
      * C2 C2 [A1,A2,A,QW,''nextword,adata,size*dim,adata,s*d] 
      BR 1 @B 
         2 @W 
         4 @L 
      ELSE S(mist 
      [здесь помещение в стек возвратов] 
      [A1,A2,A,QW,''nextword,adata,size*dim,val] 
      !R [значение в стек возвратов] 
      [A1,A2,A,QW,''nextword,adata,size*dim] 
 
      E2 !R [полный DSSP-адрес в стек возвратов] 
      [A1,A2,A,QW,''data,s*d]  !R D [A1,A2,A,QW] 
      [A1,A2,A,QW] E2 4+ E2 1+ [на след слово] 
      S(1  [рекурсивный вызов] 
      E2 E3 !R [A1,A,QW] ; 
 
: S(mist ." 
Error S(!" RESTART ; 
 
: ) ; 
%FI 
 
[S(LONG) - восстановить значение 4-х байтов по заданному адресу 
           по окончанию работы процедуры] 
1 %IF 
: S(LONG) [A] @R E2 [@R,A] 
              C @L !R [зап. значения]   !R [зап. адреса] 
              4 !R 0FFFF8210 !R 0 !R '' LVOST !R [@R] 
              !R [] ; 
    %FI 


