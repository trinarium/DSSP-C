#include <time.h>
#include <dos.h>
#include <direct.h>

// work with directory and file

int _chdrive(unsigned ws)
 {
  unsigned tot;
  _dos_setdrive(ws,&tot);
  _dos_getdrive(&ws);
  return ws;
 }


void _ChDir_C(void)
{
        char *Cname;
        Cname=(char*)(ATPOP+DsspMem);
        chdir(Cname);
}

void _SetDrv_C(void)
{
        int16 Wdrv;
        Wdrv=(int16)(ATPOP); Wdrv++;
        APUSH (_chdrive(Wdrv)==0);
}

void _GetChDir_C(void)
{
        char *Cbuf;
        int16 Wdrv,wwdrv;
        Wdrv=(int16)(ATPOP);
        Cbuf=(char*)(ATPOP+DsspMem);
        Cbuf-=3; // _getdcwd return <drive>:\<dir>
        wwdrv=_getdrive();
        _chdrive(Wdrv);
        if (getcwd(Cbuf,300)==NULL)
         {
          APUSH (errno!=0);
         }
        else
        {APUSH (0);}
        _chdrive(wwdrv);
}

void _GetDrv_C(void)
{
        APUSH (_getdrive());
}


void _CloseAll_C(void)
{
        fcloseall();
}

struct find_t infd;
int32 *lpBuf;
int8 *cpFilSp;
int32 lret;
struct tm *times,tma;
time_t tim;

void _FFF_C(void)
{       // ['DTA,AS,ATTR]
        int32 attr,timf;

        APOP(attr);
        cpFilSp=(int8*)(ATPOP+DsspMem);
        lpBuf=(int32*)(ATPOP+DsspMem);
        times=&tma;

        lret=_dos_findfirst(cpFilSp,attr,&infd);
        if (lret==-1) { APUSH (errno); }
          else
          {       APUSH (0);
                  *lpBuf=infd.attrib;
                  timf=((infd.wr_time<<16)&0x0ffff0000)|infd.wr_date;
                  *(lpBuf+1)=timf;
                  *(lpBuf+2)=infd.wr_time;
                  *(lpBuf+3)=infd.wr_date;
                  *(lpBuf+4)=infd.size;
                  *(lpBuf+5)=lret;
                  strcpy((char*)(lpBuf+6),infd.name);
           }
}

void _FNF_C(void)
{       //['DTA]
        int32 timf;
        lpBuf=(int32*)(ATPOP+DsspMem);
        lret=*(lpBuf+5);

        if (_dos_findnext(&infd))
           {    APUSH (errno);
//              _findclose(lHndFind);
           }
         else
          {
                  APUSH (0);
                  *lpBuf=infd.attrib;
                  timf=((infd.wr_time<<16)&0x0ffff0000)|infd.wr_date;

                  *(lpBuf+1)=timf;
                  *(lpBuf+2)=infd.wr_time;
                  *(lpBuf+3)=infd.wr_date;
                  *(lpBuf+4)=infd.size;
                  *(lpBuf+5)=lret;
                  strcpy((char*)(lpBuf+6),infd.name);
          }
}