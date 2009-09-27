/*********************************************************/
/***     KERNEL for DSSP INTERPRETER  III VARIANT      ***/
/***  -----------------------------------------------  ***/
/***         PREPARATION of DSSP-INTERPRETATION        ***/
/***  -----------------------------------------------  ***/
/***         started by Burtsev A.A.  23 march 1998    ***/
/***              last modification   17 may   1998    ***/
/*********************************************************/

#define	BODYSIZE(X)	(sizeof(X)/sizeof(X[0]))
#define	BODYADR(X)	(&(X)[0])

/***----      FORMING  COMMAND MEMORY              ---***/

 // Form Command Memory, according to SLOVAR
 // Remember!! if SLOVO contains NumFunc==fn_NO, it means that 
 // this value have NOT (!) to be placed in Command memory 
 void	FormCmndSlvr ()	{	
	pSLOVO	pSlv; /* pointer to SLOVO */
	bptr	pCMem = CmdMem;
	pSlv = pSlvr;
	while (pSlv->cmdAdr!=cmd_) {
	  if (pSlv->NumFunc!=fn_NO)  // if not special number-marker
		*(lptr)(pCMem+pSlv->cmdAdr)=(cmnd)(pSlv->NumFunc);
	  pSlv++;   
	}
 }  // FormCmdSlvr

 // form command body of procedure in command memory
 void	FormCmndBody ( int32  CmdOffset, 
		               pcmnd pCmndBody, int32 Len )	{	
	 // copy body of DSSP-procedure to command memory :
	 // copy Len command from address pcmndBody 
	 // to command Memory , starting at vpCMem
	int32	i;
	lptr	lpCMem = (lptr)(CmdMem+CmdOffset);
	for (i=0; i<Len; i++) 
		lpCMem[i]=pCmndBody[i];
 }

/***----          INIT SYSTEM AREA              ---***/

 void InitSysArea () {	// Initiation of system area

	 // Let's put fn_Halt in begining of DSSP-memory so that
	 // any zero code will be interpreted as HALT DSSP-operation
	 ML(DATADR(0)) = fn_Halt ;	 // !!! FOR SAFETY 

	 // Initiate system variables and system code area
	 ML(DATADR(Adr_OPSB)) = OPSB ;
	 ML(DATADR(Adr_CSB )) = CSB  ;
	 ML(DATADR(Adr_PUSK)) = cmd_MAINLOOP ;
	 ML(DATADR(Adr_PUSK+4)) = cmd_Halt ;
	 ML(DATADR(Adr_OWNBASE)) = OWNBASE ;
	 ML(DATADR(Adr_TOS)) = cmd_PrTOS ; // Default TOS = .TOS (Test TOS)
 }

/***----          INIT COMMAND MEMORY           ---***/

 void InitCmndMem () {	// Initiation of CMEM

	// CmdMem = DsspMem;	
	 /*** System Bodies ***/

	FormCmndSlvr();

	FormCmndBody( cmd_BR1,
		          BODYADR(CmndBody_BR1), BODYSIZE(CmndBody_BR1));
	FormCmndBody( cmd_RP1,
		          BODYADR(CmndBody_RP1), BODYSIZE(CmndBody_RP1));
	FormCmndBody( cmd_DO1,
		          BODYADR(CmndBody_DO1), BODYSIZE(CmndBody_DO1));
	FormCmndBody( cmd_RTLT,
		          BODYADR(CmndBody_RTLT), BODYSIZE(CmndBody_RTLT));
	FormCmndBody( cmd_ANY,
		          BODYADR(CmndBody_AnySit),BODYSIZE(CmndBody_AnySit));
	FormCmndBody( cmd_LVOST,
		          BODYADR(CmndBody_LVOST), BODYSIZE(CmndBody_LVOST));

	FormCmndBody( adr_LVARTYPE,
		          BODYADR(LVarType), BODYSIZE(LVarType) );
	FormCmndBody( adr_AVARTYPE,
		          BODYADR(AVarType), BODYSIZE(AVarType) );
	FormCmndBody( adr_WVARTYPE,
		          BODYADR(WVarType), BODYSIZE(WVarType) );
	FormCmndBody( adr_IVARTYPE,
		          BODYADR(IVarType), BODYSIZE(IVarType) );
	FormCmndBody( adr_BVARTYPE,
		          BODYADR(BVarType), BODYSIZE(BVarType) );
	FormCmndBody( adr_SBVARTYPE,
		          BODYADR(SBVarType), BODYSIZE(SBVarType) );


	FormCmndBody( adr_VCTRTYPE,
		          BODYADR(VctrType), BODYSIZE(VctrType) );
	FormCmndBody( adr_ARRTYPE,
		          BODYADR(ArrType), BODYSIZE(ArrType) );
	FormCmndBody( adr_STRUCTTYPE,
		          BODYADR(StructType), BODYSIZE(StructType) );
	FormCmndBody( adr_STRTYPE,
		          BODYADR(StrType), BODYSIZE(StrType) );


	/**** Test Bodies *****/

	FormCmndBody( cmd_EXECWORD,
		          BODYADR(CmndBody_EXECWORD),
				  BODYSIZE(CmndBody_EXECWORD) );
	FormCmndBody( cmd_MAINLOOP,
		          BODYADR(CmndBody_MAINLOOP),
				  BODYSIZE(CmndBody_MAINLOOP) );

#ifdef	TEST_VERS	

	FormCmndBody( cmd_A,
		          BODYADR(CmndBody_A), BODYSIZE(CmndBody_A) );
	FormCmndBody( cmd_AA,
		          BODYADR(CmndBody_AA), BODYSIZE(CmndBody_AA) );
	FormCmndBody( cmd_Test,
		          BODYADR(CmndBody_Test),BODYSIZE(CmndBody_Test));
	FormCmndBody( cmd_B,
		          BODYADR(CmndBody_B), BODYSIZE(CmndBody_B) );

	FormCmndBody( adr_X,
		          BODYADR(VarBody_X), BODYSIZE(VarBody_X) );
	FormCmndBody( cmd_SX,
		          BODYADR(Body_SX), BODYSIZE(Body_SX) );
	FormCmndBody( cmd_AX,
		          BODYADR(Body_AX), BODYSIZE(Body_AX) );

	FormCmndBody( adr_V,
		          BODYADR(VctrBody_V), BODYSIZE(VctrBody_V));
	FormCmndBody( cmd_SV,
		          BODYADR(Body_SV), BODYSIZE(Body_SV) );
	FormCmndBody( cmd_AV,
		          BODYADR(Body_AV), BODYSIZE(Body_AV) );

	FormCmndBody( adr_W,
		          BODYADR(VctrBody_W), BODYSIZE(VctrBody_W));
	FormCmndBody( cmd_SW,
		          BODYADR(Body_SW), BODYSIZE(Body_SW));
	FormCmndBody( cmd_AW,
		          BODYADR(Body_AW), BODYSIZE(Body_AW));

	FormCmndBody( adr_BV,
		          BODYADR(VctrBody_BV), BODYSIZE(VctrBody_BV));
	FormCmndBody( cmd_SBV,
		          BODYADR(Body_SBV), BODYSIZE(Body_SBV));
	FormCmndBody( cmd_ABV,
		          BODYADR(Body_ABV), BODYSIZE(Body_ABV));

	FormCmndBody( cmd_Sit,
		          BODYADR(CmndBody_TestSit),BODYSIZE(CmndBody_TestSit));
	FormCmndBody( cmd_TEST_ON,
		          BODYADR(CmndBody_TestOn),BODYSIZE(CmndBody_TestOn));
	FormCmndBody( cmd_Reac,
		          BODYADR(CmndBody_TestReac),BODYSIZE(CmndBody_TestReac));

	FormCmndBody( cmd_TEST_DO,
		          BODYADR(CmndBody_TestDo),BODYSIZE(CmndBody_TestDo));
	FormCmndBody( cmd_TEST_RP,
		          BODYADR(CmndBody_TestRp),BODYSIZE(CmndBody_TestRp));
	FormCmndBody( cmd_TEST_STEP,
		          BODYADR(CmndBody_TestStep),BODYSIZE(CmndBody_TestStep));

	FormCmndBody( cmd_CX,
		          BODYADR(CnstBody_CX), BODYSIZE(CnstBody_CX) );
#endif

 }

/***----       INIT Slovar and Input stream      ---***/
  void InitDsspSlovar (void) {  // Initiate Dssp Slovar
	 InitSlvr ();	// init slovar
	 InitCh();		// init input stream
 }

/***----       DEFAULT PREPARE DSSP MEMORY           ---***/

 void InitDsspMem (void) {
	 
	 // Prepare DsspMemory
	 //DsspMem = (bptr)(&DSSP_MEMORY[0]);
	 DsspMem = AllocDsspMemory(END_MEM);
	 if (DsspMem==NULL) {
		printf ("There is not enough memory for DSSP program \n");
		exit(5);
	 }

	 InitDsspSlovar(); // Slovar  must be ititiated before DsspMem

	 // Init CommandMemory
	 InitCmndMem(); 

	 InitSysArea();

 }
