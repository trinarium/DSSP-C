/*********************************************************/
/***     KERNEL for DSSP INTERPRETER  III VARIANT      ***/
/***  -----------------------------------------------  ***/
/***           WIN32 API       OPERATIONS              ***/
/***  -----------------------------------------------  ***/
/***       started by Burtsev A.A.  17 august   1998   ***/
/***           last modification    22 september 1998  ***/
/*********************************************************/

#include <windows.h>
// #include <sys/types.h>
// #include <sys/stat.h>

 void _RealADR(void)    {       // REALADR
 // convert DSSP-address to system address (real address)
         if (AT!=0)     // if not null address
                AT = (int32)ADR_ML(AT);
 }

 void _DsspADR(void)    {       // DSSPADR
 // convert real address to DSSP-address
         if (AT!=0)     // if not null address
                AT = (int32)DSSPADR(AT);
 }

// MACRO for pushing values in system stack
//           before call functions with parameters
#define SPUSH(VAL)      _asm push VAL

 typedef int32 (WINAPI *PFNWin32API) (void) ;

 HANDLE hLibKern ;      // Kernel32 DLL-library handle
 HANDLE hLibGDI  ;      // GDI32   DLL-library handle
 HANDLE hLibUser  ;     // User32 DLL-library handle
 HANDLE hLibMain  ;     // handle of main module
 PFNWin32API    pfnWin32API,pfnWin32; // address of function from DLL-library

 bool   InitDLLHandles () {
         // returns TRUE, if DDLs was loaded
         // returns FALSE, if DDLs cannot be loaded

        hLibMain = GetModuleHandle(NULL);

        hLibKern = GetModuleHandle("KERNEL32");
        if (!hLibKern)
                hLibKern = LoadLibrary("KERNEL32.EXE");

        hLibGDI  = GetModuleHandle("GDI32");
        if (!hLibGDI)
                hLibGDI  = LoadLibrary("GDI32.DLL");

        hLibUser = GetModuleHandle("USER32");
        if (!hLibUser)
                hLibUser = LoadLibrary("USER32.DLL");
        return ( (hLibMain!=NULL)&&(hLibKern!=NULL)&&
                     (hLibGDI !=NULL)&&(hLibUser!=NULL) );
 }

 /*********** CALL WIN32 API FUNCTION  **********/

 void _Win32API(void)   {       // WIN32API
 //  P1 P2 ... Pn n "FUNCNAME" WIN32API
        int32  i,n;
        int32   R; // for passing parameter and result
        bptr FuncName ; // name of function to be called
        //int32 len= ATPOP; // length of FuncName
        FuncName = ADR_MB(ATPOP);
        //FuncName[len-1]= 0; // to terminate by zero
        n= ATPOP; // number of parameters

// the following handles must be initiated during DSSP-initiation
        // hLibMain = GetModuleHandle(NULL);
        // hLibKern = GetModuleHandle("KERNEL32");
        // hLibGDI  = GetModuleHandle("GDI32");
        // hLibUser = GetModuleHandle("USER32");

        // look for function FuncName in DLL-modules :
        pfnWin32API= (PFNWin32API)GetProcAddress (hLibKern,FuncName);
        if (pfnWin32API==NULL)
          pfnWin32API= (PFNWin32API)GetProcAddress (hLibGDI,FuncName);
        if (pfnWin32API==NULL)
          pfnWin32API= (PFNWin32API)GetProcAddress (hLibUser,FuncName);
        if (pfnWin32API==NULL)
          pfnWin32API= (PFNWin32API)GetProcAddress (hLibMain,FuncName);

        if (pfnWin32API!=NULL) {
                // move parameters : from A-stack to system stack
                for (i=0; i<n; i++ ) {
                  R=ATPOP; SPUSH(R);
                }
                R = pfnWin32API () ; // call Win32API function
                APUSH(R);       // result of Win32API function
                APUSH(0);       // GOOD result of WIN32API operation
        } else {
                // Win32 API ERROR!!!
                // delete parameters from A-stack
                for (i=0; i<n; i++ ) R=ATPOP;
                APUSH(0);       // result of Win32API function
                APUSH(-1);      // BAD result of WIN32API operation
        }

 }// _Win32API

/*********** Calling External procedure (address  as parameter) ***/

void _WinFnExec(void)  {       // WinFnExec
//  P1 P2 ... Pn n address WinFnExec
    int32  i,n;
    int32   R; // for passing parameter and result
//  address of executable procedure
    pfnWin32 = (PFNWin32API) ATPOP ;
    n= ATPOP; // number of parameters

// the following handles must be initiated during DSSP-initiation
   if (pfnWin32!=NULL)
   { // move parameters : from A-stack to system stack
     for (i=0; i<n; i++ )
     { R=ATPOP;
       SPUSH(R);
     }
     R = pfnWin32 () ; // call Win32 function
     APUSH(R);       // result of Win32 function
     APUSH(0);       // GOOD result of WIN32 operation
    } else
        { // Win32  ERROR!!!
          // delete parameters from A-stack
          for (i=0; i<n; i++ ) R=ATPOP;
          APUSH(0);       // result of Win32 function
          APUSH(-1);      // BAD result of WIN32 operation
        }

}// _WinFnExec

/*********** FORMING  WINDOW PROCEDURE  **********/

typedef  void (  *CALLPROC) (void) ;
CALLPROC CallProc ;

bptr   AdrCallCode;    // address of begining of CallCode
int32  LenCallCode; // length of CallCode,
                // i.e. the code  for calling procedure,
                // which address is in CallProc variable;
                // the original of this code is formed
                // between labels BeginCallCode .. EndCallCode
                // in body of the function InitCallProc (see later )
                // ( after return statement )

 typedef LRESULT ( CALLBACK *CALLWNDPROC)
            ( HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam) ;
 CALLWNDPROC CallWndProc ;

 LRESULT CALLBACK ExecWndProc (lptr StartPC,
         HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam)  {
         // let's locate return address form CallProc
         // as parameter StartPC
         // e.g. exchange in system stack return addresses :
         //  from CallProc and from procedure, where CallProc was called
         _asm   mov EAX, StartPC
         _asm   mov EBX, 4[EBP]
         _asm   mov StartPC, EBX
         _asm   mov 4[EBP],EAX

         // push parameters of WND PROC to A-stack
         APUSH(hwnd);
         APUSH(iMsg);
         APUSH(wParam);
         APUSH(lParam);

         // execute Dssp-Wnd procedure
         ExecDpfCode((lptr)DSSPADR(StartPC));
         // hwnd iMsg wParam lParam  => Result

         // return result of Wnd Procedure
         return (ATPOP);
 }//ExecWndProc

 void _FormWndProc (void) {     // _FORMWNDPROC
        // Form Exe-code, which have to be executed as
        // CALLBACK WINDOW PROCEDURE

        // Sample of use :
        // WPADR '' WPROC ... [RealAdr 0f WPADR]

        // A-Stack before _FORMWNDPROC :
        //           [ DSSPADR_of_WNDPROC_Area, DsspAdr_of_Proc ]
        // A-Stack after _FORMWNDPROC :
        //           [ RealAdr_of_WNDPROCArea ]

  // Such code will be called as PASCAL-like procedure with 4 parameters,
  // which must be already placed in system stack (before calling)
  // and must be deleted from system stack by
  // called procedure before return from it

        // structure of code to be formed :
        // -------------------------------
        // AdrWndProc:
        //        CallProc()
        // AdrWndProcDssp:
        //        DsspWndProc
        //        cmd_halt
        // -------------------------------
        // AdrWndProc - address of place, where to be formed the code
        // length of this place must be at least =
        // length of CallCode + 2 long words
        //   = 6 + 8 = 14  bytes (for 80386 processor)
        bptr    AdrWndProc;
        lptr    AdrWndProcDssp; // position, where DSSP-code must be placed
        int32   i;
        lptr    DsspWndProc;

        // Top of A-stack = DSSP-procedure, that
        //       have to do actions as window procedure does
        DsspWndProc = (lptr)ATPOP;

        // SubTop of A-stack = address of place, where to be formed the code
        AdrWndProc = ADR_MB(ATPOP);

        // copy code between BeginCallCode ... and EndCode
        // to place, pointed by  AdrWndProc :
        for ( i=0; i<LenCallCode; i++ ) {
                AdrWndProc[i] = AdrCallCode[i] ;
        }

        // put DsspWndProc after CallCode
        AdrWndProcDssp = (lptr)(AdrWndProc+LenCallCode) ;
        AdrWndProcDssp[0] = (int32)DsspWndProc;
        // put then HALT command:
        AdrWndProcDssp[1] = cmd_Halt;

        APUSH(AdrWndProc); // put in operand stack
        // system address of window procedure, which have been formed

        return ;

 } // _FormWndProc

#pragma optimize( "", off )
void   InitCallProcVar () {

        // initiate CallProc procedure variable :
        CallProc = (CALLPROC)ExecWndProc ;

        // initiate AdrCallCode and LenCallCode variables :
        // AdrCallCode = BeginCallCode;
        // LenCallCode = EndCallCode - BeginCallCode ;
        _asm    mov     EAX, OFFSET BeginCallCode
        _asm    mov     AdrCallCode,EAX
        _asm    mov     EBX,OFFSET EndCallCode
        _asm    sub     EBX, EAX
        _asm    mov     LenCallCode,EBX

        return ;

        //goto EndCallCode ;
        /***** THE CALL CODE : *****/
        BeginCallCode :
          CallProc();
        EndCallCode: ;
        /* THIS CODE HAVE NOT TO BE executed  !!! */
        /* IT HAVE  TO BE COPIED TO THE BEGINING OF BODY
        /* OF THE WINDOW PROCEDURE, WHICH IS BEING FORMED  */
        /***************************/
 } //InitCallProcVar
#pragma optimize( "", on )


 /***********************************************/
 /***    INITiate WIN32API --- DSSP interface ***/
 /***********************************************/
 bool InitWin32API(void)        {       // Initiate DSSP--WIN32API interface
         InitCallProcVar ();
         return(InitDLLHandles ());
 }

/*********************************************************/
/***  -----------------------------------------------  ***/
/***    for testing    WIN32 API    OPERATIONS         ***/
/***  -----------------------------------------------  ***/
/*********************************************************/

#ifdef  TEST_VERS

/*------------------------------------------------------------
   DSSPWIN.C -- Displays "DSSP Win 95!" in client area
        (c) Burtsev Alexey, adopted from Charles Petzold, 1998
  ------------------------------------------------------------*/

LRESULT CALLBACK WndProc (HWND, UINT, WPARAM, LPARAM) ;

//int WINAPI DsspWin (HINSTANCE hInstance, HINSTANCE hPrevInstance,
//                    PSTR szCmdLine, int iCmdShow)
int DsspWin (void) {
     static char szAppName[] = "dsspwin" ;
     HWND        hwnd ;
     MSG         msg ;
     WNDCLASSEX  wndclass ;
     HINSTANCE hInstance =  GetModuleHandle(NULL) ;
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
                    NULL) ;                  // creation parameters

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
         // int32       i2 =  DT_CENTER ;
         // int32       i3 =  DT_VCENTER ;

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

 /***********************************************/
 /***       TESTing functions of Win32 API    ***/
 /***********************************************/

/****************  Test DsspWin ****/

 void _TestWin32API(void)       {       // Test Win32 API
        int Res;
//      Res = DsspWin ( NULL, //GetModuleHandle(NULL)
//                           NULL, NULL, SW_SHOWDEFAULT);
        Res = DsspWin ( );
        APUSH(Res);
 }
/*****************/

/****************  Test Message Box *****/
 void _TestWin32API_(void)      {       // Test Win32 API
        int32 Res;

        Res= MessageBox(NULL,
                                "TestWin32API, MessageBox",
                                "DSSPMES",
                                MB_OK);
        APUSH(Res);
 }
/*****************/

 /********** TEST for calling WINDOW procedure (with 4 parameters) **/
 /*********  pointed by real address ******/
 void _TestExecWndProc  (void) { // TestExecWndProc
        // Call WND PROC, which address is Top of A-stack
        LRESULT Res;
        CALLWNDPROC     CallWndProc;
        CallWndProc = (CALLWNDPROC)ATPOP;
        Res = CallWndProc( (HWND) 1, 2, 3, 4);
 }


 /********************************************************/
 /***       SOME USEFUL TESTs of Win32 API functions   ***/
 /********************************************************/

//#define FUNCNAME "GetVersion"
#define FUNCNAME "GetCurrentDirectoryA"
//#define FUNCNAME "MulDiv"

 // DEBUG VERSION !!!
 void _Win32API_(void)  {       // WIN32API
        int32   R; // for passing parameter and result
        bptr FuncName ; // name of function to be called

        FuncName = FUNCNAME;
        //  hLibKern = LoadLibrary("KERNEL32.EXE");
        //  hLibGDI  = LoadLibrary("GDI32.DLL");
        //  hLibUser = LoadLibrary("USER32.DLL");
        hLibMain = GetModuleHandle(NULL);
        hLibKern = GetModuleHandle("KERNEL32");
        hLibGDI  = GetModuleHandle("GDI32");
        hLibUser = GetModuleHandle("USER32");

        pfnWin32API= (PFNWin32API)GetProcAddress (hLibKern,FuncName);
        if (pfnWin32API==NULL)
          pfnWin32API= (PFNWin32API)GetProcAddress (hLibGDI,FuncName);
        if (pfnWin32API==NULL)
      pfnWin32API= (PFNWin32API)GetProcAddress (hLibUser,FuncName);
        if (pfnWin32API==NULL)
      pfnWin32API= (PFNWin32API)GetProcAddress (hLibMain,FuncName);

        if (pfnWin32API!=NULL) {
        //      R=ATPOP; SPUSH(R);      // last  parameter
                R=ATPOP; SPUSH(R);      // 2-th  parameter
                R=ATPOP; SPUSH(R);      // first parameter
                R = pfnWin32API () ;
                APUSH(R);
        } else
                APUSH(0);

        //  FreeLibrary (hLibKern) ;
        //  FreeLibrary (hLibGDI) ;
        //  FreeLibrary (hLibUser) ;
 }

//#define       TEST_Win32API

#ifdef TEST_Win32API

 BOOL bl;
 TCHAR tchar;

 char  WINAPI  TstChar ( char ch, BOOL b, int32 i ) {
        int16 Int;
         bl = b;
         tchar = ch;
         Int = (int16) (bl+ (BOOL)tchar + i) ;
         return( -1);
 }

 void TestingWin32Api(void)     {       // for testing calling Win32API
        int32 R;

        // R= TstChar('A', TRUE, -1 );
        _asm    push  -1;
        SPUSH(TRUE);
        SPUSH(tchar);
        R= TstChar('A', TRUE, -1 );
        // R= TstChar1();
 }

#endif
#endif