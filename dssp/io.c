/*********************************************************/
/***     KERNEL for DSSP INTERPRETER  III VARIANT      ***/
/***  -----------------------------------------------  ***/
/***           INPUT / OUTPUT  OPERATIONS              ***/
/***  -----------------------------------------------  ***/
/*** adopted from Frantov D., modified by Burtsev A.A. ***/
/***              last modification   17 may   1998    ***/
/***    Dmitry Frantov modification   2 jule   1998    ***/
/***               ( marked !!!DF!!! )                 ***/
/*********************************************************/
#ifdef IBM
#include <io.h>
#endif

#include <fcntl.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>

#ifdef Win32
#include "io_win32.c"
#include "io_dir32.c"
#endif

#ifdef Dos386
#include "io_dos38.c"
#include "io_dir38.c"
#endif

#ifdef Sun
#include "io_sun.c"
#endif
#ifndef Sun
#ifdef Lite
#include "io_sun.c"
#endif
#endif

#include "mem.c"


#ifndef Dos386
 void _Tobs(void)       {       // TOB'
	 putchar((ATPOP)&0xFF); 
 }
#endif

#ifndef Win32
#ifndef Dos386
 void _Trbs_1(void)       {       // TRB'
	APUSH( getchar() );
 }
#endif
#endif 
 
#ifndef Win32
#ifndef Dos386
 void _Trbss_1(void)      {       // TRB''
	int16 ch,ch1;
	ch1=0;
	ch= getchar();
	if (!ch)
		ch1=getchar();
	APUSH( (ch1<<8)|ch );
 }
#endif
#endif



//Disk Operations

void _FsFun_C(void) /* returned error code and CF flag */
{
	APUSH (errno);
	APUSH (errno!=0);
//      perror("dssp:");
	errno=0;
}

void _Clos_C(void) /* close file REBX = handle */
{
        errno=0;
	close(ATPOP);
    if ((errno!=EBADF))
		errno=0;
}

void _Open_C(void) /* close file REBX = handle */
{
	int16 Wmode;
	char *Cname;
	Wmode=(int16)ATPOP;
	Cname=(char*)(ATPOP+DsspMem);
#ifdef IBM
	Wmode|=O_BINARY;
#endif
        errno=0;
	APUSH(open(Cname,Wmode));
    if ((errno!=EACCES)&&(errno!=EEXIST)&&(errno!=EINVAL)&&
		(errno!=EMFILE)&&(errno!=ENOENT))
		errno=0;
}

void _LSeek_C(void) /* seek file-pointer */
{
	int16 Wmode,Wch;
	int32 Lnpos;
        errno=0;
	Wch=(int16)ATPOP; 
	Wmode=(int16)ATPOP;
	Lnpos=ATPOP; 
	APUSH(lseek(Wch,Lnpos,Wmode));
    if ((errno!=EBADF)&&(errno!=EINVAL))
		errno=0;
}

void _RdWr_C(void) /* read-write n bytes */
{
	unsigned int Wlen;
	int16 Wch;
	int32 Ladr,fn;
        errno=0;
	Wch=(int16)ATPOP;
	fn=ATPOP;
	Wlen=(unsigned int)ATPOP;
	Ladr=(int32)(ATPOP+DsspMem);
	APUSH(Wlen);
#ifdef IBM
#ifndef Watcom
	_setmode(Wch,_O_BINARY);
#endif
#ifdef Watcom
	setmode(Wch,O_BINARY);
#endif
#endif
	if (fn==0x3F00)
	 {
		APUSH(read(Wch,(void*)Ladr,Wlen));
	    if ((errno!=EBADF))
			errno=0;
	 }
	else
	 {
      if (fn==0x4000)
		APUSH(write(Wch,(void*)Ladr,Wlen));
	    if ((errno!=EBADF)&&(errno!=ENOSPC))
			errno=0;
	 } 
}




void _Crt0_C(void) /* create file or truncked file (if exist) */
{
	char *Cname;
	int16 Wattr;
        errno=0;
	Wattr=(int16)ATPOP;
	Cname=(char*)(ATPOP+DsspMem);
	APUSH(creat(Cname, S_IREAD | S_IWRITE));
	if ((errno!=EACCES) && (errno!=EMFILE) && (errno!=ENOENT))
		errno=0;
}

void _Crt_C(void)
{
	char *Cname;
	int16 Wattr;
        errno=0;
	Wattr=(int16)ATPOP;
	Cname=(char*)(ATPOP+DsspMem);
	APUSH(open(Cname,O_CREAT|O_EXCL , S_IREAD|S_IWRITE));
}

void _Crt_TMP_C(void)
{
	char *Cname;
	int16 Wattr;
        errno=0;
	Wattr=(int16)ATPOP;
        Cname=(char*)(ATPOP+DsspMem);
	APUSH(open(Cname,O_CREAT , S_IREAD|S_IWRITE));
}

void _Ren_C(void)
{
	char *Cname_old,*Cname_new;
	Cname_old=(char*)(ATPOP+DsspMem);
	Cname_new=(char*)(ATPOP+DsspMem);
	APUSH(rename(Cname_old,Cname_new));
}

void _Del_C(void)
{
	char *Cname;
	Cname=(char*)(ATPOP+DsspMem);
	APUSH(remove(Cname));
}

/******************************************************/
/* for Lite & Sun version: return 1;*/
#ifndef Win32
#ifndef Dos386
void _TTIn(void)
{ APUSH(1); }
#endif
#endif

/* part of TRB' and TRB'' (for nonWindows platforms) */
#ifndef Win32
void _Trbs_0(void)        {}       // TRB'
void _Trbss_0(void)       {}       // TRB''
#endif

/* Get OS version = 1 */
#ifndef Win32
void _OS(void)          // [] OS? [0-WinNT,1-Win9x]
 {
        APUSH (1);
 }
#endif

#ifndef Win32
#ifndef Dos386
/* Get byte in IO-port (return 0)*/
void PortB(void)  //  PORTB
{
	AT=(int32)0;
}
/* Get word in IO-port (return 0)*/
void PortW(void)  //  PORT
{
	AT=(int32)0;
}
/* Get double word in IO-port (return 0)*/
void PortD(void)  //  PORTD
{
	AT=(int32)0;
}
/*Put byte in IO-port (only delete data and port number)*/
void sPortB(void) // !PORTB
{
	ADEL;
	ADEL;
}
/*Put word in IO-port (only delete data and port number)*/
void sPortW(void) // !PORT
{
	ADEL;
	ADEL;
}
/*Put Dword in IO-port (only delete data and port number)*/
void sPortD(void) // !PORTD
{
	ADEL;
	ADEL;
}

/* Change Dir delete adr in stack */
void _ChDir_C(void) 
{
	ADEL;
}
/* SetDrive return 0 */
void _SetDrv_C(void) 
{
	AT=0;
}

/* GetDir return null string and 0 */
void _GetChDir_C(void) 
{
	char *Cbuf;
	int16 Wdrv;
	Wdrv=(int16)(ATPOP);
	Cbuf=(char*)(ATPOP+DsspMem);
	Cbuf-=3; // _getdcwd return <drive>:\<dir>
	*Cbuf=0;
	APUSH (0);
}
/* GetDrive return 0 */
void _GetDrv_C(void) 
{
	APUSH (0);
}

void _FFF_C(void)
{
	// ['DTA,AS,ATTR]
	ADEL;ADEL;ADEL;
}

void _FNF_C(void)
{
	//['DTA]
	ADEL;
}
void _CloseAll_C(void)
{
}
#endif
#endif
