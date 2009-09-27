/*********************************************************/
/***     KERNEL for DSSP INTERPRETER  III VARIANT      ***/
/***  -----------------------------------------------  ***/
/***         MAIN  INTERPRETATION   OPERATIONS         ***/
/***  -----------------------------------------------  ***/
/***         started by Burtsev A.A.  23 march 1998    ***/
/***              last modification   22 sept  1998    ***/
/*********************************************************/

/*********************************************************/
/******      PROCEDURE INTERPRETATION  OPERATIONS   ******/
/*********************************************************/

void _UnOp (void) {    // unimplemented operation
         printf("\n operation is not implemented yet "); exit(98);
}

void _UndefOp (void) { // undefined operation
         printf("\n operation is not defined yet "); exit(99);
}

void _Nop (void) {     // NOP
}

 //  Halt DSSP-interpretation
 void _Halt  (void) {
        IntrptFlag = -1; // it means interruption number -1
 }

// start new-level interpretation ( BEGIN DSSP-procedure )
void _Int  (void)
{    CPUSH(pc);
     pc = PCADR(cmd+1);
}

// finish current-level interpretation ( END DSSP-procedure )
void _K  (void)
{     CPOP(pc); }

void _Call (void)
{    // CALLTELO
     // cmd->  CALLTELO
     //    +1     ADR  // address of DSSP-procedure
     //    +2    ... // address of this position
     //              // will be in C-STACK
     CPUSH(cmd+2);
     cmd = (lptr)ML(cmd+1);
     EXECMD;
}

/*********************************************************/
/******          CONTROL   OPERATIONS              *******/
/*********************************************************/

void _Exec (void)
{    // EXEC
     APOP(cmd);
     EXECMD;

}

//---------   1-BRANCH Conditional operations   ---------//

 void _If0 (void) {     // IF0  // IF0 PRC
//      printf ("if0 %lx ",AT);
        if (ATPOP==0) {
                GETCMD;
                EXECMD;
        } else {pc++;}// printf (" pc=%lx\n",(long)pc);}
 }

 void _IfMi (void) {    // IF-  // IF- PRC
        if (ATPOP<0) {
                GETCMD;
                EXECMD;
        } else pc++;
 }

 void _IfPl (void) {    // IF+  // IF+ PRC
        if (ATPOP>0) {
                GETCMD;
                EXECMD;
        } else pc++;
 }

//---------   2-BRANCH Conditional operations   ---------//

 void _Br0 (void) {     // BR0  //  BR0 PR1 PR2
        if (ATPOP==0) {
                GETCMD;
                pc++;
        } else {
                pc++;
                GETCMD;
        }// if
        EXECMD;
 }

 void _BrMi (void) {    // BR-  //  BR- PR1 PR2
        if (ATPOP<0) {
                GETCMD;
                pc++;
        } else {
                pc++;
                GETCMD;
        }// if
        EXECMD;
 }

 void _BrPl (void) {    // BR+  //  BR+ PR1 PR2
        if (ATPOP>0) {
                GETCMD;
                pc++;
        } else {
                pc++;
                GETCMD;
        }// if
        EXECMD;
 }

//---------   3-BRANCH Conditional operation    ---------//

 void _BrS (void) {     // BRS  //  BRS PR1 PR2 PR3
        if (AT<0) {
                GETCMD;
                pc+=2;
        } else
        if (AT==0) {
                pc++;
                GETCMD;
                pc++;
        } else {
                pc+=2;
                GETCMD;
        }// if
        ADEL;
        EXECMD;
 } // _BrS

//--------   MANY-BRANCH Conditional operations  --------//

 cmnd CmndBody_BR1 [ ] = {  /* body of the BR1 */
         cmd_Br1
 };

 void _Br (void) {      // BR   //  BR  V1 P1 V2 P2 ... VX PX ELSE P0
        GETCMD;
        CPUSH(pc);
        pc=PCADR(cmd_BR1);
        EXECMD;
 }

void _Br1 (void) {     // BR1
        CPOP(pc);
        if (AITEM(1)==AT)
        {       ADELn(2);
                GETCMD;
                // looking for ELSE
                while (ML(pc)!=cmd_Else) pc++;
                pc +=2; // so that pc points after P0
                EXECMD;
        } else {
                ADEL;
                pc++;
                GETCMD;
                CPUSH(pc);
                pc=PCADR(cmd_BR1);
                EXECMD;
        }
 }

void _Else (void) { // ELSE
        CPOP(pc);
        ADEL;
}

//-------------      LOOP operations     ----------------//

 void _DW (void) {      // DW   //  COND  DW  BODY
        if (ATPOP!=0) {
                GETCMD;
                pc -=3; // so that pc points again to COND position
                EXECMD;
        } else pc++;
 }

 void _DOFor (void) {   // DO-  //  DO-  BODY
         // N DO- P means execute [i] P for i=N,..,1
        if (--AT>=0) {  // decrement parameter and then test it
                cmd= (lptr)ML(pc);
                pc--;   // so that pc points again to DO- position
                EXECMD;
        } else { // terminate loop, if AT  became <0
                pc++;
                ADEL;   // pop parameter
        }
 }

 void _Loop (void) {    // LOOP //  LOOP  BODY
        GETCMD;
        pc-=2;
        EXECMD;
 }

//------- OPERATIONS with CONTROL STACK and BODY --------//

 void _GetPC (void) {   // CPC
         APUSH(csp);
 }


 void _SetPC (void) {   // !PC
         APOP(csp);
 }

 void _GetR  (void) {   // @R
         int32  RVal;
         CPOP (RVal);
         APUSH(RVal);
 }

 void _SetR  (void) {   // !R
         int32  RVal;
          APOP(RVal);
         CPUSH(RVal);
 }

 void _GetAdrD (void) { // ''
        // Push to A-stack the following value in body
         APUSH(GETBODYVAL);
 }

 void _GTP (void) { // GTP
        // Push to A-stack the following value in body
        // of that procedure, from where the procedure,
        // including GTP, was called.
        lptr    Rpc; int32 Val;
        Rpc= (lptr)CT; Val=ML((bptr)Rpc);
        Rpc++; CT=(int32)Rpc;
        APUSH(Val);
 }

/*********************************************************/
/******         COMMON A-STACK OPERATIONS          *******/
/*********************************************************/

//----------- PUSH const values in stack   ---------------//

void _Cnst0 (void) { // 0
       APUSH(0);
}

void _Cnst1 (void) { // 1
       APUSH(1);
}

void _Cnst2 (void) { // 2
       APUSH(2);
}

void _Cnst4 (void) { // 4
       APUSH(4);
}

//----------- PUSH COPY values in stack   ---------------//

/* PUSH in stack copy of value from position at deep n */
#define APUSHCOPYn(N)   { AINS; AT= AITEM(N); }

void _C (void) { // C
       APUSHCOPYn(1);
}

void _C2 (void) { // C2
       APUSHCOPYn(2);
}

void _C3 (void) { // C3
       APUSHCOPYn(3);
}

void _C4 (void) { // C4
       APUSHCOPYn(4);
}

 void _CT (void) { // CT
       AT=AITEM(AT);
}

//----------- EXCHANGE values in stack   ---------------//

/* EXCHANGE top and value from position at deep n */
#define AEXCHn(N)       { int32 tmp=AT; AT=AITEM(N); AITEM(N)=tmp; }

void _E2 (void) { // E2
       AEXCHn(1);
}

void _E3 (void) { // E3
       AEXCHn(2);
}

void _E4 (void) { // E4
       AEXCHn(3);
}

void _ET (void) { // ET
       int32 n=AT;
       AT=AITEM(1); AITEM(1)=AITEM(n); AITEM(n)=AT;
       ADEL;
}

//----------- Delete values from stack   ---------------//

/* Delete n top values from A-stack with Testing */
#define ADELTn(N)       { if (ADEEP>=(N)) {ADELn(N);} else {/*Error*/} }

void _D (void) { // D
       ADELTn(1);
}

void _D2 (void) { // DD
       ADELTn(2);
}

void _D3 (void) { // DDD
       ADELTn(3);
}

void _D4 (void) { // DDDD
       ADELTn(4);
}

/* Delete subtop from A-stack with Testing */
void _E2D (void) { // E2D
       if (ADEEP>=(2)) {
               AITEM(1)=AT; ADEL;
       } else {/*Error*/}
}

/* Make A-stack empty, delete all items from it */
void _DS (void) { // DS
       AEMPTY;
}

/* Push in A-stack value of Deep of A-stack */
void _Deep (void) { // DEEP
       int32 n=ADEEP; APUSH(n);
}

/*********************************************************/
/******       ARITHMETIC STACK OPERATIONS          *******/
/*********************************************************/

/* Set TOP of A-stack equal const value */
#define ATSET(V)        { AT = (V); }

void _T0 (void) { // T0
       ATSET(0);
}
void _T1 (void) { // T1
       ATSET(1);
}
void _TN1 (void) { // T-1
       ATSET(-1);
}
void _T2 (void) { // T2
       ATSET(2);
}
void _T4 (void) { // T4
       ATSET(4);
}

 /* Add Top and SubTop of A-stack */
void _Add (void) { // +
       AITEM(1) += AT ; ADEL;
}

/* Add const value to Top of A-stack */
#define ATADD(V)        { AT += (V); }

void _Add1 (void) { // 1+
       ATADD(1);
}

void _Add2 (void) { // 2+
       ATADD(2);
}

void _Add3 (void) { // 3+
       ATADD(3);
}

void _Add4 (void) { // 4+
       ATADD(4);
}

 /* Subtract Top from SubTop of A-stack */
void _Sub (void) { // -
       AITEM(1) -= AT ; ADEL;
}

/* Subtract const value from Top of A-stack */
#define ATSUB(V)        { AT -= (V); }

void _Sub1 (void) { // 1-
       ATSUB(1);
}

void _Sub2 (void) { // 2-
       ATSUB(2);
}

void _Sub3 (void) { // 3-
       ATSUB(3);
}

void _Sub4 (void) { // 4-
       ATSUB(4);
}

void _Neg (void) { // NEG
       AT = -AT ;
}

void _Abs (void) { // ABS
       if (AT<0)
               AT = -AT ;
}

void _Sgn (void) { // SGN
       if (AT<0)       AT=-1; else
       if (AT==0)      AT= 0; else
                               AT= 1;
}

void _Max (void) { // MAX
       if (AT>AITEM(1))
               AITEM(1) = AT ;
       ADEL;
}

void _Min (void) { // MIN
       if (AT<AITEM(1))
               AITEM(1) = AT ;
       ADEL;
}

 /* Multiply Top and SubTop of A-stack */
void _Mult (void) { // *
       AITEM(1) *= AT ; ADEL;
}

 /* Divide SubTop on Top (with remainder) */
void _Div (void) { //  /
        // Integer  division (SubTop / Top), so that
        //  new SubTop= quotient , new Top = remainder
       int32   q;
       q = AITEM(1)/AT;        // quotient;
       AT = AITEM(1)-AT*q;     // remainder
       AITEM(1) = q ;
}

/*********************************************************/
/******        LOGICAL  and BIT OPERATIONS         *******/
/*********************************************************/

void _Lt (void) { // <
       int32 tmp= ATPOP;
       AT = (AT<tmp);
}

void _LE (void) { // <=
       int32 tmp= ATPOP;
       AT = (AT<=tmp);
}

void _Eq (void) { // =
       int32 tmp= ATPOP;
       AT = (AT==tmp);
}

void _Gt (void) { // >
       int32 tmp= ATPOP;
       AT = (AT>tmp);
}

void _GE (void) { // >=
       int32 tmp= ATPOP;
       AT = (AT>=tmp);
}

void _NE (void) { // !=
       int32 tmp= ATPOP;
       AT = (AT!=tmp);
}
void _Seg (void) { // SEG
       // [ X, A, B ] SEG [TRUE/FALSE
       //  does X in [A,B] or not ? ( A<= X <= B ? )
       int32 X= AITEM(2);
       AITEM(2)= (AITEM(1)<=X) && (X<=AT);
       ADELn(2);
}

void _Not (void) { // NOT
       AT = (AT==0);
}

void _Inv (void) { // INV
       AT = ~AT ;
}

void _And (void) { // &
       AITEM(1) &= AT ; ADEL;
}

void _Or (void) { // &0
       AITEM(1) |= AT ; ADEL;
}

void _Xor (void) { // '+'
       AITEM(1) ^= AT ; ADEL;
}

void _Shl (void) { // SHL
       AT = (AT<<1)&(0xFFFFfffe) ;
}

void _Shr (void) { // SHR
       AT = (AT>>1)&(0x7FFFffff) ;
}

void _Sht (void) { // SHT
       if (AT<0)       AITEM(1) >>= (-AT);
       else            AITEM(1) <<= ( AT);
       ADEL;
}

void _SwB (void)       { // SWB
       int32   tmp=(AT)&(0x0000FFFFL);
       tmp=((tmp&0xFFL)<<8)|(tmp>>8);
       AT = (AT)&(0xFFFF0000L)|tmp;
}

void _SwW (void)       { // SWW
        AT=(AT<<16)|(AT>>16)&(0x0000FFFFL);
}

void _Lo (void)        { // LO
        AT &= (0x0000FFFFL);
}

void _Hi (void)        { // HI
        AT=(AT>>16)&(0x0000FFFFL);
}

void _SetLo (void)     { // SETLO
       int32   tmp=(ATPOP)&(0x0000FFFFL);
       AT = (AT)&(0xFFFF0000L)|tmp;
}

void _SetHi (void)     { // SETHI
       int32   tmp=(ATPOP)&(0x0000FFFFL);
       AT = (AT)&(0x0000FFFFL)|(tmp<<16);
}

void _SgX (void)       { // SGX
       AT = ((AT<<16)>>16);
}


/*********************************************************/
/******           DATA   OPERATIONS                *******/
/*********************************************************/

void _Cnst (void) { // CNST
       // Push the following value to arithmetic stack
       APUSH(GETBODYVAL);
}

//----------- GET VALUEs from DSSP memory ---------------//

void _LVal (void) { // @L
       // Get long value at address of Top
       AT= ML(AT);
}

void _WVal (void) { // @W
       // Get word value at address of Top
       AT= MW(AT);
}

void _BVal (void) { // @B
       // Get byte value at address of Top
//      printf("%lx\n",AT);
        AT= MB(AT);
//      printf("@b=%lx\n",AT);
}

void _CLVal (void) { // C@L
       _C(); _LVal();
}

void _CWVal (void) { // C@W
       _C(); _WVal();
}

void _CBVal (void) { // C@B
       _C(); _BVal();
}

//----------- SET VALUEs in DSSP memory ---------------//

void _TL (void) { // !TL
       // Put SubTop as long value in location at address of Top
       ML(AT)=AITEM(1);
       ADELn(2);
}

void _TW (void) { // !TW
       // Put SubTop as word value in location at address of Top
       MW(AT)=(word)(AITEM(1));
       ADELn(2);
}

void _TB (void) { // !TB
       // Put SubTop as byte value in location at address of Top
       MB(AT)=(byte)(AITEM(1));
       ADELn(2);
}

void _TL_ (void) { // <!TL
       // Put Top as long value in location at address of SubTop
       ML(AITEM(1))=AT;
       ADELn(2);
}

void _TW_ (void) { // <!TW
       // Put Top as word value in location at address of SubTop
       MW(AITEM(1))=(word)AT;
       ADELn(2);
}

void _TB_ (void) { // <!TB
       // Put Top as byte value in location at address of SubTop
       MB(AITEM(1))=(byte) AT;
       ADELn(2);
}

//----------- GET and SET BITs in DSSP memory ---------------//

void _ValBit (void) { // @BI
       // Get bit number Top at address of SubTop
       int32 maska;
       int32 A,n;
       n=ATPOP;
       A=AT;
//      printf("A=%lx ",(long)A);
       A+=(int32)(n/8);
       maska= 1<<(n-((int32)(n/8))*8);
//      printf("A=%lx n=%lx maska=%lx @=%lx\n",(long)A,(long)n,(long)maska,(long)MB(A));
       AT= (MB(A)&maska)!=0;
}


void _StBit (void) { // !BI
       // Set bit number Top at address of SubTop,
       // according to logical value in AITEM(2)
       int32 maska;
       int32 n=AT;
       int32 A = (AITEM(1));
       A+=(int32)(n/8);
       maska= 1<<(n-((int32)(n/8))*8);
       if (AITEM(2))
                       MB(A)|=maska;
       else    MB(A)&=~maska;
       ADELn(3);
}

void _StBit1 (void) { // !BI1
       // Set bit number Top at address of SubTop
       int32 maska;
       int32 n=AT;
       int32 A = (AITEM(1));
       A+=(int32)(n/8);
       maska= 1<<(n-((int32)(n/8))*8);
       MB(A)|=maska;
//      int32 maska = 1<<(AT);
//      int32 A = (AITEM(1));
//      ML(A)|=maska;
       ADELn(2);
}

void _StBit0 (void) { // !BI0
        // Set bit number Top at address of SubTop
//      int32 maska = 1<<(AT);
//      int32 A = (AITEM(1));
//      ML(A)&=~maska;
       int32 maska;
       int32 n=AT;
       int32 A = (AITEM(1));
       A+=(int32)(n/8);
       maska= 1<<(n-((int32)(n/8))*8);
       MB(A)&=~maska;
       ADELn(2);
}

//------- SET VALUE for DATA BLOCK in DSSP memory ------//

void _SetB (void) { // !!!MB
       // [ V, A, n ] !!!MB [ ]
       // Assign value V to every byte of block
       // for n bytes, starting at A
       int32 A, i, V;
       i= AT-1;
       A= AITEM(1);
       V= AITEM(2);
       for ( ; i>=0; i-- )
               MB(A+i)= (byte)V;
       ADELn(3);
}

void _SetW (void) { // !!!MW
       // [ V, A, n ] !!!MW [ ]
       // Assign value V to every word of block
       // for n words, starting at A
       int32 A, i, V;
       A= AITEM(1);
       V= AITEM(2);
       i= AT-1;
       for ( ; i>=0; i-- )
               MW(A+i*2)= (word)V;
       ADELn(3);
}

void _SetL (void) { // !!!ML
       // [ V, A, n ] !!!ML [ ]
       // Assign value V to every dword of block
       // for n dwords, starting at A
       int32 A, i, V;
       A= AITEM(1);
       V= AITEM(2);
       i= AT-1;
       for ( ; i>=0; i-- )
               ML(A+i*4)= V;
       ADELn(3);
}

//-------     COPY DATA BLOCK in DSSP memory      ------//

void _MoveB (void) { // !SB
       // [ A, n, B ] !SB [ ]
       // Move (copy) block n bytes, starting at A
       // to new place, starting at B;
       // ( overlapping is possible )
       int32 A, n, B, i;
       B= (dword)AT;
       n= AITEM(1);
       A= (dword)AITEM(2);
       // copy n bytes
       if (B<A) {      // copy n bytes to left ( <= )
               // start with first left byte
               for ( i=0; i<n; i++ )
                       MB(B+i)= MB(A+i);
       } else {        // copy n bytes to right ( => )
               // start with last right byte
               for ( i=n-1; i>=0; i-- )
                       MB(B+i)= MB(A+i);
       }
       ADELn(3);
}

void _MoveW (void) { // !SW
        // Move (copy) block of words
        AITEM(1)<<=1; _MoveB();
}

void _MoveL (void) { // !SL
        // Move (copy) block of long values (dwords)
        AITEM(1)<<=2; _MoveB();
}

//----- SEARCH VALUE in DATA BLOCK in DSSP memory -----//

void _SrchB (void) { // SRCHB
       // [V, A, n ] SRCHB [i]
       // search: find byte value B in byte block,
       // from address A to A+n-1 (n>0) or
       // from address A to A-n+1 (n<0)
       // result (i) - offset from A or n, if not found
       int32 n, i; bptr A; byte V;
       n= ATPOP;
       A= (bptr)ATPOP;
       V= (byte) AT;
       i=0;
       if (n>0) {
               while ( (i!=n)&&(MB(A+i)!=V) ) i++ ;
       } else {
               while ( (i!=n)&&(MB(A+i)!=V) ) i-- ;
       }
       AT= i;
}      // _SrchB

/*********************************************************/
/*****       INTERPRETATION  OF DSSP-PROGRAM         *****/
/*********************************************************/

void _Restart (void) { // RESTART
       opsp= opsb;
       csp= csb;
       InitMS(); // init mechanism  of situations
       pc = (lptr)DATADR(START_PC);

       APUSH(1);       // AT:=1 , if Not first Start
}//_Restart

void Interpretation (void) {
       // CmdMem = DsspMem;
       // init stacks
       OwnBase = (int32)DATADR(ML(DATADR(Adr_OWNBASE )));
       opsb= (lptr)DATADR(ML(DATADR(Adr_OPSB)));
       csb = (lptr)DATADR(ML(DATADR(Adr_CSB )));
       opsp= opsb;

       csp= csb;
       InitMS(); // init mechanism  of situations
#ifdef interrupts
       InitIntrMh();  // Init Interrupt Mehanism
#endif
       pc = (lptr)DATADR(START_PC);

       APUSH(0);       // AT:= 0 , if First Start

       StopFlag   = FALSE ;
       IntrptFlag = FALSE ;
       // main interpretation loop with interruption :
       while (StopFlag==FALSE) {
               INTLOOP;
               // invoke interrupt handler, if it have occured
          _Interrupt ();
       }// while
}// Interpretation

int32 ExecDpfCode (lptr StartPC) {
// execute threaded code, pointed by StartPC
// this code must be finished by HALT operation
// opsp points to operand stack
// csp  points to control stack
       int32 RetVal ; // returns Completion Code (StopFlag)
       // local vars  for saving DSSP registers :
       lptr    Saved_pc ;
       lptr    Saved_cmd;
       bool    Saved_StopFlag, Saved_IntrptFlag ;
       // What else must be saved ?

       // let's save some DSSP registers :
       Saved_pc = pc;
       Saved_cmd = cmd;
       Saved_StopFlag = StopFlag;
       Saved_IntrptFlag= IntrptFlag;

       pc = StartPC;
       StopFlag   = FALSE ;
       IntrptFlag = FALSE ;
       // main interpretation loop with interruption :
       while (StopFlag==FALSE) {
               INTLOOP;
               // invoke interrupt handler, if it have occured
          _Interrupt ();
       }// while
       RetVal = StopFlag;

       // restore DSSP registers, which has been saved :
       pc = Saved_pc;
       cmd= Saved_cmd;
       StopFlag= Saved_StopFlag;
       IntrptFlag = Saved_IntrptFlag;

       return (RetVal);
}// ExecDpfCode

/*********** TESTS for ExecDpfCode **************/

void _ExecDSSP (void) { // Exec DSSP inside DSSP !!!
        int32 CCode;
        APUSH(1);
        CCode = ExecDpfCode((lptr)START_PC);
        APUSH(CCode);
}

void _ExecDSSP1(void) { // Exec DSSP PROCEDURE, terminating by HALT
        int32 CCode;
        lptr   Start;
        Start = (lptr)ATPOP; Start++;
        CCode = ExecDpfCode(Start);
        APUSH(CCode);
}

/************************************************/
void isk()
{
        int i,notf;
        char *imch;
        unsigned char *az,*wim;
        long ba;
        long im1,im2,count;
        az=(unsigned char*)(DsspMem+ATPOP);
        wim=(unsigned char*)(DsspMem+Adr_SWIM);
        for(;;)
        {
                if (*(long*)az==0) break;
                if (*(char*)(az+3)<0)
                        az=az+(*(long*)az);
                else
                {
                        im1=*(long*)az;
                        imch=(char*)&im1;
                        *(imch+3)&=0x04;
                        *(imch+2)&=0x07;
                        im2=*(long*)(wim);
                        if (im1==im2)
                        {
                                notf=0;
                                count=((long)*(az+2))&0x07;
                                for(i=1;i<=(count);i++)
                                        if (*(long*)(az-i*4)!=*(long*)(wim-(i)*4)) {notf=1;break;}
                                if (!notf) {break;}
                                az=az-((count)*4+4);
                                az-=4;
                        }
                        else
                        {
                                count=((long)*(az+2))&0x07;
                                az=az-((count)*4+4);
                                az-=4;
                        }
                }
        }

        if (*(long*)az==0) ba=0;
        else
                ba=*(long*)(az-((count)*4+4));
        AINS;AT=(long)(az-DsspMem);
        AINS;AT=ba;
}

void _Real(void) //DFADR DSSP Address -> Fisical Address
{ AT=AT+(int32)DsspMem;
}

void _DsspA(void) //FDADR Fisical Address -> DSSP Address
{ AT=AT-(int32)DsspMem;
}