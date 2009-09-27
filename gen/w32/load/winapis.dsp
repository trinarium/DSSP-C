[************************************************************************]
[*****            INTERFACE: DSSP -- WIN32 API                 **********]
[*****   started 19.08.1998         last modification  20.10.1998   *****]
[************************* ( со структурами ) ***************************]
B10  USE SYSTEM 
CR ." WINAPIs - Interface: DSSP --- Win32API ( со структурами ) "

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
  
 [****** НОВЫЕ ТИПЫ СТРУКТУР для  ИНТЕРФЕЙСА WIN32API ********]
B10 
   
 [*** Cпецифические типы : ***]

 :: : HWND LONG ; [ WND HANDLE - Дескриптор Окна ]
 :: : HINSTANCE LONG ; [ HANDLE INSTANCE - Дескриптор Приложения ]
 :: : HDC LONG ;  [ Дескриптор Контекста Устройства ]
 :: : HICON LONG ;  [ Дескриптор Иконки ]
 :: : HCURCOR LONG ;  [ Дескриптор Курсора ]
 :: : HBRUSH LONG ;  [ Дескриптор Кисти ]
 :: : LPCSTR LONG ; [ Указатель строки ]
 :: : WNDPROC LONG ; [ Указатель процедуры окна ]
 
 :: : WPARAM WORD ;
 :: : LPARAM LONG ;
 :: : DWORD  LONG ;
 :: : LONGINT LONG ;
 :: : BOOL   LONG ;
  
 [ структура ПОЗИЦИИ на экране ]
  STRUCT: POINT  LONGINT VAR .x LONGINT VAR .y  ;STRUCT
 
 [ Структура Сообщения ]
  STRUCT: MSG 
      HWND   VAR .hwnd    DWORD  VAR .message   
      WPARAM VAR .wParam  WPARAM VAR .wParam2 [дополнение до длинного целого]
      LPARAM VAR .lParam  DWORD  VAR .time    
      POINT  VAR .pt
  ;STRUCT  MSG SIZEVALUE MSGlen  
      
 [ Структура Класса Окна ]

  STRUCT: WNDCLASSEX 
     DWORD  VAR .cbSize       DWORD   VAR .style      WNDPROC   VAR .lpfnWndProc 
    LONGINT VAR .cbClsExtra   LONGINT VAR .cbWndExtra
     HINSTANCE VAR .hInstance HICON   VAR .hIcon      HCURCOR VAR .hCursor       HBRUSH    VAR .hbrBackground 
     LPCSTR VAR .lpszMenuName LPCSTR  VAR .lpszClassName 
     HICON  VAR .hIconSm  
  ;STRUCT    WNDCLASSEX  SIZEVALUE WNDCLASSEXlen    
                               
 [ Структура Описателя Прямоугольной области экрана ]
  STRUCT: RECT  LONGINT VAR .left  LONGINT VAR .top
            LONGINT VAR .right LONGINT VAR .bottom 
  ;STRUCT  RECT SIZEVALUE RECTlen
          
 [ Структура Описателя Формируемого Изображения ]
  STRUCT: PAINTSTRUCT
            HDC  VAR .hdc       BOOL VAR .fErase
            RECT VAR .rcPaint   BOOL VAR .fRestore  BOOL VAR .fIncUpdate
            BYTE 31 VCTR .rgbReserved  [ зарезервировано]
  ;STRUCT   PAINTSTRUCT SIZEVALUE PAINTSTRUCTlen

 
