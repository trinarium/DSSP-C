/*********************************************************/
/***     KERNEL for DSSP INTERPRETER  III VARIANT      ***/
/***  -----------------------------------------------  ***/
/***              DEBUG  OPERATIONS                    ***/
/***  -----------------------------------------------  ***/
/*** adopted from Frantov D., modified by Burtsev A.A. ***/
/***              last modification   17 may   1998    ***/
/*********************************************************/

//-------------- \G11 ----------------------
//
// This operation is necessary for Dssp-debugger
//      It pops from A-stack new CSP, then
//      gets new PC from new C-stack
//      and then pops from A-stack new command for execution

void _G11(void)
{    APOP(csp);
     CPOP(pc);
     EXECMD_(ATPOP);
}