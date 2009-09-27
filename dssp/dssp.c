/*********************************************************/
/***     KERNEL for DSSP INTERPRETER  III VARIANT      ***/
/***      Indirect Token Threaded Code ( ITTC )        ***/
/***           ( with relative addresses )             ***/
/***  -----------------------------------------------  ***/
/***                MAIN  MODULE                       ***/
/***  -----------------------------------------------  ***/
/***         started by Burtsev A.A.  23 march 1998    ***/
/***              last modification   22 sept  1998    ***/
/***    Dmitry Frantov modification   2 jule   1998    ***/
/***               ( marked !!!DF!!! )                 ***/
/*********************************************************/

// #define       TEST_VERS
// for generation test version, including some test operations

#include "dssp.cfg"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*#ifdef Lite
#ifndef Sun
  #define Sun
#endif
#endif */


#ifdef Sun
  #include "unistd.h"
  #include "minmax.c"
  #ifdef TERMIOS
    #include <termios.h>
    #define TERMIO struct termios
    #define GTTY(fd, tadr)  tcgetattr(fd, tadr)
    #define STTY(fd, tadr)  tcsetattr(fd, TCSADRAIN, tadr)
  #else
    #include <termio.h>
    #define TERMIO struct termio
    #define GTTY(fd, tadr) ioctl(fd, TCGETA,  tadr)
    #define STTY(fd, tadr) ioctl(fd, TCSETAW, tadr)
  #endif
#endif

#ifndef Sun
  #ifdef Watcom
    #include "minmax.c"
  #endif
#endif

#include "dssp.h"
#include "dssp.hgf"     // Function Headers and Func Table
#include "dssp.mmp"     // DSSP Memory Map, Dssp-addresses
#include "dssp.cmd"     // List of DSSP Command Codes
#include "dssp.slv"     // Slovar (Dictionary) table
#include "interupt.c"   // Interrupt Mehanism
#include "kern.c"       // KERNEL of DSSP-INTERPRETER :
#include "trap.c"       // Operations with Trap in C-Stack
#include "data.c"       // Data structure Operations
#include "debug.c"      // Debug Operations (G11)
#include "contexts.c"   // Coroutine Operations (TRANSFER...)

// Only for Win32
#ifdef Win32
  //#include "dsspwin.c"
  #include "winapi.c" // Win32API Operations
#endif
#ifndef Win32
  #include "nonwin.c"
#endif

#ifdef NG_include
  #include "ng_int.c"
#endif

#include "io.c"         // Input / output Operations
#include "testintr.c"   // Operations for TEST-DSSP
#include "initdssp.c"   // preparation of DSSP-Interpretation
#include "genvoc.c"     // generate vocabulary file for NG
#include "input.c"      // input DSSP-program in Dssp-memory

/*********************************************************/
/******            DSSP  MEMORY                    *******/
/*********************************************************/

bptr AllocDsspMemory(int32 DsspMemSize)
{      // Allocate DsspMemory up to DsspMemSize bytes
       //return( (bptr)malloc(DsspMemSize) );
       // allocate memory and full it by NULL value (zero)
       return( (bptr)calloc(DsspMemSize,sizeof(byte)) );
}

void FreeDsspMem (void) {  // free DSSP Memory
        if (DsspMem!=NULL)
               free(DsspMem);
}

/*********************************************************/
/******     MAIN PROGRAM for DSSP-INTERPRETATION   *******/
/*********************************************************/

#ifdef Sun  
    static TERMIO trm, trm_for_vost;
#endif

int main(int argc,char *argv[])
{ char namev[80];  // string of vocabulary file name
#ifdef NG_include
    char namec[80];  // string of dsp file name
#endif
  char namep[80];  // string of DSSP-program file name
        argv_gl=argv;
        argc_gl=argc;
        namep_gl=namep;
        param_st=2;


#ifdef Sun
        GTTY(fileno(stdin), &trm);
        trm_for_vost = trm;
        trm.c_lflag &= ~ICANON;
        //trm.c_lflag &= ~ECHO;
        trm.c_cc[VMIN] = 1;
        STTY(fileno(stdin), &trm);
#endif

        if (argc==1)
        { // Execute Test DSSP-program
                InitDsspMem();
                printf("\n Test DSSP starts ! \n");
                Interpretation();
                printf("\n Test DSSP finished ! \n");
                //getchar();
                FreeDsspMem();
        } // end Test DSSP-program

        else if ( (argv[1][0]=='-')||(argv[1][0]=='/') )
                // this is switch (-flag), let's recognize it
                switch (argv[1][1])
                { case 'V':
                  case 'v':  // put vocabulary table to file
                        // get file name
                        if (argc>2) { strcpy(namev,argv[2]);  // from command string
                        } else        strcpy(namev,NAMEV); // default file name
                        if (!strchr(namev,'.') ) strcat(namev,".voc"); // append file type
                        InitDsspMem();
                        CreateVocFile(namev);   // create vocabulary file
                        FreeDsspMem();
                        break;

#ifdef NG_include
#define NAMEC "tg.dsp"
                  case 'C':
                  case 'c':  // compile DPF file
                        // get file name
                        if (argc>2) { strcpy(namec,argv[2]);  // from command string
                        } else        strcpy(namec,NAMEC); // default file name
                        if (!strchr(namec,'.')) strcat(namec,".dsp"); // append file type
                        BuildDPFFile(namec);   // create vocabulary file
                        break;
#endif
                  case 'h':  // display help information
                        printf (" Call 'DSSP -v [vocfile[.voc]]' to create vocabulary file \n");
#ifdef NG_include
                        printf (" Call 'DSSP -c [dspfile[.dsp]]' to create DPF file \n");
#endif
                        printf ("   or 'DSSP    dssp.dpf' to execute DSSP-program \n");
                        printf ("   or 'DSSP'  to execute test DSSP-program \n");
                        break;

                  default : printf (" unknown switch ");
                } // switch

        else {  // input DSSP-program in threaded code and execute it
                // get file name from command string
                strcpy(namep,argv[1]);
                if (!strchr(namep,'.')) strcat(namep,".dpf"); // append file type
                InputProgram(namep);
                InitDsspSlovar();  // TMP!! Only for Testing with old Slovar
//              printf("\n Start- DSSP-program ! \n");
                InitConsole(); // initiation console
#ifdef Win32
                InitWin32API(); // initiate DSSP--WIN32API interface
#endif
                Interpretation();
//              printf("\n Finish DSSP-program! \n");
                FreeDsspMem();
        }
#ifdef Sun
        STTY(fileno(stdin), &trm_for_vost);
#endif
        return  0 ;
}
// main
/******  END OF MAIN PROGRAM for DSSP-INTERPRETATION *****/
