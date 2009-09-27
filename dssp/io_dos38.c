#include <bios.h>
#include <conio.h>
#include <i86.h>


void    InitConsole(void) { // Initiation of Console
                            // for console operations
}

union REGPACK r;
struct SREGS s;


void _Trbss_1(void)            // TRB''
{
//   APUSH((int32)_bios_keybrd(_NKEYBRD_READ));
    r.h.ah = 0x0;
    intr( 0x16, &r );
    APUSH ((int32)(r.w.ax));
//    APUSH ((int32)(r.h.dl));
}

void _Trbs_1(void)
{       // TRB'
//   APUSH((int32)(_bios_keybrd(_NKEYBRD_READ)&0x0ff));
//     APUSH(getch());
    r.h.ah = 0x7;
    intr( 0x21, &r );
    APUSH ((int32)(r.h.al));

}

void _Tobs(void)             // TOB''
{    putch(((ATPOP)&0xFF)); }

void _TTIn(void)
{
//APUSH (-kbhit());
    r.h.ah = 0x1;
    intr( 0x16, &r );
    if (r.w.flags & 0x040) 
    APUSH ((int32)(0));
    else
    APUSH ((int32)(r.w.ax));

}

void _RCP(void)
{   r.h.ah = 0x03;
    r.h.al = 0x0;
    r.h.bh = 0x0;
    intr( 0x10, &r );
    APUSH ((int32)(r.h.dh));
    APUSH ((int32)(r.h.dl));
}

void _SCP(void)
{   r.h.ah = 0x02;
    r.h.al = 0x0;
    r.h.bh = 0x0;
    r.h.dl=ATPOP;
    r.h.dh=ATPOP;
    intr( 0x10, &r);
}

#define SCREEN_AREA 0xb800
#define SCREEN_LEN 80*2
#define SCREEN_LIN_ADDR ((SCREEN_AREA) << 4)
#define SCREEN_SIZE 80*25
char       *ptr;

void _WUp(void)
{  int32 x,xn,y,xe,ye,n;
   char *ptrl,*ptr1,*ptr2,atr;
   ptr = (char *)SCREEN_LIN_ADDR;
   ptr1 = (char *)SCREEN_LIN_ADDR;
   n=ATPOP;
   xe=ATPOP;
   ye=ATPOP;
   xn=x=ATPOP;
   y=ATPOP;
   atr=(int8)ATPOP;
   ptr+=((y)*SCREEN_LEN+(x)*2);
   ptrl=ptr;
   ptr2=ptr1=ptr+n*SCREEN_LEN;
   if (n==0) n=ye+1;
   for (y;y<=ye-n;y++)
   { for (x=xn;x<=xe;x++)
     { *(ptr++)=*(ptr1++);*(ptr++)=*(ptr1++); }
     ptr1=ptr2=ptr2+SCREEN_LEN;
     ptr=ptrl=ptrl+SCREEN_LEN;
   }
   for (y;y<=ye;y++)
   { for (x=xn;x<=xe;x++)
     { *(ptr++)=32;*(ptr++)=atr; }
     ptr1=ptr2=ptr2+SCREEN_LEN;
     ptr=ptrl=ptrl+SCREEN_LEN;
   }
}

void _WDown(void)
{
   int32 x,xn,y,xe,ye,n;
   char *ptrl,*ptr1,*ptr2,atr;
   ptr = (char *)SCREEN_LIN_ADDR;
   ptr1 = (char *)SCREEN_LIN_ADDR;
   n=ATPOP;
   xn=xe=ATPOP;
   ye=ATPOP;
   x=ATPOP;
   y=ATPOP;
   atr=(int8)ATPOP;
   ptr1+=((ye)*SCREEN_LEN+(xe)*2)+1;
   ptr2=ptr1;
   ptrl=ptr=ptr1-n*SCREEN_LEN;
   if (n==0) n=-(ye+1);
   for (ye;ye>=y+n;ye--)
   { for (xe=xn;xe>=x;xe--)
     { *(ptr1--)=*(ptr--);*(ptr1--)=*(ptr--); }
     ptr1=ptr2=ptr2-SCREEN_LEN;
     ptr=ptrl=ptrl-SCREEN_LEN;
   }
   for (ye;ye>=y;ye--)
   { for (xe=xn;xe>=x;xe--)
     { *(ptr1--)=atr;*(ptr1--)=32; }
     ptr1=ptr2=ptr2-SCREEN_LEN;
     ptr=ptrl=ptrl-SCREEN_LEN;
   }
}

void _WAtr(void)
{   ptr = (char *)SCREEN_LIN_ADDR;
    r.h.ah = 0x03;
    r.h.al = 0x0;
    r.h.bh = 0x0;
    intr( 0x10, &r );
    ptr+=(((r.h.dh))*SCREEN_LEN+(r.h.dl)*2);
    *(ptr+1)=(char)((ATPOP)&0x0ff);
    *(ptr)=(char)((ATPOP)&0x0ff);
}

void _RAtr(void)
{   ptr = (char *)SCREEN_LIN_ADDR;
    r.h.ah = 0x03;
    r.h.al = 0x0;
    r.h.bh = 0x0;
    intr( 0x10, &r );
    ptr+=(((r.h.dh))*SCREEN_LEN+(r.h.dl)*2);
    APUSH (*(ptr));
    APUSH (*(ptr+1));
}

void _WAtrL(void)
{  int32 ii,end;
   int8 ch,atr;
   ptr = (char *)SCREEN_LIN_ADDR;
   r.h.ah = 0x03;
   r.h.al = 0x0;
   r.h.bh = 0x0;
   intr( 0x10, &r );
   ptr+=(((r.h.dh))*SCREEN_LEN+(r.h.dl)*2);
   APOP(end);
   atr=(int8)((ATPOP)&(0x0ff));
   ch=(int8)((ATPOP)&(0x0ff));
   for (ii=0;ii<end;ii++)
   { *(ptr++)=ch;*(ptr++)=atr; }
}


// I/O ports
unsigned short dataword;
unsigned long datalong;
int databyte;
unsigned short port;


void PortB(void)  //  PORTB
{       port=(unsigned short)AT;
        databyte=inp(port);
        AT=(int32)databyte;
}
void PortW(void)  //  PORT
{
        port=(unsigned short)AT;
        dataword=(unsigned short)inpw(port);
        AT=(int32)dataword;
}
void PortD(void)  //  PORTD
{
        port=(unsigned short)AT;
        datalong=(unsigned long)inpd(port);
        AT=(int32)datalong;
}
void sPortB(void) // !PORTB
{       port=(unsigned short)ATPOP;
        databyte=(int)ATPOP;
        databyte=outp(port,databyte);
}
void sPortW(void) // !PORT
{       port=(unsigned short)ATPOP;
        dataword=(unsigned short)ATPOP;
        dataword=outpw(port,dataword);
}
void sPortD(void) // !PORTD
{       port=(unsigned short)ATPOP;
        datalong=(unsigned long)ATPOP;
        datalong=outpd(port,datalong);
}