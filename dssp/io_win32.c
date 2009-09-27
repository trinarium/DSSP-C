void _CloseAll_C(void)
{
	fcloseall();
}

CHAR_INFO inf;
COORD cur,sz;
SMALL_RECT rect;
DWORD DConOpt;    // Console Options
CONSOLE_SCREEN_BUFFER_INFO info;
HANDLE HndOut,HndIn;
PCHAR_INFO Pinf;
long S;


//KeyBoard Operations
#include <conio.h>
#include "rkey.c"


 //    Console operations
void	InitConsole(void) { // Initiation of Console
// for console operations
  HndOut=GetStdHandle(STD_OUTPUT_HANDLE);
  HndIn=GetStdHandle(STD_INPUT_HANDLE);
  GetConsoleMode(HndIn,&DConOpt);
  SetConsoleMode(HndIn,DConOpt & (~ENABLE_LINE_INPUT) & (~ENABLE_ECHO_INPUT));
 }

 
void _OS(void)          // [] OS? [0-WinNT,1-Win9x]
 {
	S=GetVersion();
	APUSH (S<0);
 }

void _WUp(void)
{
   int32 n,atr,i,ii;
   int8 dx,dy,end,start;

   APOP (n);
   rect.Right=(int8)ATPOP; rect.Bottom=(int8)ATPOP;
   rect.Left=(int8)ATPOP;  rect.Top=(int8)ATPOP;
   APOP(atr);

   dx=rect.Right-rect.Left+1;  dy=rect.Bottom-rect.Top+1;

   if (!n) n=dy;
   Pinf=(PCHAR_INFO)malloc(dx*dy*sizeof(CHAR_INFO));
   sz.X=dx;  sz.Y=dy;
   cur.X=cur.Y=0;
   ReadConsoleOutput(HndOut,Pinf,sz,cur,&rect);  //read block
   cur.Y+=(int8)n;
   rect.Bottom-=(int8)n;                         // change place
   WriteConsoleOutput(HndOut,Pinf,sz,cur,&rect); // write block
   free(Pinf);
   end=(int8)rect.Right;
   start=(int8)rect.Left;
   for (i=1;i<=n;i++)                          //clear old block
	 {
       inf.Char.AsciiChar=' ';
       inf.Attributes=(int8)atr;
       sz.X=sz.Y=1;
       cur.X=cur.Y=0;
       rect.Bottom++;
       rect.Top=rect.Bottom;
       rect.Left=start;
       rect.Right=start+1;
	   for (ii=start;ii<=end;ii++) 
        {
         WriteConsoleOutput(HndOut,&inf,sz,cur,&rect);
         rect.Left++;  rect.Right++;
        }
     }
}


void _WDown(void)
{
   int32 n,atr,i,ii;
   int8 dx,dy,end,start;
   APOP (n);
   rect.Right=(int8)ATPOP; rect.Bottom=(int8)ATPOP;
   rect.Left=(int8)ATPOP;  rect.Top=(int8)ATPOP;
   APOP(atr);
   dx=rect.Right-rect.Left+1;   dy=rect.Bottom-rect.Top+1;
   if (!n) n=dy;
   Pinf=(PCHAR_INFO)malloc(dx*dy*sizeof(CHAR_INFO));
   sz.X=(int8)dx;  sz.Y=(int8)dy;
   cur.X=cur.Y=0;
   ReadConsoleOutput(HndOut,Pinf,sz,cur,&rect);  //read block
   rect.Top+=(int8)n;                            //change place
   WriteConsoleOutput(HndOut,Pinf,sz,cur,&rect); //write block
   free(Pinf);
   end=(int8)rect.Right;
   start=(int8)rect.Left;
   for (i=1;i<=n;i++)                  // Clear old block
	 {
       inf.Char.AsciiChar=' ';
       inf.Attributes=(int8)atr;
       sz.X=sz.Y=1;
       cur.X=cur.Y=0;
       rect.Top--;  rect.Bottom=rect.Top;
       rect.Left=start;   rect.Right=start+1;
	   for (ii=start;ii<=end;ii++) 
        {
         WriteConsoleOutput(HndOut,&inf,sz,cur,&rect);
         rect.Left++;
         rect.Right++;
        }
     }
}


void _WAtr(void)
{
   inf.Attributes=(int8)ATPOP;
   inf.Char.AsciiChar=(int8)ATPOP;
   GetConsoleScreenBufferInfo(HndOut,&info);
   sz.X=sz.Y=1;
   cur.X=cur.Y=0;
   rect.Left=info.dwCursorPosition.X;
   rect.Top=info.dwCursorPosition.Y;
   rect.Right=info.dwCursorPosition.X;
   rect.Bottom=info.dwCursorPosition.Y;
   WriteConsoleOutput(HndOut,&inf,sz,cur,&rect);
}

void _RAtr(void)
{
   GetConsoleScreenBufferInfo(HndOut,&info);
   sz.X=sz.Y=1;
   cur.X=cur.Y=0;
   rect.Left=info.dwCursorPosition.X;
   rect.Top=info.dwCursorPosition.Y;
   rect.Right=info.dwCursorPosition.X;
   rect.Bottom=info.dwCursorPosition.Y;
   ReadConsoleOutput(HndOut,&inf,sz,cur,&rect);
   APUSH ((int32)inf.Char.AsciiChar);
   APUSH ((int32)inf.Attributes);
}

void _WAtrL(void)
{
   int32 ii,end;
   APOP(end);
   inf.Attributes=(int8)ATPOP;
   inf.Char.AsciiChar=(int8)ATPOP;
   GetConsoleScreenBufferInfo(HndOut,&info);
   cur=info.dwCursorPosition;
   sz.X=sz.Y=1;
   cur.X=cur.Y=0;
   rect.Left=info.dwCursorPosition.X;
   rect.Top=info.dwCursorPosition.Y;
   rect.Right=info.dwCursorPosition.X+1;
   rect.Bottom=info.dwCursorPosition.Y+1;
   for (ii=0;ii<end;ii++) 
   {
     WriteConsoleOutput(HndOut,&inf,sz,cur,&rect);
     rect.Left++;
     rect.Right++;
   }
}

void _RCP(void)
 {
   GetConsoleScreenBufferInfo(HndOut,&info);
   cur=info.dwCursorPosition;
   APUSH ((int32)cur.Y);
   APUSH ((int32)cur.X);
 }

void _SCP(void)
 {
   cur.X=(int8)ATPOP;  cur.Y=(int8)ATPOP;
   SetConsoleCursorPosition(HndOut,cur);
 }



int8 CScan=0,Ch=0;
int32 LScan=0;

void _TTIn(void)
{
	APUSH (kbhit());
}



//for WinNT
void _Trbs_0(void)
  {
	if (CScan!=0) 
	  { 
		APUSH (CScan);
                CScan=0;
		return;
	  }
	rkey_nt(&LScan,&Ch,&CScan);
	APUSH ((int32)Ch & 0X0ff);
	if (Ch!=0) CScan=0;
  }

void _Trbss_0(void)
  {
	rkey_nt(&LScan,&Ch,&CScan);
	CScan=0; 
	APUSH (LScan);
  }

  
// for Win9x
void _Trbs_1(void)
  {
	if (CScan!=0) 
	  { 
		APUSH (CScan);
	    CScan=0;
		return;
	  }
	rkey_95(&LScan,&Ch,&CScan);
	APUSH ((int32)Ch & 0X0ff);
	if (Ch!=0) CScan=0;
  }

void _Trbss_1(void)
  {
	rkey_95(&LScan,&Ch,&CScan);
	CScan=0; 
	APUSH (LScan);
  }
 

// I/O ports

unsigned short dataword;
unsigned long datalong;
int databyte;
unsigned short port;


void PortB(void)  //  PORTB
{
	port=(unsigned short)AT;
	databyte=_inp(port);
	AT=(int32)databyte;
}
void PortW(void)  //  PORT
{
	port=(unsigned short)AT;
	dataword=(unsigned short)_inpw(port);
	AT=(int32)dataword;
}
void PortD(void)  //  PORTD
{
	port=(unsigned short)AT;
	datalong=(unsigned long)_inpd(port);
	AT=(int32)datalong;
}
void sPortB(void) // !PORTB
{
	port=(unsigned short)ATPOP;
	databyte=(int)ATPOP;
	databyte=_outp(port,databyte);
}
void sPortW(void) // !PORT
{
	port=(unsigned short)ATPOP;
	dataword=(unsigned short)ATPOP;
	dataword=_outpw(port,dataword);
}
void sPortD(void) // !PORTD
{
	port=(unsigned short)ATPOP;
	datalong=(unsigned long)ATPOP;
	datalong=_outpd(port,datalong);
}


