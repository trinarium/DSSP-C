PROGRAM $WINAPI
[************************************************************************]
[*****            INTERFACE: DSSP -- WIN32 API                 **********]
[*****   started 19.08.1998         last modification  20.08.1998   *****]
[************************************************************************]

B10  USE SYSTEM CR ."WINAPI - Interface: DSSP --- Win32API"

 [*********************************************************************]
 [***** SOME USEFUL OPERATIONS FOR INTERFACE: DSSP -- WIN32 API *******]
 [*********************************************************************]

 [*** ПРЕОБРАЗОВАНИЕ СТРОКИ с длиной в строку с признаком конца ***]
    [ поставим код нуля вместо последнего символа ]
:: : \0 [S,Len] 1- C2 + 0 <!TB [S] ;

 [*** Команда ВЫЗОВА WIN32API функции с контролем ее исполнения ****]
 [ в случае неудачи возбуждается исключительная ситуация WIN32FAIL! ]
    :   WIN32FAIL!_REAC ." Сбой при вызове WIN32API ! " RESTART ;
  TRAP WIN32FAIL!  WIN32FAIL!_REAC
:: : WIN32API [P1,..,Pk,k,Name,Len] \0 WIN32API_ IF- WIN32FAIL! [Res] ;

 [*** Команда резервирования области для формирования WND PROCEDURE ***]
   : SCOMP_  !0 REGIM  0 PUSH CODE ,INT ;
:: : DATABLCK [Len] SCOMP_ '' '' ,L [Len] MUSE [a] ,L ECOMP  [] ;
   14 VALUE WNDPROCAREAlen [ длина блока-области формирования WND PROC ]
:: : WNDPROCAREA WNDPROCAREAlen DATABLCK ;
   [ Пример:  WNDPROCAREA  WPA ]
   [ Далее вызов WPA посылает в стек ДССП-адрес зарезервированной области ]

 [*** Команда ФОРМИРОВАНИЯ WND-ПРОЦЕДУРЫ, АДРЕС которой задается в теле ***]
:: : FORMWNDPROC [WPA] GTP _FORMWNDPROC [RealWPA] ;   PBIT FORMWNDPROC

  [ ПРИМЕР формирования тела ОКОННОЙ WND-Процедуры ]
1 %IF
  : WP [ hwnd, iMsg, wParam, lParam ]
       ." This is DSSP-procedure for execute as WND-Procedure "
       DD DD []
       0 [ LRESULT ] ;
    WNDPROCAREA  WPA [ Область формирования Exe WND-процедуры ]
  : FWP WPA FORMWNDPROC WP [RealAdr] ;
%FI

 [***********************************************************************]
 [****** ОБЪЯВЛЕНИЯ, ПОЛЕЗНЫЕ ДЛЯ ПРИМЕНЕНИЯ ИНТЕРФЕЙСА WIN32API ********]
 [***********************************************************************]

 [****** СИМВОЛЬНЫЕ КОНСТАНТЫ ИНТЕРФЕЙСА WIN32API ********]
B16  : | &0 ; [ совместимая с Си операция ИЛИ ]

 0000 VALUE NULL

 0000 VALUE MB_OK

 0001 VALUE CS_VREDRAW  [ Опции стиля класса ]
 0002 VALUE CS_HREDRAW

 0000 VALUE WHITE_BRUSH    [ Номер цвета кисти ]

 7F00 VALUE IDI_APPLICATION [ Идентификационный номер иконки ]
 7F00 VALUE IDC_ARROW       [ Идентификационный номер курсора ]

 000A VALUE SW_SHOWDEFAULT

 00CF0000 VALUE WS_OVERLAPPEDWINDOW  [ Стиль окна ]

 80000000 VALUE CW_USEDEFAULT      [ Опция создания окна ]

 0001 VALUE WM_CREATE         [ Сообщения окна ]
 0002 VALUE WM_DESTROY
 000F VALUE WM_PAINT

 0020 VALUE DT_SINGLELINE [ Опции рисования текста ]
 0001 VALUE DT_CENTER
 0004 VALUE DT_VCENTER

 [*** Команды резервирования области для формирования структур : ***]
B10

 [ Структура Сообщения и смещения ее полей ]
      28 VALUE MSGlen    :: : MSG MSGlen DATABLCK ;
  : .hwnd 00 + ; : .message 04 + ;  : .wParam 08 + ;   : .lParam 12 + ;
  : .time 16 + ; : .pt      20 + ;  : .pt.x   20 + ;   : .pt.y   24 + ;

 [ Структура Класса Окна и смещения ее полей ]
      48 VALUE WNDCLASSEXlen    :: : WNDCLASSEX WNDCLASSEXlen DATABLCK ;
  : .cbSize  00 + ;       : .style   04 + ;     : .lpfnWndProc 08 + ;
  : .cbClsExtra  12 + ;   : .cbWndExtra  16 + ; : .hInstance 20 + ;
  : .hIcon  24 + ;        : .hCursor   28 + ;   : .hbrBackground 32 + ;
  : .lpszMenuName  36 + ;  : .lpszClassName 40 + ;  : .hIconSm   44 + ;

 [ Структура Описателя Прямоугольной области экрана ]
      16 VALUE RECTlen   :: : RECT RECTlen DATABLCK ;
 [ Структура Описателя Формируемого Изображения ]
      64 VALUE PAINTSTRUCTlen   :: : PAINTSTRUCT PAINTSTRUCTlen DATABLCK ;

 [*** Команды объявлений переменных специальных типов : ***]
 :: : HWND LONG VAR ; [ WND HANDLE - Дескриптор Окна ]
 :: : HINSTANCE LONG VAR ; [ HANDLE INSTANCE - Дескриптор Приложения ]
 :: : HDC LONG VAR ;  [ Дескриптор Контекста Устройства ]
