/*********************************************************/
/***     KERNEL for DSSP INTERPRETER  III VARIANT      ***/
/***  -----------------------------------------------  ***/
/***        GENERATION of DSSP VOCABULARY FILE         ***/
/***  -----------------------------------------------  ***/
/***         started by Burtsev A.A.  23 march 1998    ***/
/***              last modification   17 november 1998 ***/
/*********************************************************/

#ifdef  TEST_VERS
        #define TEST_SLOVAR     // output test words into slovar
#endif

#define NAMEV   "dssp.voc"      // default name fo voc-file
#define WORDNAMELEN (32)        // max length of DSSP word
#define NULLBYTE (0)    // NULL byte as appendix of name

struct  _DVFHeader {  // Header of Vocabulary file
   char    idDVF[4]; // Identifier of DSSP Vocabulary file
   int32   WT; // BW0 - offset of begining of word table
   int32   NW; // Number of words in Word Table
   int32   NOPCODE;        // NOPCODE
   int32   Dummy[20];      // reserv to expand up to 96 bytes
};

struct  _DVFHeader DVFHeader =
{ "DVF",
   START_CMD+SIZE_CMD, // TW
   1+sizeof(SLOVAR)/sizeof(SLOVO), // maximum NW
   fn_Nop,     //  NOPCODE

   // Dummy [20]
   0,0,0,0,0,
   0,0,0,0,0,
   0,0,0,0,0,
   0,0,0,0,
   0xFFFFffff // priznak of Header END (-1)
};

void CreateVocFile(sptr fname)
{ FILE *fh;      // file handler
  pSLOVO pSlv; /* pointer to SLOVO */
  int32 len, WNum;
  byte NULLByte =0;

       printf (" It is created DSSP vocabulary file '%s' \n", fname);
       if ((fh=fopen(fname,"wb"))==NULL) {
          printf("It is impossible to create file '%s' \n", fname);
          exit(1);
       }

       // REMARK: not all WORDs from SLOVAR have to be put in file
       // So let's Calculate Number of WORDs (SLOV) in SLOVAR,
       // which will be put to Vocabulary file
       WNum=0;
       pSlv = pSlvr;
       while (pSlv->cmdAdr!=cmd_) {

#ifndef TEST_SLOVAR
          if (pSlv->cmdAdr==cmd_TEST)     break; // terminate slovar
#endif
         len= strlen(pSlv->pname);
         if (len>0) { // if not empty SLOVO
                 WNum++; // account this SLOVO
         }//if
         pSlv++;   // to next SLOVO
       }//while

       DVFHeader.NW= WNum;  // remember real number of WORDs in VFHeader

       // put Header
       fwrite(&DVFHeader,sizeof(byte),sizeof(DVFHeader),fh);

       //---- put DSSP memory ( System Area + Command bodies )
       *(lptr)(DsspMem+START_CMD+SIZE_CMD-4) = 0xEEEEeeee;
                               // this is priznak of END of command memory
       fwrite(DsspMem,sizeof(byte),START_CMD+SIZE_CMD,fh);

       // Put table of words (vocabulary,slovar) in file
       pSlv = pSlvr;
       while (pSlv->cmdAdr!=cmd_) {

#ifndef TEST_SLOVAR
          if (pSlv->cmdAdr==cmd_TEST) break; // terminate slovar
#endif
         len= strlen(pSlv->pname);
         if (len>0) { // not empty SLOVO
               // put cmd
               fwrite(&pSlv->cmdAdr,sizeof(int32),1,fh);
               // put name string
               fwrite(pSlv->pname,sizeof(char),len,fh);
               // put NULL bytes
               for ( ; len<WORDNAMELEN; len++) fputc(NULLBYTE,fh);
         } //if
         pSlv++;   // to next SLOVO
       } //while
       fclose(fh);
}
/* CreateVocFile */
