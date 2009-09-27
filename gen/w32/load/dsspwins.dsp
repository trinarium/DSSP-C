 [*****************************************************************]
 [***------ СЛОЖНЫЙ ПРИМЕР ПРИМЕНЕНИЯ ИНТЕРФЕЙСА WIN32API ----*****]
 [******************* ( со стуктурами ) ***************************]
 
 CR 
 ." DSSPWIN : DSSP-приложение со своим окном ( со структурами ) "
 
 [--- 5 -------- ЗАПУСК ПРИЛОЖЕНИЯ В ОТДЕЛЬНОМ (ДОЧЕРНЕМ) ОКНЕ -----------]
 [    ( СО СВОЕЙ ОКОННОЙ ПРОЦЕДУРОЙ И СВОИМ ЦИКЛОМ ОБРАБОТКИ СООБЩЕНИЙ )  ]
 [ ПРИМЕЧАНИЕ: Команда-ТЕСТ TestWin32API запускает такое же окно, но      ] 
 [        реализована она в ядре на языке Си ( см. winapi.c & dsspwin.c ) ] 
 
 [------------------------------------------------------------
   DSSPWIN.C -- Displays "DSSP Win 95!" in client area
        (c) Burtsev Alexey, adopted from Charles Petzold, 1998
  ------------------------------------------------------------]
  
  LONG VAR szAppName [ указатель на строку имени класса Окна ]
  : !szAppName! "dsspwin " \0 REALADR ! szAppName ;

     [--- ПЕРЕМЕННЫЕ основной процедуры ---]
  HWND VAR hwnd  [ Дескриптор Окна ]
  MSG  VAR msg   [ Сообщение ]           : &msg msg REALADR ;
  WNDCLASSEX VAR wndclass [ Класс Окна ]  : &wndclass wndclass REALADR ;
  HINSTANCE  VAR hInstance [ Идентификатор (дескриптор) приложения) ]
 
  : !hInstance 	NULL 1 "GetModuleHandleA " WIN32API ! hInstance ;
	 
  : !!!wndclass !0 wndclass ; [ обнуление всей структуры ]
                   [ Установка отдельных полей : ]
  : !wndclass.cbSize WNDCLASSEXlen  wndclass ! .cbSize ;
  : !wndclass.style CS_HREDRAW CS_VREDRAW | wndclass ! .style ;
  : !wndclass.lpfnWndProc FormDsspWndProc wndclass ! .lpfnWndProc ;
  : !wndclass.hInstance  hInstance wndclass ! .hInstance ;
  : !wndclass.hIcon NULL IDI_APPLICATION 2 "LoadIconA " WIN32API
                    wndclass ! .hIcon ;
  : !wndclass.hCursor NULL IDC_ARROW 2 "LoadCursorA " WIN32API
                    wndclass ! .hCursor ;
  : !wndclass.hbrBackground WHITE_BRUSH 1 "GetStockObject " WIN32API 
                    wndclass ! .hbrBackground ;
  : !wndclass.lpszMenuName  NULL wndclass ! .lpszMenuName ;
  : !wndclass.lpszClassName szAppName wndclass ! .lpszClassName ;
  : !wndclass.hIconSm NULL IDI_APPLICATION 2 "LoadIconA " WIN32API
                    wndclass ! .hIconSm ;
                    
  : RegisterClass &wndclass 1 "RegisterClassExA " WIN32API D ;
  
  : CreateWindow [ ] 0
              szAppName         [ Имя класса Окна ]
              "The Dssp Win Program " \0 REALADR  [ Заголовок ]
              WS_OVERLAPPEDWINDOW [ стиль окна ]
              CW_USEDEFAULT C C C [xpos,ypos,xsize,ysize]
              NULL                [ родительское окно (Нет) ]
              NULL                [ window menu handle (Пока нет)]
              hInstance           [ program instance handle ]
	      NULL    [ ук-ль на дополнительные параметры окна (Пока нет) ]
	      12 "CreateWindowExA " WIN32API [hwnd] ! hwnd [ ] ;
	            
   : ShowWindow hwnd SW_SHOWDEFAULT 2 "ShowWindow " WIN32API D ;
   : UpdateWindow hwnd 1 "UpdateWindow " WIN32API D ;

   : GetMessage &msg NULL 0 0 4 "GetMessageA " WIN32API [ 0 - Конец цикла ] ;
   : TranslateMessage &msg 1 "TranslateMessage " WIN32API D ;
   : DispatchMessage &msg  1 "DispatchMessageA "  WIN32API D ;
         
 [---------------- ОСНОВНАЯ ПРОЦЕДУРА --------------------------------]
 [ int WINAPI DsspWin (HINSTANCE hInstance, HINSTANCE hPrevInstance,  ]
 [                  PSTR szCmdLine, int iCmdShow)                     ]
 [ Здесь в упрощенном варианте:  int DsspWin (void)                   ]
:: : DSSPWIN [ ]  [ ON WIN32FAIL! NOP ]
             !szAppName! !hInstance
          [ Инициализация структуры класса окна ] 
             !!!wndclass  !wndclass.cbSize  !wndclass.style 
             !wndclass.lpfnWndProc      !wndclass.hInstance  
             !wndclass.hIcon  !wndclass.hCursor   !wndclass.hbrBackground 
             !wndclass.lpszMenuName    !wndclass.lpszClassName 
             !wndclass.hIconSm 
             
          RegisterClass   [ Регистрация класса окнаа ] 

          CreateWindow    [ Создание окнаа ] 
          ShowWindow    [ Высвечивание окна ]
          UpdateWindow  [ Обновление окна ]
          
          RP MsgLoop    [ цикл обработки сообщений ]
          msg .wParam   [ msg.wParam ] ;
   : MsgLoop GetMessage EX0 TranslateMessage DispatchMessage ;

          [---- Формирование процедуры окна ------------]
    WNDPROCAREA  DsspWPArea [ Область формирования EXE-тела WND-процедуры ]
  : FormDsspWndProc DsspWPArea FORMWNDPROC DsspWndProc [RealAdr] ;
  
  [--- ПЕРЕМЕННЫЕ ОКОННОЙ процедуры ---]
     HDC VAR     hdc 
     PAINTSTRUCT VAR ps    : &ps ps REALADR ;
     RECT        VAR rect  : &rect rect REALADR ;
 [ ЗАМЕЧАНИЕ: Эти переменные д.быть ЛОКАЛЬНЫМИ в процедуре ОКНА   ]
 [           ( для обеспечения реентерабельности процедуры окна ) ]

 [----------------  ПРОЦЕДУРА ОКНА --------------------------------]
 [    LRESULT CALLBACK DsspWndProc (HWND, UINT, WPARAM, LPARAM)    ]
:: : DsspWndProc [ hwnd, iMsg, wParam, lParam ]    
                 C3 BR 
                       WM_CREATE  Handle_Msg
                       WM_DESTROY Handle_WM_Destroy
                       WM_PAINT   Handle_WM_Paint
                    ELSE  DefWindowProc       [ LResult ] ;
  : Handle_Msg DDDD 0 ;             
  : DefWindowProc  [ hwnd, iMsg, wParam, lParam ]
                   4 "DefWindowProcA "  WIN32API [ LResult ] ;
  : Handle_WM_Destroy DDDD 0 1 "PostQuitMessage " WIN32API D 0 [0] ;
  : Handle_WM_Paint [ hwnd, iMsg, wParam, lParam ] 
               BeginPaint ! hdc  
               GetClientRect 
               "DSSP Win 95!" DrawText 
               EndPaint  
               Handle_Msg [ 0 ] ;
    : BeginPaint C4 [hwnd] &ps 2 "BeginPaint " WIN32API [hdc] ;
    : GetClientRect C4 [hwnd] &rect 2 "GetClientRect " WIN32API D [] ;
    : DrawText [Txt,len] \0 REALADR hdc E2 -1 &rect
               DT_SINGLELINE DT_CENTER | DT_VCENTER | 
               5 "DrawTextA " WIN32API D [] ;
    : EndPaint C4 [hwnd]  &ps 2 "EndPaint " WIN32API D [] ;
  
[***** Эта Оконная Процедура действует по такому алгоритму (на Си)  :

LRESULT CALLBACK WndProc (HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam)
{    HDC         hdc ;
     PAINTSTRUCT ps ;
     RECT        rect ; 
     switch (iMsg)  {
          case WM_CREATE :  return 0 ;
          case WM_PAINT :
               hdc = BeginPaint (hwnd, &ps) ;
               GetClientRect (hwnd, &rect) ;
               DrawText (hdc, "DSSP Win 95!", -1, &rect,
               DT_SINGLELINE | DT_CENTER | DT_VCENTER) ;
               EndPaint (hwnd, &ps) ;
               return 0 ;
          case WM_DESTROY :
               PostQuitMessage (0) ;
               return 0 ;
          }
     return DefWindowProc (hwnd, iMsg, wParam, lParam) ;
}
*******************************************************************]     

