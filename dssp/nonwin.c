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


 void _Win32API(void)   {       // WIN32API
 }// _Win32API

/*********** Calling External procedure (address  as parameter) ***/

void _WinFnExec(void)  {       // WinFnExec
}// _WinFnExec

/*********** FORMING  WINDOW PROCEDURE  **********/

 void _FormWndProc (void) {     // _FORMWNDPROC
 } // _FormWndProc


#ifdef  TEST_VERS


 /***********************************************/
 /***       TESTing functions of Win32 API    ***/
 /***********************************************/

/****************  Test DsspWin ****/

 void _TestWin32API(void)       {       // Test Win32 API
 }
/*****************/


 /********** TEST for calling WINDOW procedure (with 4 parameters) **/
 /*********  pointed by real address ******/
 void _TestExecWndProc  (void) { // TestExecWndProc
 }

#endif