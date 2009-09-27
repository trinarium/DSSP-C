//KeyBoard Operations


//    Console operations

void InitConsole(void)
 { // Initiation of Console
 }


void _WUp(void)
{
   ADEL;
   ADEL;ADEL;ADEL;ADEL;
   ADEL;
}


void _WDown(void)
{
   ADEL;
   ADEL;ADEL;ADEL;ADEL;
   ADEL;
}


void _WAtr(void)
{
   ADEL;
   putchar((int8)ATPOP);

}

void _RAtr(void)
{
   APUSH (32);
   APUSH (7);
}

void _WAtrL(void)
{
   int32 ii,end;
   int8 ch;
   APOP(end);
   ADEL;
   ch=(int8)ATPOP;
   for (ii=0;ii<end;ii++)
   {
           putchar(ch);
   }
}

void _RCP(void)
 {
   APUSH (1);
   APUSH (1);
 }

void _SCP(void)
 {
   ADEL;ADEL;
 }


