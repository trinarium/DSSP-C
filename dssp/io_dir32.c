#include <time.h>
#include <direct.h>

// work with directory and file
struct tm *times,tm;


void _ChDir_C(void) 
{
	char *Cname;
	Cname=(char*)(ATPOP+DsspMem);
	_chdir(Cname);
}

void _SetDrv_C(void) 
{
	int16 Wdrv;
	Wdrv=(int16)(ATPOP); Wdrv++;
	APUSH (_chdrive(Wdrv)!=0);
}

void _GetChDir_C(void) 
{
	char *Cbuf;
	int16 Wdrv;
	Wdrv=(int16)(ATPOP);
	Cbuf=(char*)(ATPOP+DsspMem);
	Cbuf-=3; // _getdcwd return <drive>:\<dir>
	if (_getdcwd(Wdrv,Cbuf,300)==NULL)
	 {
	  APUSH (errno!=0);
	 }
	else
	{APUSH (0);}
}

void _GetDrv_C(void) 
{
	APUSH (_getdrive());
}


struct _finddata_t infd;
int32 *lpBuf;
int8 *cpFilSp;
int32 lHndFind;

void _FFF_C(void)
{
	// ['DTA,AS,ATTR]
	int32 attr,timf;

    APOP(attr);
	cpFilSp=(int8*)(ATPOP+DsspMem);
	lpBuf=(int32*)(ATPOP+DsspMem);
	times=&tm;

	lHndFind=_findfirst(cpFilSp,&infd);
	if (lHndFind==-1)
	   {
		APUSH (errno);
	   }
	  else
	   {
		  APUSH (0);
		  *lpBuf=infd.attrib;
		  times=localtime(&(infd.time_create));
		  if (times!=NULL)
		  {
		   timf=times->tm_mday;
		   timf|=(((times->tm_mon+1) << 5) & 0x01e0);
		   timf|=(((times->tm_year-62) << 8) & 0x0ff00);
		   timf|=((times->tm_min << (5+16)) & 0x07e00000);
  		   timf|=((times->tm_hour << (0x0b+16)) & 0xf8000000);
		  }
		  else
  		  {
		   timf=1;
		   timf|=(((1) << 5) & 0x01e0);
		   timf|=(((70-62) << 8) & 0x0ff00);
		   timf|=((00 << (5+16)) & 0x07e00000);
  		   timf|=((12 << (0x0b+16)) & 0xf8000000);
		  }
		  *(lpBuf+1)=timf;
		  *(lpBuf+2)=infd.time_access;
		  *(lpBuf+3)=infd.time_write;
		  *(lpBuf+4)=infd.size;
		  *(lpBuf+5)=lHndFind;
		  strcpy((char*)(lpBuf+6),infd.name);
	   }
}

void _FNF_C(void)
{
	//['DTA]
	int32 timf;
    lpBuf=(int32*)(ATPOP+DsspMem);
	lHndFind=*(lpBuf+5);

	if (_findnext(lHndFind,&infd))
	   {
		APUSH (errno);
		_findclose(lHndFind);
	   }
	 else
	  {
		  APUSH (0);
		  *lpBuf=infd.attrib;
		  times=localtime(&(infd.time_create));
		  if (times!=NULL)
		  {
		   timf=times->tm_mday;
		   timf|=(((times->tm_mon+1) << 5) & 0x01e0);
		   timf|=(((times->tm_year-62) << 8) & 0x0ff00);
		   timf|=((times->tm_min << (5+16)) & 0x07e00000);
  		   timf|=((times->tm_hour << (0x0b+16)) & 0xf8000000);
		  }
		  else
  		  {
		   timf=1;
		   timf|=(((1) << 5) & 0x01e0);
		   timf|=(((70-62) << 8) & 0x0ff00);
		   timf|=((00 << (5+16)) & 0x07e00000);
  		   timf|=((12 << (0x0b+16)) & 0xf8000000);
		  }

          *(lpBuf+1)=timf;
		  *(lpBuf+2)=infd.time_access;
		  *(lpBuf+3)=infd.time_write;
		  *(lpBuf+4)=infd.size;
		  *(lpBuf+5)=lHndFind;
		  strcpy((char*)(lpBuf+6),infd.name);
	  }
}
