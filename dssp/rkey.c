#define SHIFTS (SHIFT_PRESSED)
#define CAPS   (CAPSLOCK_ON)
#define NUMS   (NUMLOCK_ON)
#define SCRS   (SCROLLLOCK_ON)
#define ALTS   (RIGHT_ALT_PRESSED | LEFT_ALT_PRESSED)
#define CTRLS  (RIGHT_CTRL_PRESSED | LEFT_CTRL_PRESSED)
#define F1      0x70
#define F12     0x7b
#define PGUP    0x21
#define DOWN    0x28
#define ENH     (ENHANCED_KEY)


int noncode[7]={0x01d,0x02a,0x36,0x38,0x03a,0x45,0x46};
              // Ctrl  LSh  RSh  Alt  Caps  Num  Scroll
int direct[8]={0x84,0x76,0x75,0x77,0x73,0x48,0x74, 0x50};
              //PgUp PgDn End  Home Left Up   Right Down

void rkey_nt(int32 *scanc,int8 *ch1,int8 *ch2)
  {
	int32 i=1,j;
	int32 ascii,scanch,ch,state,key,uni,scan;
	INPUT_RECORD inf;
	//while (1)
	//{
     do 
	  {
	  //  PeekConsoleInput(HndIn,&inf,1,&i);
		ReadConsoleInput(HndIn,&inf,1,&i); 
	  
        if (inf.EventType==KEY_EVENT)
		  if (inf.Event.KeyEvent.bKeyDown==TRUE)
		   {
		    if (!inf.Event.KeyEvent.uChar.AsciiChar)
		     { 
	          for (j=0;j<7;j++)
				if (noncode[j]==inf.Event.KeyEvent.wVirtualScanCode) break;
              if (j==7) break;
		     }
		    else
			 break;
		   }  
		//ReadConsoleInput(HndIn,&inf,1,&i); 
	  }
	 while (1);
	 //(inf.EventType!=KEY_EVENT) & (inf.Event.KeyEvent.bKeyDown!=TRUE));
     //ReadConsoleInput(HndIn,&inf,1,&i);
	 key=inf.Event.KeyEvent.wVirtualKeyCode;
	 state=inf.Event.KeyEvent.dwControlKeyState;
	 ascii=inf.Event.KeyEvent.uChar.AsciiChar;
	 uni=inf.Event.KeyEvent.uChar.UnicodeChar;
	 scan=(int32)inf.Event.KeyEvent.wVirtualScanCode;

	 if (ascii) // normal symbol with ascii code
	   if (!(state & ALTS)) // ALTs not pressed
	    {
		 ch=ascii & 0x0ff;
		 scanch=(ascii & 0xff) | ((scan<<8) &0x0ff00);
         *scanc=scanch;
	     *ch2=0;
		 *ch1=ch & 0x0ff;
         // *ch1=((ch & 0x0ff00)>>8) & 0x0ff;
	     return;
	    }
	   else
	    {
		   ch=scan;
		   scanch=(ch<<8) & 0x0ff00;
	    }
	 else
       if ((key>=F1) && (key<=F12))  // Fxx
	      {
		    if (state & ALTS)  // Alt+Fxx
		      ch=scan+0x2d;
	        else
			 {
	          if (state & CTRLS)  //Ctrl+Fxx
			   ch=scan+0x023;
			  else
			   {
                if (state & SHIFTS)  //Shift+Fxx
			     ch=scan+0x019;
				else
				 ch=scan;
			  }
			}
		   scanch=(ch<<8) & 0x0ff00;
	      } 
	   else
    	 if ((key>=PGUP) & (key<=DOWN))
		   {
             if (state & CTRLS) // CTRL+Control_Key
				 ch=direct[key-PGUP];
			 else
			     ch=scan;
			 scanch=(ch<<8) & 0x0ff00;
	       }
	      else
		   {
             ch=scan;
  			 scanch=(ch<<8) & 0x0ff00;
		   }
	//}
	*scanc=scanch;
	*ch2=ch & 0x0ff;
    *ch1=((ch & 0x0ff00)>>8) & 0x0ff;
	return;
  }


void rkey_95(int32 *scanc,int8 *ch1,int8 *ch2)
  {
	int8 cc;
	int32 i=1,j;
	int32 ascii,scanch,ch,state,key,uni,scan;
	INPUT_RECORD inf;
//	while (1)
//	{
     do 
	  {
	    PeekConsoleInput(HndIn,&inf,1,&i);
        if (inf.EventType==KEY_EVENT)
		  if (inf.Event.KeyEvent.bKeyDown==TRUE)
		    if (!inf.Event.KeyEvent.uChar.AsciiChar)
		     { 
			  for (j=0;j<7;j++)
				if (noncode[j]==inf.Event.KeyEvent.wVirtualScanCode) break;
              if (j==7) break;
		     }
		    else
 			 break;
		ReadConsoleInput(HndIn,&inf,1,&i); 
	  }
	 while (1);
		 //(inf.EventType!=KEY_EVENT) & (inf.Event.KeyEvent.bKeyDown!=TRUE));
     //ReadConsoleInput(HndIn,&inf,1,&i);
	 state=inf.Event.KeyEvent.dwControlKeyState;
	 if ((inf.Event.KeyEvent.uChar.AsciiChar==0) || (state & ALTS) || (state & ENH))
	     {
		    //printf("yyy");
	     	ReadConsoleInput(HndIn,&inf,1,&i);inf.Event.KeyEvent.uChar.AsciiChar=0;
		  }
	  else
	   {
		  ReadFile(HndIn,&cc,1,&i,NULL);
		  inf.Event.KeyEvent.uChar.AsciiChar=cc;
	   }
	 key=inf.Event.KeyEvent.wVirtualKeyCode;
	 state=inf.Event.KeyEvent.dwControlKeyState;
	 ascii=inf.Event.KeyEvent.uChar.AsciiChar;
	 uni=inf.Event.KeyEvent.uChar.UnicodeChar;
	 scan=(int32)inf.Event.KeyEvent.wVirtualScanCode;

	 if (ascii) // normal symbol with ascii code
	   if (!(state & ALTS)) // ALTs not pressed
	    {
		 ch=ascii & 0x0ff;
		 scanch=(ascii & 0xff) | ((scan<<8) &0x0ff00);
         *scanc=scanch;
	     *ch2=0;
		 *ch1=ch & 0x0ff;
         // *ch1=((ch & 0x0ff00)>>8) & 0x0ff;
	     return;
	    }
	   else
	    {
		   ch=scan;
		   scanch=(ch<<8) & 0x0ff00;
	    }
	 else
       if ((key>=F1) && (key<=F12))  // Fxx
	      {
		    if (state & ALTS)  // Alt+Fxx
		      ch=scan+0x2d;
	        else
			 {
	          if (state & CTRLS)  //Ctrl+Fxx
			   ch=scan+0x023;
			  else
			   {
                if (state & SHIFTS)  //Shift+Fxx
			     ch=scan+0x019;
				else
				 ch=scan;
			  }
			}
		   scanch=(ch<<8) & 0x0ff00;
	      } 
	   else
    	 if ((key>=PGUP) & (key<=DOWN))
		   {
             if (state & CTRLS) // CTRL+Control_Key
				 ch=direct[key-PGUP];
			 else
			     ch=scan;
			 scanch=(ch<<8) & 0x0ff00;
	       }
	      else
		   {
             ch=scan;
  			 scanch=(ch<<8) & 0x0ff00;
		   }
	//}
	*scanc=scanch;
	*ch2=ch & 0x0ff;
    *ch1=((ch & 0x0ff00)>>8) & 0x0ff;
	return;
  }
