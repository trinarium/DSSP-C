/*********************************************************/
/***     KERNEL for DSSP INTERPRETER  III VARIANT      ***/
/***  -----------------------------------------------  ***/
/***   OPERATIONS with TRAP RECORDs in CONTROL stack   ***/
/***  -----------------------------------------------  ***/
/***         started by Burtsev A.A.  23 march 1998    ***/
/***              last modification   17 may   1998    ***/
/*********************************************************/

/*********************************************************/
/****  STRUCTURE of TRAP RECORDs in CONTROL stack    *****/
/*********************************************************/

#define IDTRAP  (0)     // the value to recognize the trap record
  typedef       struct  _TRAPREC {      // TRAP-record in Control stack
        lptr    PopTProc;       // pointer to body, which pop the trap
        int32   IdTrap;         // Identifier of trap record (=IDTRAP)
        int32   TagLen;         //  Tag  | Len
                                                // other parameters
 } TRAPREC, *pTRAPREC ;

 // For functions, which is looking for the Trap rec in C-stack
 lptr   csPtr;  // points to position, from which to start find
                                // and then to position of IDTRAP, when found
 lptr   TrapPtr; // points to TagLen position of Trap Rec
                         // before execution of final Trap procedure

#define LTRAP_LEN       (12) // Length for Loop TRap (from TagLen)

 /*----   SVERTKA of CONTROL STACK ----*/

//  execute final procedure of Trap Rec
void ExecFinTrapProc (lptr pTrapPtr) {
        // pTrapPtr points to TagLen position of Trap Rec
        // remember pointer to the trap in TrapPtr for final Trap procedure
        TrapPtr = pTrapPtr;
        // execute Trap procedure
        // assuming, that it defined as address command
        EXECMD_((lptr)ML((lptr)ML(TrapPtr-2)+1));
          // execute it as number function
          // EXECFN(ML((lptr)ML(TrapPtr-2)+1));
}

//  execute final procedures during  svertka of C-stack
void ExecSvertkaCS (void) {
        // find in C-stack between csp ... csPtr
        // every Trap Rec except Loop Trap
        // and execute its final procedure
        int32   TgL;
        lptr    sPtr;
        sPtr = csp;
        while (sPtr<csPtr) {    // looking for Trap
          if (ML(sPtr)!=IDTRAP) {
                        sPtr++;
          } else {  // we've found the position with IDTRAP
                sPtr++; // now sPtr points to TagLen position
                TgL=ML(sPtr);   // TgL=TagLen or parameter of loop
                if (TgL>=0)     {       // This is Loop Trap
                        sPtr= (lptr)( (bptr)sPtr+LTRAP_LEN);
                } else {
                        // execute final procedure of this Trap Rec
                        ExecFinTrapProc (sPtr);
                        // and skip the Trap Rec in C-stack
                        sPtr= (lptr)( (bptr)sPtr+(TgL&0xFF) );
                }
          }
        }// while

}

/*********************************************************/
/******           LOOPs with TRAPs  ( RP , DO )    *******/
/*********************************************************/


 cmnd CmndBody_RP1 [ ] = {  /* body of the RP1 */
         cmd_Rp1
 };

 void _Rp (void) { // RP - repeat
        cmd= (lptr)ML(pc);
        pc++;
        CPUSH(pc);
        CPUSH(cmd);
        CPUSH(0);
        CPUSH(IDTRAP);
        pc=PCADR(cmd_RP1);
        EXECMD;
 }

 void _Rp1 (void) { // RP1 - repeat again
         pc--;  // pc= PCADR(cmd_RP1);
         cmd=(lptr)ML(csp+2);
         EXECMD;
 }

 cmnd CmndBody_DO1 [ ] = {  /* body of the DO1 */
         cmd_Do1
 };

 void _Do (void) { // DO - for i:= 1 to AT do
        GETCMD; //      cmd= (lptr)ML(pc); pc++;
        if (AT>0) {
                CPUSH(pc);
                CPUSH(cmd);
                CPUSH(ATPOP);
                CPUSH(IDTRAP);
                pc=PCADR(cmd_DO1);
                EXECMD;
        } else
                ADEL;
 }

 void _Do1 (void) { // DO1 - DO again
        if (--ML(csp+1)!=0) { // continue
                pc--;   // pc= PCADR(cmd_DO1);
                cmd=(lptr)ML(csp+2);
                EXECMD;
        } else {        // break
                csp += 3;
                CPOP(pc);
        }
 }

 void FindLoopTrap (void) {     // Find Trap of Loop in control stack
        int32   TgL ;
        for (;;) {      // looking for Trap of Loop
                while (ML(csPtr)!=IDTRAP) csPtr++;
                csPtr++;        // now csPtr points to TagLen position
                TgL=ML(csPtr);  // TgL=TagLen or parameter of loop
                if (TgL>=0)     {       // This is Loop Trap
                        break;          // EXIT looking LOOP !!
                } else {        // skip the Trap Rec
                        // execute final procedure of this Trap Rec
                        ExecFinTrapProc (csPtr);        // It is nessecary for S( !!
                        // and skip the Trap Rec in C-stack
                        csPtr= (lptr)( (bptr)csPtr+(TgL&0xFF) );
                }
        }// for
 }//FindLoopTrap

 void _Ex (void) { // EX - Exit from loop with Trap (RP DO)
        csPtr = csp;
        FindLoopTrap();
        // now csPtr points to TagLen or parameter position
        csp= csPtr+2;
                CPOP(pc);
 }

 void _Ex0 (void) {     // EX0  - Exit if AT=0
        if (ATPOP==0)
                _Ex();
 }

 void _ExMi (void) { // EX-     - Exit if AT<0
        if (ATPOP<0)
                _Ex();
 }

 void _ExPl (void) { // EX+     - Exit if AT>0
        if (ATPOP>0)
                _Ex();
 }

/*********************************************************/
/******      RECOVERing of Variables  (LVOST)      *******/
/*********************************************************/

 cmnd   CmndBody_LVOST [] = { cmd_LVost,
   cmd_LVosts // as address command
   // fn_LVosts      // as number function
 };

 void _LVost (void) { // LVost
         // Execute final procedure before svertka
         TrapPtr= csp+1 ;
         _LVosts();
         // Return and Pop TrapRec from stack
         _RtL();        // like Pop analogich Sit TrapRec
 }

 void _LVosts (void) { // LVosts - during SVERTKA
        // Execute final actions before POP Trap Rec from stack
        // Remember, that : !!!
        // TrapPtr points to TagLen position of the Trap Record
        // which have to be deleted after execution of final Trap procedure
        //
        // LVOST TrapRec contains array [1..n] of record items
        // This array is placed in TrapRec just after TagLen location.
        // Every record item of this array contains 3 fields :
        //  1) length of value to be recovered (1,2 or 4 bytes)
        //  2) address of Dssp-memory, to which the value to assign
        //  3) the value, which was saved and have to be assign

        int32   n;      // n=number of values (VARs) to be recovered
        lptr    TPtr; int32 adr,val;
        TPtr=TrapPtr;
        n= ((ML(TPtr++)&0x00FF)-4)/12; // n= (Len-4)/12
        // TPtr points to first item of the array
        for ( ; n>0; n-- ) {    // recover one value
                adr= ML(TPtr+1);
                val= ML(TPtr+2);
                switch ( ML(TPtr) ) {
                        case 1: // byte value
                                MB(adr)= (byte)val;
                                break;
                        case 2: // word value
                                MW(adr)= (int16)val;
                                break;
                        case 4: // long value
                                ML(adr)= (int32)val;
                                break;
                }
                TPtr += 3; // to new record item of the array
        }//for
        // printf (",,,>"); // for debugging
 }


/*********************************************************/
/******     MECHANISM  of EXCEPTION HANDLING       *******/
/*********************************************************/

 cmnd   CmndBody_RTLT [] = { cmd_RtL,
   cmd_RtLs        // as address command
   // fn_RtLs      // as number function
 };

#define STRAP_BIT       (0x0100) // bit-priznak of Trap situation
#define STAG_MASK       (0x0600) // mask for Situation Tag
#define STAG_SHFT       (9)             // shift counter for STAG mask

// Notify TRAPRec
#define NTF     (0)
#define NTF_TAGLEN      (0xFFFF810C)
// TRAPREC NtfTrap = { cmd_RTLT, IDTRAP, NTF_TAGLEN     /* ... */ };

// Escape TRAPRec
#define ESC     (1)
#define ESC_TAGLEN      (0xFFFF8310)
// TRAPREC EscTrap = { cmd_RTLT, IDTRAP, ESC_TAGLEN     /* ... */ };

// Retry TRAPRec
#define RTR     (3)
#define RTR_TAGLEN      (0xFFFF8714)
// TRAPREC RtrTrap = { cmd_RTLT, IDTRAP, RTR_TAGLEN     /* ... */ };

 void _RtL (void) { // RTL
         // Return and Pop TrapRec from stack
         // csp points to IdTrap location; ML[csp+1] = TagLen
         csp = (lptr)( (int32)csp + 4 + (ML(csp+1)&0x00FF) );
         // now csp points to location after TrapRec
        CPOP(pc);
 }

 void _RtLs (void) { // RTLs - during SVERTKA
        // Execute final actions before POP Trap Rec from stack
        // Remember, that : !!!
        // TrapPtr points to TagLen position of the Trap Record
        // which have to be deleted after execution of final Trap procedure
        //
        // printf ("...>"); // for debugging
 }

 /******  DESCRIPTOR of SITUATION ****************/
 //---  INTELO | RAISE_ | EREACT | ; |
 //                                |---- SitPtr
#define SIT_OFFSETB     (3*4)   // because Offset in bytes

 // Descriptor for dummy ANY situation
 cmnd CmndBody_AnySit [ ] = {  /* body of AnySit descriptor */
         INTELO,
         cmd_Raise_, cmd_Nop,
         cmd_K
 };
#define ANYSIT  (cmd_ANY + SIT_OFFSETB)

 lptr   AnySit; // ANY situation pointer

 /************* BOTTOM  TRAP  structure  ******/
 //----: ---| SitPtr | Reac | TagLen | IDTRAP | RTLT | ----
 //  cbs--|     1                3

// Offset field TagLen of Bottom Trap from bottom of C-stack (csb)
#define BTRAP_OFFSET (3)        // 3 dwords
#define BTRAP_SIT    (1)        // 1 dword

 void Put_BTrap (void) {        // Put Bottom Trap in C-Stack
        CPUSH(AnySit);
        CPUSH(cmd_Nop);
        CPUSH(NTF_TAGLEN);
        CPUSH(IDTRAP);
        CPUSH(PCADR(cmd_RTLT));
 }

/***------  SETTING REACTIONs on  SITUATION  --------***/

        //--- MACRO PUSH Situation TRAP to control stack ---

#define PUSH_STRAP(SIT,TAGLEN)  { \
        CPUSH(SIT);                     \
        CPUSH(GETBODYVAL);      \
        CPUSH(TAGLEN);          \
        CPUSH(IDTRAP);          \
        CPUSH(PCADR(cmd_RTLT));         }


  /* Get Address of React proc and push it */

 void _NOn (void) {     // ON - Push Notify Trap to control stack
                                        // for situation following from body
        lptr    sPtr;
        sPtr= (lptr)DATADR(GETBODYVAL+SIT_OFFSETB) ;
        PUSH_STRAP(sPtr,NTF_TAGLEN);
 }

 void _EOn (void) { // EON - Push Escape Trap to control stack
                                        // for situation following from body
        lptr    sPtr;
        sPtr= (lptr)DATADR(GETBODYVAL+SIT_OFFSETB) ;
        CPUSH(opsp);    // Push in TRAPRec current position opsp
        PUSH_STRAP(sPtr,ESC_TAGLEN);
 }

 void _ROn (void) { // RON - Push Retry Trap to control stack
                                        // for situation following from body
        lptr    sPtr;
        sPtr= (lptr)DATADR(GETBODYVAL+SIT_OFFSETB) ;
        CPUSH(pc+1);    // remember position after RON SIT REACT ...
        CPUSH(opsp);    // Push in TRAPRec current position opsp
        PUSH_STRAP(sPtr,RTR_TAGLEN);
 }

 void _NtfOn (void) { // _ON - Push Notify Trap to control stack
                                        // for situation from Top of A-stack
        lptr    sPtr;
        sPtr= (lptr)DATADR(ATPOP+SIT_OFFSETB) ;
        PUSH_STRAP(sPtr,NTF_TAGLEN);
 }

 void _EscOn (void) { // _EON - Push Escape Trap to control stack
                                        // for situation from Top of A-stack
        lptr    sPtr;
        sPtr= (lptr)DATADR(ATPOP+SIT_OFFSETB) ;
        CPUSH(opsp);    // Push in TRAPRec current position opsp
        PUSH_STRAP(sPtr,ESC_TAGLEN);
 }

 void _RtrOn (void) { // _RON - Push Retry Trap to control stack
                                        // for situation from Top of A-stack
        lptr    sPtr;
        sPtr= (lptr)DATADR(ATPOP+SIT_OFFSETB) ;
        CPUSH(pc+1);    // remember position after RON SIT REACT ...
        CPUSH(opsp);    // Push in TRAPRec current position opsp
        PUSH_STRAP(sPtr,RTR_TAGLEN);
 }

/***------  RAISE  SITUATION  --------***/
 lptr   SitPtr; // pointer to situation
 lptr   ReacP;  // address of reaction Procedure

 void FindSitTrap (void) {      // Find Trap of Situation in control stack
        // SitPtr = situation pointer
        int32   TgL ; int16 Len;
        int16   SitCnt; // counter of situation levels in C-stack (eDX)
        SitCnt=0;
        csPtr = csp+1;  // start find from position after top of C-stack
        for (;;) {      // looking for Trap for the situation SitPtr
                // Find Trap Rec in control stack
                while (ML(csPtr)!=IDTRAP) {
                        if (ML(csPtr)==(int32)SitPtr)
                                SitCnt++; // inc Sit level
                        csPtr++;
                }
                csPtr++;        // now csPtr points to TagLen position
                TgL=ML(csPtr);  // TgL=TagLen;
                if (TgL>=0)     {       // This is Loop Trap
                        Len=LTRAP_LEN;  // length (in bytes) for Loop Trap Rec
                } else {
                        Len=(TgL&0xFF);
                        if ((TgL&STRAP_BIT)!=0) {       // this is Sit Trap Rec
                          if (  (ML(csPtr+2)==(int32)SitPtr)   // = SitPtr  ?
                                  ||(ML(csPtr+2)==(int32)AnySit) ) // or = AnySit ?
                          { // this is TRAP Rec with reaction on SitPtr
                            --SitCnt;           // dec Sit Level counter
                            if (SitCnt<0)       // then The TRAP IS FOUND
                                  break;                // EXIT looking LOOP !!
                          }
                        }
                }
                // skip Trap Rec
                csPtr = (lptr)((bptr)csPtr + Len);
        } // for
        // csPtr points to TagLen position of Trap Rec
 } // FindSitTrap

void _RaiseSit (void) { // RAISE_ situation SitPtr
        int32   Tag;
        // pc = situation pointer (points to position ";" in Sit body)
        // It is nessecary so that to account situation handling levels :
                // If this pc=SitPtr will be put in C-stack,
                // this value will be found for incrementing SitCnt
        SitPtr = pc;
//      ML(csb-BTRAP_SIT) = (int32) SitPtr; // Put Last Sit in BTRAP
        ML(csb-BTRAP_SIT) = (int32)SitPtr; // Put Last Sit in BTRAP
        FindSitTrap();
        // Check, If found Trap is in Bottom of C-stack ?
        if (csPtr==(csb-BTRAP_OFFSET)) {
                // Get (in cmd) address of procedure of End Reaction
                ReacP=(lptr)ML(SitPtr-1);
        }else{  // this is normal Trap Rec
                ReacP=(lptr)ML(csPtr+1);        // Reaction procedure
                Tag = (ML(csPtr)&(STAG_MASK))>>STAG_SHFT;
                // =0 (NTF), =1 (ESC), =3(RTR) (=2 for EONW)
                if (Tag!=NTF) {
                        // set deep of A-stack, stored in Trap
                        opsp=(lptr)ML(csPtr+3);
                        // set new pc
                        pc=(lptr)ML(csPtr+4);
                                // = position pc, stored in RTR-TRAP
                                // or return address for ESC-TRAP
                        // cut down (roll up) the control stack
                        if (Tag==RTR)
                                csPtr -= 2;     // retain the found RTR-TRAP
                                // go up before | IDTRAP | RTLT |
                        else // Tag==ESC
                                csPtr += 5;     // delete the found ESC-TRAP
                                                        // and return address
                                // go down after |TagLen|REAC|SitPtr|Deep|RetAdr|
                                ExecSvertkaCS ();
                                csp = csPtr; // set new position of control stack
                }
        }
        EXECMD_(ReacP); // execute procedure of Reaction
 }// _RaiseSit

 void _RaiseD (void) { // RAISE_ situation from descriptor
        //---  INTELO | RAISE_ | EREACT | ; |
        //                                |---- SitPtr
        pc++;
        // pc points to the end Of Situation Body (Sit pointer)
        _RaiseSit();
 }//_RaiseD

 void _RaiseSt (void) { // _RAISE situation from Top of A-stack
        CPUSH(pc);
        pc = (lptr)DATADR(ATPOP + SIT_OFFSETB);
        // pc must point to the end Of Situation Body (Sit pointer)
        _RaiseSit();
 }//_RaiseSt

 void _ReRaise (void) { // RERAISE situation
        // raise situation, which was raised last time, AGAIN !
        CPUSH(pc);
        pc = (lptr)ML(csb-BTRAP_SIT);   // Get Last Sit from BTRAP
        // pc must point to the end Of Situation Body (Sit pointer)
        _RaiseSit();
 }//_RaiseSt

/***------  INIT MECHANISM of  SITUATION  --------***/

 void InitMS (void) {   // Init Mechanism of Situations
        AnySit= (lptr)DATADR(ANYSIT);
        Put_BTrap();
 }
