CR ." Control file of configuration DSSP/c" CR
CR ." Configuration begin "

B10
[Паpаметpы компоновки]
900000 __ END_MEM     [верхняя граница памяти DSSP386]
470000 __ BEG_D1      [адрес начала данных DSSP386]
469990 __ END_PTM     [конец временного стека]
460000 __ BEG_PTM     [начало временного стека]
459980 __ CSB         [начало стека возвратов]
440000 __ OPSB        [начало стека операндов]
370000 __ MAIN_OWN    [начало собственных данных]
     0 __ BEG_WD1     [адрес начала собственных данных]
439996 370000  -  [69996 ] __ END_WD1 [адрес конца собственных данных]
[439996 __ END_WD1]     [адрес конца собственных данных]
[370000 __ BEG_WD1]     [адрес начала собственных данных]
369996 __ END_YD1     [адрес конца сохраняемых данных]
300000 __ BEG_YD1     [адрес начала сохраняемых данных]
     5 __ NOMVER      [номер версии DSSP386]
    01 __ NOMRED      [номер редакции DSSP386]
    01 __ PLATFORM    [Hомеp платоpмы генеpации:
                       1-IBM PC 2-SUN 3-LINUX 4-R3000]

[ USE$DF]
CR ." Configure system DSSP/c"  CR

[Компоновка системы]
InitDSSP  [ загружается ядро = Заголовок + Тела + Таблица слов ]

LOAD pusk0.dsp
LOAD data.dsp
LOAD pusk1.dsp

[ LOAD INT_LINK ]
LOAD inters.dsp
LOAD dir.dsp
LOAD channels.dsp
LOAD slvr_tmp.dsp
LOAD cmpltr.dsp
[ LOAD TSTARR ]
[ LOAD BITSET ]
[ LOAD ARRAYS ]
LOAD terminal.dsp
LOAD disk.dsp
LOAD vocs.dsp
[LOAD NEW@@]
CR ."-----------" CR

LOAD mdt.dsp
[ LOAD DISP ]
LOAD CONTEXTS.DSP
LOAD INTERUPT.DSP 
[LOAD OTL1]
[LOAD QND]

LNK
TYPEPTR
NEOPR
FQND

CR ." Configuration end "
CR ." Save system in file DSSP.dpf" CR
SVS dssp.dpf

[%FI]
CR ."Load DSSP DSSP.dpf and execute LOAD PRIME,
debuger and editor load if necessary,
execute command LOAD OTL and LOAD E." CR
