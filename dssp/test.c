/*********************************************************/
/***     KERNEL for DSSP INTERPRETER  III VARIANT      ***/
/***  -----------------------------------------------  ***/
/***            OPERATIONS for TESTING                 ***/
/***  -----------------------------------------------  ***/
/***         started by Burtsev A.A.  23 march 1998    ***/
/***              last modification   17 may   1998    ***/
/*********************************************************/

/*********************************************************/
/******           DIALOG  OPERATIONS               *******/
/*********************************************************/

 void _PrT (void)	{	// .
	 // Print Top of A-stack as int32 value
	 printf("%10ld ", AT);
 }

 void _PrTx (void)	{	// .X
	 // Print Top of A-stack as 16-based value
	 printf("%#10lX ", AT);
 }

void _PrS (void)	{	// .S
	 // Print All items of A-stack as int32 values
	int32	i,deep= ADEEP;
	    printf("deep= %5ld \n", deep);
	for (i=deep-1; i>=0; i--)
		printf("    = %5ld : %10ld \n", i, AITEM(i));
 }
 void _PrTOS (void)	{	// TOS
	 // Print Dssp-String (which have not be terminated by 0) :
	 // SubTop = Dssp-address of Dssp-string
	 // Top    = length of DSSP-string 
	 sptr pCh;
	 int32 len,i;
	 len = ATPOP;
	 pCh = (sptr)ADR_MB(ATPOP); // real Address of DSSP-string
	 for ( i=0; i<len; i++ ) {
#ifndef Dos386
		putchar(pCh[i]);
#endif
#ifdef Dos386
		putch(pCh[i]);
#endif
	 }
 }

void _PrSx (void)	{	// .SX
	 // Print All items of A-stack as 16-based values
	int32	i,deep= ADEEP;
	    printf("deep= %5ld \n", deep);
	for (i=deep-1; i>=0; i--)
		printf("    = %5ld : %#10lX \n", i, AITEM(i));
 }

 void _TIS (void)	{	// .TIS
	 // Input Dssp-String (which have not be terminated by 0) :
	 // SubTop = Dssp-address of Dssp-string
	 // Top    = length of DSSP-string 
	 // AFTER: Top = length of string, which has been read (without \n)
	 sptr pCh;
	 int32 len,i; byte Ch;
	 len = ATPOP;
	 pCh = (sptr)ADR_MB(ATPOP); // real Address of DSSP-string (without \n)
	 i=0;
	 while (i<len) {
		Ch= getchar();
		if (Ch=='\n')	break;
		pCh[i]= Ch;
		i++;
	 }
	 APUSH( i );
 }
/*********************************************************/
/******       SLOVAR (DICTIONARY)   OPERATIONS     *******/
/*********************************************************/

#define WORDLen	(33)
#define	SP	(' ')

pSLOVO	pSlvr;	/* pointer to SLOVAR as array [] of SLOVO */	

char	FWORD [WORDLen] ; /* WORD for find */
char Ch;	/* current character */
bool	NewLine; /* =TRUE, if new line */

 void	InitSlvr ()	{ // Prepare Slovar 
	 pSlvr=&SLOVAR[0]; /* pointer to SLOVAR table */
 }

 int32	FindWord ()	{	// find  FWORD in dictionary *pSlvr
	// Result = command number for word, if it was found
	//        = or cmd_ (-1) , if not found
	pSLOVO	pSlv; /* pointer to SLOVO */
	pSlv=pSlvr;
	while (   (pSlv->cmdAdr!=cmd_) 
		    && (strcmp(pSlv->pname,FWORD)!=0) ) 
		pSlv++;   
	return(pSlv->cmdAdr);
 }  // FindWord

 void	InitCh() { // Empty current character
	 Ch=SP; NewLine=TRUE;
 } 

 void	GetCh() {	// read next char and transform it
	Ch=getchar();
	if (Ch=='\t') Ch=SP;
	if (Ch=='\r') Ch=SP;
	if (Ch=='\n') {
		NewLine= TRUE;
		Ch= SP;
	}
 } // GetCh

 void	GetWord ()	{	// read next word  in FWORD 
	// Word is the sequence of characters between spaces
	//  or equal symbols (tabs, lf, cr )
	int32	i; 
	i=0;
	
	if (NewLine) {
		printf("\n* "); NewLine=FALSE;
	}
	// skip spaces before WORD
	while (Ch==SP) {
		GetCh();
		if (NewLine) {
				printf("\n? "); NewLine=FALSE;
		}
	}
	// get chars of WORD until next space
	do { 
		FWORD[i]=Ch; i++;
		GetCh();
	} while (Ch!=SP);
	
	FWORD[i]='\0';

 } // GetWord

 void	PutWord ()	{	// put FWORD 
	printf(FWORD);
	printf(" ");
 } // PutWord

 /*----------- DSSP OPERATIONS with SLOVAR ---------*/

 void	_GetWord (void)		{	// GETWORD 
	GetWord();
 }  

 void	_FindWord (void)	{	// FINDWORD 
	APUSH(FindWord());
 } 

 void	_PutWord (void)		{	// PUTWORD 
	PutWord();
 } 

 void	_UnWord (void)		{	// UNWORD 
	int32 val;
	val=atol(FWORD);
	if (val!=0) {
		AT=(val);
	} else {
		ADEL;	// pop unknown command (-1)
		printf("\n unknown word ");
		PutWord();
		printf("\n");
	}
 } 

/*********************************************************/
/******           TEST   OPERATIONS                *******/
/*********************************************************/

 void _N (void) { 
 	printf("\n : ");
 }
 void _A0 (void) { 
 	printf(" A0,");
 }
 void _A1 (void) { 
 	printf(" A1,");
 }
 void _A2 (void) { 
 	printf(" A2,");
 }
 void _A3 (void) { 
 	printf(" A3,");
 }
 void _A4 (void) { 
 	printf(" A4,");
 }
 void _A5 (void) { 
 	printf(" A5,");
 }
 void _A6 (void) { 
 	printf(" A6,");
 }
 void _E (void) { 
 	printf(" ; \n ");
 }

#define TESTADR	(START_DATA)
 void _TestAdr (void) { // TESTADR
	 int32	Adr= (int32) DSSPADR(DsspMem+TESTADR);
 	printf(" Address %10d", Adr);
	APUSH(Adr);
 }

/*********************************************************/
/******         TEST COMMAND BODIES                *******/
/*********************************************************/

 cmnd CmndBody_MAINLOOP [ ] = {  /* body of MAINLOOP  */
	 // : MAINLOOP  LOOP EXECWORD HALT ;
  	 INTELO, cmd_Loop, cmd_EXECWORD, cmd_Halt, cmd_K
 };

 cmnd CmndBody_EXECWORD [ ] = {  /* body of EXECWORD */
	 // : EXECWORD 
	 //       GETWORD [CMND] FINDWORD
	 //       C BR- UNWORD EXEC ;
  	 INTELO,
	 cmd_GetWord, cmd_FindWord,
	 cmd_C, cmd_BrMi, cmd_UnWord, cmd_Exec,
	 cmd_K
 };
 
 cmnd CmndBody_AA [ ] = {  /* body of the AA */
 	 INTELO,
	 cmd_N,
 	 cmd_GetAdr, cmd_A1, cmd_Exec,
	 cmd_Cnst1, cmd_BrPl, cmd_A2, cmd_A3, 
	 cmd_Cnst0, cmd_BrPl, cmd_A3, cmd_A4, 
	 cmd_E,	 cmd_K
 };

 cmnd CmndBody_A [ ] = {  /* body of the A */
  	 INTELO,
	 cmd_N, cmd_A0, cmd_AA,
	 cmd_A5, cmd_A6, cmd_E,
	 cmd_K
 };
 
 cmnd CmndBody_Test [ ] = {  /* Test body */
	 cmd_A, cmd_Halt
 };

 cmnd CmndBody_B [ ] = {  /* body of the Br */
 	 INTELO,   cmd_N ,
	 cmd_Br, 
		cmd_Cnst0, cmd_A0,
		cmd_Cnst1, cmd_A1,
		cmd_Cnst2, cmd_A2,
		cmd_Else,  cmd_A3,
		cmd_Nop, cmd_A4,
	 cmd_GetPC, cmd_SetPC,
	 cmd_E, cmd_K
 };

 cmnd CmndBody_BrPl [ ] = {  /* body of the BrPl */
 	 INTELO,   cmd_N ,
	 //cmd_Br0, 
	 //cmd_BrMi,
	 cmd_BrPl,
		cmd_A1,
		cmd_A0,
	 cmd_E, cmd_K
 };

 cmnd CmndBody_BIf0 [ ] = {  /* body of the If0 */
 	 INTELO,   cmd_N ,
	 cmd_If0, 
	 //cmd_IfMi,
	 //cmd_IfPl,
		cmd_A1,
	 cmd_A0,
	 cmd_E, cmd_K
 };


 cmnd CmndBody_BrS [ ] = {  /* body of the BrS */
 	 INTELO,   cmd_N ,
	 cmd_BrS, 
		cmd_A2,
		cmd_A0,
		cmd_A1,
	 cmd_E, cmd_K
 };

/*********** TEST FOR VARs and VECTORs *****************/

       /*----- Test Long Var X ----*/

#define	ADRVAR_X	(START_DATA)
  cmnd VarBody_X[ ] = {  /* body of the Var X */
	cmd_GETADR, // 0, // global data
//  cmd_VARX -->
	VARTELO,
	cmd_LVARTYPE,	// pointer to LVAR Type descriptor
	(ADRVAR_X),// address of Var X as offset in DSSP-memory
 };
  cmnd Body_SX [ ] = {  /* body of !X */
	 INTELO,
	 cmd_VARST, cmd_VARX,
	 cmd_K
 };
  cmnd Body_AX [ ] = {  /* body of 'X */
	 INTELO,
	 cmd_VARADR, cmd_VARX,
	 cmd_K
 };

       /*----- Test Long Vector LV --*/

#define	ADRVCTR_V	(START_DATA)
#define	  LIMIT_V	(10)
  cmnd VctrBody_V [ ] = {  /* body of the Long Vector V */
	cmd_GETADR, // 0, // global data
//  cmd_VCTRV -->
 	VARTELO,
	cmd_LVCTRTYPE,	// pointer to VCTR Type descriptor
	(ADRVCTR_V),// address of Vector V as offset in DSSP-memory
	LIMIT_V,		// number of items in vector V
	0 // reserv
 };
  cmnd Body_SV [ ] = {  /* body of !LV */
	 INTELO,
	 cmd_VARST, cmd_VCTRV,
	 cmd_K
 };
  cmnd Body_AV [ ] = {  /* body of 'LV */
	 INTELO,
	 cmd_VARADR, cmd_VCTRV,
	 cmd_K
 };

       /*----- Test Word Vector WV --*/

#define	ADRVCTR_W	(START_DATA)
#define	  LIMIT_W	(20)
  cmnd VctrBody_W [ ] = {  /* body of the Word Vector W */
	cmd_GETADR, // 0, // global data
//  cmd_VCTRW -->
 	VARTELO,
	cmd_WVCTRTYPE,	// pointer to VCTR Type descriptor
	(ADRVCTR_W),// address of Vector W as offset in DSSP-memory
	LIMIT_W,		// number of items in vector W
	0 // reserv
 };
  cmnd Body_SW [ ] = {  /* body of !WV */
	 INTELO,
	 cmd_VARST, cmd_VCTRW,
	 cmd_K
 };
  cmnd Body_AW [ ] = {  /* body of 'WV */
	 INTELO,
	 cmd_VARADR, cmd_VCTRW,
	 cmd_K
 };

       /*----- Test Byte Vector BV --*/

#define	ADRVCTR_BV	(START_DATA)
#define	  LIMIT_BV	(40)
  cmnd VctrBody_BV [ ] = {  /* body of the Byte Vector BV */
	cmd_GETADR, // 0, // global data
//  cmd_VCTRBV -->
 	VARTELO,
	cmd_BVCTRTYPE,	// pointer to VCTR Type descriptor
	(ADRVCTR_BV),// address of Vector BV as offset in DSSP-memory
	LIMIT_BV,	 // number of items in vector BV
	0 // reserv
 };
  cmnd Body_SBV [ ] = {  /* body of !BV */
	 INTELO,
	 cmd_VARST, cmd_VCTRBV,
	 cmd_K
 };
  cmnd Body_ABV [ ] = {  /* body of 'BV */
	 INTELO,
	 cmd_VARADR, cmd_VCTRBV,
	 cmd_K
 };

       /*----- Test Mechanism of Exception Handling --*/

 cmnd CmndBody_TestSit [ ] = {  /* Test body of Sit descriptor */
	 INTELO,
	 cmd_Raise_, cmd_A0,
	 cmd_K
 };

 cmnd CmndBody_TestOn [ ] = {  /* Test body for ON,EON,_RAISE */
	 INTELO,
	 cmd_A4,
	 //cmd_Cnst, cmd_ANY, cmd_NtfOn, cmd_Reac, 
	 // cmd_EOn, cmd_ANY, cmd_Reac, cmd_Nop
	 cmd_EOn, cmd_Sit, cmd_Reac, 
	 // cmd_Nop, cmd_Nop, cmd_Nop, cmd_Nop
	 cmd_A5,
	 cmd_TEST_RP,
	 //cmd_Sit,
	 //cmd_Cnst,
	 //cmd_Sit,
	 //cmd_Raise,
	 cmd_A4,
	 cmd_K
 };
  
 cmnd CmndBody_TestReac [ ] = {  /* Test body of Reac */
	 INTELO,
	 cmd_A6, cmd_ReRaise,
	 cmd_K
 };

 cmnd CmndBody_TestDo [ ] = {  /* Test body for DO */
	 INTELO,
	 cmd_A1,
	 cmd_Cnst, 5,
	 cmd_Cnst, 9, /* -1, */
	 cmd_Do, cmd_TEST_STEP, 
	 cmd_A3, cmd_K
 };

 cmnd CmndBody_TestRp [ ] = {  /* Test body for RP */
	 INTELO,
	 cmd_A1,
	 cmd_Cnst, 5,
	 cmd_Rp, cmd_TEST_STEP, 
	 cmd_A3,
	 cmd_K
 };

 cmnd CmndBody_TestStep [ ] = {  /* Test body for one step of loop */
	 INTELO,
	 cmd_NOn, cmd_Sit1, cmd_Reac, 
	 cmd_A2,
	 cmd_Sub1, 
	 cmd_C, cmd_Cnst2, cmd_Sub, 
	 cmd_C, cmd_ExMi,
	 cmd_If0, cmd_Sit,
	 cmd_K
 };

        /*----- Test Long Const CX ----*/
  cmnd CnstBody_CX[ ] = {  /* body of Const CX */
	CNSTBODY,
	5	// the const value
 };
