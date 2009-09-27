 [*****************************************************************]
 [****---- ПРОСТЫЕ ПРИМЕРЫ ПРИМЕНЕНИЯ ИНТЕРФЕЙСА WIN32API ----*****]
 [*****************************************************************]

 CR 
 ." ---- Some Examples of use WIN32API :"  CR
 ."  1) GetVersion 2) GetCurDir 3) [N1,N2,nDiv] MulDiv [Res] 4) DsspMsgBox "
 
 [--- 1 -------- ПОЛУЧЕНИЕ НОМЕРА ВЕРСИИ  -------------]
    [ NomVers=GetVersion(); ]
:: : GetVersion [] 0 "GetVersion "  WIN32API [NomVers]  ;
	
 [--- 2 -------- ПОЛУЧЕНИЕ ИМЕНИ ТЕКУЩЕГО КАТАЛОГА -------------]
    [	RealLen = GetCurrentDirectory(nBufferLength,lpBuffer);  ]
    256 VALUE NameDirLen   NameDirLen DATABLCK NameDir
:: : GetCurDir []  NameDirLen  NameDir REALADR [nBufferLength,lpBuffer]
                 2 "GetCurrentDirectoryA "  WIN32API [RealLen] 
                 NameDir E2 TOS [] ;
                 
 [--- 3 -------- СОВМЕСТНОЕ УМНОЖЕНИЕ/ДЕЛЕНИЕ ДЛИННЫХ ЦЕЛЫХ  -------------]
    [ Res= MulDiv(nNumber,nNumerator,nDenominator); ]
:: : MulDiv [Num1,Num2,nDiv] 3 "MulDiv "  WIN32API [Num1*Num2/nDiv] ;
    
 [--- 4 -------- ВЫЗОВ ДИАЛОГОВОГО ОКНА СООБЩЕНИЯ -----------]
    [ Res= MessageBox(NULL,"TestWin32API, MessageBox","DSSPMES",MB_OK); ]
:: : DsspMsgBox [] NULL " Hello! This is TEST of MessageBox " \0 REALADR
                   "DSSP_MESSAGE " \0 REALADR  MB_OK 
                   4 "MessageBoxA " WIN32API [Res] D ;

