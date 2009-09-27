/*********************************************************/
/***  -----------------------------------------------  ***/
/***    for testing    WIN32 API    OPERATIONS         ***/
/***  -----------------------------------------------  ***/
/***         started by Burtsev A.A. 14 august 1998    ***/
/***             last modification   22 septem 1998    ***/
/*********************************************************/

/*------------------------------------------------------------
   DSSPWIN.C -- Displays "DSSP Win 95!" in client area
        (c) Burtsev Alexey, adopted from Charles Petzold, 1998
  ------------------------------------------------------------*/

#include <windows.h>

LRESULT CALLBACK WndProc (HWND, UINT, WPARAM, LPARAM) ;

//int WINAPI DsspWin (HINSTANCE hInstance, HINSTANCE hPrevInstance,
//                    PSTR szCmdLine, int iCmdShow)
int DsspWin (void) {
     static char szAppName[] = "dsspwin" ;
     HWND        hwnd ;
     MSG         msg ;
     WNDCLASSEX  wndclass ;
	 HINSTANCE hInstance = 	GetModuleHandle(NULL) ;
     wndclass.cbSize        = sizeof (wndclass) ;
     wndclass.style         = CS_HREDRAW | CS_VREDRAW ;
     wndclass.lpfnWndProc   = WndProc ;
     wndclass.cbClsExtra    = 0 ;
     wndclass.cbWndExtra    = 0 ;
     wndclass.hInstance     = hInstance ;
     wndclass.hIcon         = LoadIcon (NULL, IDI_APPLICATION) ;
     wndclass.hCursor       = LoadCursor (NULL, IDC_ARROW) ;
     wndclass.hbrBackground = (HBRUSH) GetStockObject (WHITE_BRUSH) ;
     wndclass.lpszMenuName  = NULL ;
     wndclass.lpszClassName = szAppName ;
     wndclass.hIconSm       = LoadIcon (NULL, IDI_APPLICATION) ;

     RegisterClassEx (&wndclass) ;

     hwnd = CreateWindow (szAppName,         // window class name
		            "The Dssp Win Program",     // window caption
                    WS_OVERLAPPEDWINDOW,     // window style
                    CW_USEDEFAULT,           // initial x position
                    CW_USEDEFAULT,           // initial y position
                    CW_USEDEFAULT,           // initial x size
                    CW_USEDEFAULT,           // initial y size
                    NULL,                    // parent window handle
                    NULL,                    // window menu handle
                    hInstance,               // program instance handle
	            NULL) ;		     // creation parameters

     ShowWindow (hwnd, /*iCmdShow*/  SW_SHOWDEFAULT) ;
     UpdateWindow (hwnd) ;

     while (GetMessage (&msg, NULL, 0, 0))
          {
          TranslateMessage (&msg) ;
          DispatchMessage (&msg) ;
          }
     return msg.wParam ;
     }

LRESULT CALLBACK WndProc (HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam)
     {
     HDC         hdc ;
     PAINTSTRUCT ps ;
     RECT        rect ;
	 // int32  i1 =  DT_SINGLELINE ;
	 // int32	i2 =  DT_CENTER ;
	 // int32	i3 =  DT_VCENTER ;

     switch (iMsg)
          {
          case WM_CREATE :   return 0 ;

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
