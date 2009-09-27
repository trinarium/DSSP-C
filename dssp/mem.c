/*********************************************************/
/***     KERNEL for DSSP INTERPRETER  III VARIANT      ***/
/***  -----------------------------------------------  ***/
/***           Phisical Memory OPERATIONS              ***/
/***  -----------------------------------------------  ***/
/*** written by Frantov D.,           22 dec   1998    ***/
/***    Dmitry Frantov modification   22 dec   1998    ***/
/***                                                   ***/
/*********************************************************/
void mallocD(void)
{
  int32 len;
  len=(int32)AT;
  AT=(int32)malloc(len);
}

void freeD(void)
{
   free((void*)ATPOP);
}

void reallocD(void)  // [PTRold,N] REALLOC [PTRnew]
{
  int32 len,adr;
  len=(int32)ATPOP;
  adr=(int32)AT;
  AT=(int32)realloc((void*)adr,len);
}
void nullptr(void)
{
  APUSH((int32)NULL);
}


// read phisical memory [adrF] @AB [@adrF]
void aAB(void)
{ int8 *ptr;
  ptr=(int8*)AT;
  AT=(int32)(*ptr);
}
void aAW(void)
{ int16 *ptr;
  ptr=(int16*)AT;
  AT=(int32)(*ptr);
}
void aAL(void)
{ int32 *ptr;
  ptr=(int32*)AT;
  AT=(int32)(*ptr);
}    
// write phisical memory  [l,a] !TAB [] 
void TAB(void)
 { int8 *ptr;
   int8 l;
   ptr=(int8*)ATPOP;
   l=(int8)ATPOP;
   *ptr=(int8)l;
}
void TAW(void)
 { int16 *ptr;
   int16 l;
   ptr=(int16*)ATPOP;
   l=(int16)ATPOP;
   *ptr=(int16)l;
}
void TAL(void)
 { int32 *ptr,l;
   ptr=(int32*)ATPOP;
   l=(int32)ATPOP;
   *ptr=(int32)l;
}

#ifdef Dos386

void _interr(void)
{
  int nint;
  union REGPACK r;
  nint=(int)ATPOP;
  r.x.eax=(unsigned int)ML(REAXA);
  r.x.ebx=(unsigned int)ML(REBXA);
  r.x.ecx=(unsigned int)ML(RECXA);
  r.x.edx=(unsigned int)ML(REDXA);
  r.x.edi=(unsigned int)ML(REDIA);
  r.x.esi=(unsigned int)ML(RESIA);
  r.x.ebp=(unsigned int)ML(REBPA);
  r.x.flags=(unsigned int)ML(REFLA);
  r.x.ds=(unsigned int)ML(RDSA);
  r.x.es=(unsigned int)ML(RESA);
  r.x.fs=(unsigned int)ML(RFSA);
  r.x.gs=(unsigned int)ML(RGSA);
  intr(nint,&r);
  ML(RGSA)=(int32)r.x.gs;
  ML(RFSA)=(int32)r.x.fs;
  ML(RESA)=(int32)r.x.es;
  ML(RDSA)=(int32)r.x.ds;
  ML(REAXA)=(int32)r.x.eax;
  ML(REBXA)=(int32)r.x.ebx;
  ML(RECXA)=(int32)r.x.ecx;
  ML(REDXA)=(int32)r.x.edx;
  ML(REBPA)=(int32)r.x.ebp;
  ML(REDIA)=(int32)r.x.edi;
  ML(RESIA)=(int32)r.x.esi;
  ML(REFLA)=(int32)r.x.flags;
}

#endif
#ifndef Dos386
void _interr(void)
{ ADEL; }
#endif


