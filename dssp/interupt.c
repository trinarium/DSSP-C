/*********************************************************/
/***     KERNEL for DSSP INTERPRETER  III VARIANT      ***/
/***  -----------------------------------------------  ***/
/***               INTERRUPT    MEHANISM               ***/
/***  -----------------------------------------------  ***/
/***         started by Burtsev A.A.  04 august1999    ***/
/***              last modification   29 august1999    ***/
/*********************************************************/

#ifdef	Watcom
#include "dos.h"
#endif

#define MAX_INTE (255)  // number of external interrupts
#define MAX_INTI (255)  // number of internal interrupts

cmnd	ITABLE [MAX_INTE] ; // Table of interrupt vectors (External)
cmnd	ITABLI [MAX_INTI] ; // Table of interrupt vectors (Internal)

/*********************************************************/
/***          Open/Close/Init hardware interrupts      ***/
/*********************************************************/

 void _ION (void) {  // Enable (open) Hardware Interrupts
#ifdef	Watcom
 	_enable();
#endif
 }

 void _IOFF (void) {  // Disable (close) Hardware Interrupts
#ifdef	Watcom
 	_disable();
#endif
 }

 void _IRESET (void) {  //  ReSet (Init) Hardware Interrupts
 }

#define IENTRY	_IOFF()	//  Entry into critical section (No interrupts!)
#define IEXIT 	_ION () //  Exit  from critical section (YES interrupts!)

/*********************************************************/
/***  Put/Get Operations with Interrupt Request Queue  ***/
/*********************************************************/

 //------ Interrupt Request QUEUE Declarations :
 
#define IRQMax (64) // Maximum size of interrupt queue
int16 IRQCnt;	// --- Counter of items in queue 
int16 IRQIn ;	// Index-pointer to end of queue
int16 IRQOut;	// Index-pointer to begin of queue
int16 IRQBuf[IRQMax]; // Buffer to store queue items (interrupt requests)

// macro operation to increment index mod IRQMax
#define INCM(IRQInd)	{ IRQInd++; if (IRQInd==IRQMax) IRQInd=0; }

 void IRQPut (int16 INum) {  // Put interrupt INum in Queue :
	if (IRQCnt<IRQMax) {
		INCM(IRQIn);
		IRQBuf[IRQIn]= INum;
		IRQCnt++;
	}
 }

 int16 IRQGet (void) {  // Get interrupt from Queue :
	int16 INum;
	if (IRQCnt>0) {
		INum= IRQBuf[IRQOut];
		INCM(IRQOut);
		IRQCnt--;
		return (INum);
	} else 
		return(FALSE);
 }

 void IRQInit (void) {  // Init Interrupt Request Queue 
	 IRQCnt= 0;
	 IRQIn = 0;
	 IRQOut= 1;
 }

/*********************************************************/
/***        Open/Close/Raise DSSP interrupts           ***/
/*********************************************************/

 bool IEFlag;	// flag, which enables DSSP interrupts 
//  Їыру ЁрчЁх°хэш  юсЁрсюЄъш яЁхЁ√трэш  т ─╤╤╧ ----
//	; ╬сЁрсюЄър яЁхЁ√трэшщ т ─╤╤╧ ЁрчЁх°хэр, ъюуфр Їыру >0 (=1,True)
//	; ╧ЁхЁ√трэш  сєфєЄ чрЇшъёшЁютрэ√, эю шї юсЁрсюЄър сєфхЄ
//	; т√яюыэ Є№ё  Єюы№ъю яюёых єёЄрэютъш ¤Єюую Їырур
//	; ёЁрчє яюёых чртхЁ°хэш  Єхъє∙хщ яЁшьшЄштэющ ─╤╤╧-яЁюЎхфєЁ√


 void _IntrON (void) { // xINTRON Enable DSSP Interrupts
	IENTRY;
	IEFlag = TRUE;
	IntrptFlag= IRQGet();
	IEXIT;
 }

 void _IntrOFF(void) { // xINTROFF Disable DSSP Interrupts
	IENTRY; // for modification IEFlag in disable interrupt mode
	IEFlag = FALSE;
	IEXIT;
 }

 void RAISE_INTR (int16 INum) {  // RAISE interrupt INum :
	IENTRY; // for modification IEFlag in disable interrupt mode
	IRQPut(INum);
	if ( IEFlag && (IntrptFlag==FALSE) )
		IntrptFlag= IRQGet();
	IEXIT;
 }

 void TEST_INTR (void) {  // TEST: interrupt DSSP interpreter or not ?
	if (IEFlag && (IRQCnt>0) ) {
		IENTRY; // for modification in disable interrupt mode
		IntrptFlag= IRQGet();
		IEXIT;
	} else
 		IntrptFlag= FALSE ;
}


/*********************************************************/
/***        Link to hardware interrupt mehanism        ***/
/*********************************************************/

//----------   Get/Set/Restore System Interrupt Link -------------
//  It means to perform some preparations 
// ( specific for concrete environment ) 
// so that on interrupt request number Num 
// it will be executed any function, which
// registers occured interrupt, assigning 
// the number Num to interrupt flag IntrptFlag

#ifdef	Watcom    //---------- watcom for Dos386 ----------

 typedef void (__interrupt __far *INT_HANDLER_PTR)() ;
 
 void __interrupt __far DsspIntHandler_1Bh () {
 	RAISE_INTR (0x1B);
 }
 void __interrupt __far DsspIntHandler_1Ch () {
 	RAISE_INTR (0x1C);
 }
 void __interrupt __far DsspIntHandler_23h () {
 	RAISE_INTR (0x23);
 }
 void __interrupt __far DsspIntHandler_24h () {
 	RAISE_INTR (0x24);
 }
 void __interrupt __far DsspIntHandler_61h () {
 	RAISE_INTR (0x61);
 }
 
 void SetSysIntVect (int16 Num) {
	switch (Num) {
	  case 1 : 
		       break; 
	  case 2 : 
		       break; 
	  case 0x1B : 
	               _dos_setvect(0x1B, DsspIntHandler_1Bh);
		       break; 
	  case 0x1C : 
	               _dos_setvect(0x1C, DsspIntHandler_1Ch);
		       break; 
	  case 0x23 : 
	               _dos_setvect(0x23, DsspIntHandler_23h);
		       break; 
	  case 0x24 : 
	               _dos_setvect(0x24, DsspIntHandler_24h);
		       break; 
	  case 0x61 : 
	               _dos_setvect(0x61, DsspIntHandler_61h);
		       break; 
          		       
	  default:
		       ;

  	} // switch
 }// SetSysIntVect

 void GetSysIntVect (int16 Num, int32 *IHPtr1p, int32 *IHPtr2p) {
	INT_HANDLER_PTR IHPtr ;
	IHPtr =_dos_getvect(Num);
	// destruct IHPtr in two long int values
	*IHPtr1p = *((lptr)(&IHPtr));  // get low long word
	*IHPtr2p = *((lptr)(&IHPtr)+1); // get high long word
 }// GetSysIntVect

 void RestoreSysIntVect (int16 Num, int32 IHPtr1, int32 IHPtr2  ) {
	INT_HANDLER_PTR IHPtr ;
	// construct IHPtr using two long int values
	*(wptr)((lptr)(&IHPtr)+1) = (word)IHPtr2 ; // set low word of second high long part
	*((lptr)(&IHPtr)) = IHPtr1;  // set low long word part
 	_dos_setvect(Num, (INT_HANDLER_PTR)IHPtr);
 }// ResetSysIntVect
 
#else       //---------- Not watcom for Dos386 ----------

 void SetSysIntVect (int16 Num) {
	switch (Num) {
	  case 1 : 
		       break; 
	  case 2 : 
		       break; 
	  default:
		       ;

  	} // switch
 }// SetSysIntVect

 void GetSysIntVect (int16 Num, int32 *IHPtr1p, int32 *IHPtr2p) {
 }// GetSysIntVect

 void RestoreSysIntVect (int16 Num, int32 IHPtr1, int32 IHPtr2  ) {
 }// ResetSysIntVect 

#endif
 
/*********************************************************/
/***     Main operations of DSSP interrupt mehanism    ***/
/*********************************************************/

// invoke interrupt handler (when interrupt was happened)
 void _Interrupt (void) {
	int16 IntNum; // interrupt number
	IntNum= IntrptFlag; // IntrptFlag contains interrupt number
	TEST_INTR();        // get new interrupt or reset InrptFlag
	if (IntNum<0) {  // this is internal interrupt
	  if (IntNum==-1)	StopFlag = 1;
	    // Internal interruption number -1 
	    // means to stop interpretation (HALT)
	  else {
		cmd=(lptr)ITABLI[-IntNum]; // address of DSSP-procedure 
		EXECMD;      // to do internal interrupt handling
	  }
	} else {         // this is external interrupt
		cmd=(lptr)ITABLE[IntNum]; // address of DSSP-procedure 
		EXECMD;      // to do external interrupt handling
	}
 }// _Interrupt

 // raise interrupt, number of which in AT
 void _RaiseIntrpt(void) { // [n] INTR []
	RAISE_INTR((int16)ATPOP); 
 }// _RaiseInterrupt

 // Get Interrupt Vector
 void _GetIntVect(void) { 
	// [n] GetIntVect [DsspIntVect,SysIntVectLo,SysIntVectHi]
	int16 IntNum; // interrupt number
	int32 IH1, IH2 ;
	IntNum = (int16)AT ;
	if (IntNum<0) {  //  internal interrupt
		AT = ITABLI[-IntNum] ;
		APUSH(0);
		APUSH(0);
	} else {
		AT = ITABLE[IntNum] ;
		GetSysIntVect(IntNum,&IH1,&IH2); // get system interrupt link
		APUSH(IH1);
		APUSH(IH2);
	}
 }// _GetIntVect

 // Set Interrupt Vector
 void _SetIntVect(void) { // [DsspIntVect,n] SetIntVect []
	int16 IntNum; // interrupt number
	IntNum = (int16)ATPOP ;
	if (IntNum<0) {  //  internal interrupt
		ITABLI[-IntNum] = AT;
	} else {         //  external interrupt
		ITABLE[IntNum] = AT ;
		SetSysIntVect(IntNum); // set system interrupt link 
	}
	ADEL; 
 }// _SetIntVect

 // Restore Interrupt Vector
 void _RestoreIntVect(void) { // [DsspIV,SysIVLo,SysIVHi,n] ... []
	// [DsspIntVect,SysIntVectLo,SysIntVectHi,n] RestoreIntVect []
	int16 IntNum; // interrupt number
	int32 IH1, IH2 ;
	IntNum = (int16)ATPOP ;
	if (IntNum<0) {  //  internal interrupt
		ADELn(2); 	// delete SysIntVect
		ITABLI[-IntNum] = ATPOP; // DsspIntVect
	} else {         //  external interrupt
		APOP(IH2);	 // get SysIntVectHi
		APOP(IH1);	 // get SysIntVectLo
		RestoreSysIntVect(IntNum,IH1,IH2); // reset system interrupt link
		ITABLE[IntNum] = ATPOP ;  // DsspIntVect
	}
 }// _RestoreIntVect

 // Init Interrupt Mehanism
 void InitIntrMh (void) {
	 int16 i;
	 for (i=MAX_INTE;i>=0;i--) 
		 ITABLE[i]= cmd_Nop ;
	 for (i=MAX_INTI;i>=0;i--) 
		 ITABLI[i]= cmd_Nop ;
	 IRQInit();	// Init IR Queue
	 IEFlag = TRUE;  // Init Interrupt Enable Flag = TRUE
	 IntrptFlag = FALSE;  // Interrupt Interpreter Flag
 }// _InitInrptMh


/*********************************************************/

