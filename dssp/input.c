/*********************************************************/
/***     KERNEL for DSSP INTERPRETER  III VARIANT      ***/
/***  -----------------------------------------------  ***/
/***              INPUT OF DSSP-PROGRAM                ***/
/***  -----------------------------------------------  ***/
/***         started by Burtsev A.A.  23 march 1998    ***/
/***              last modification   17 may   1998    ***/
/*********************************************************/

struct	_DPFHeader {  // Header of DSSP program file
	char	idDPF[4];// Identifier of DSSP PROGRAM file
	int32	hMemSize; // Size of DSSP memory for Program 
	int32	hBlkNum;  // number of blocks to be read (<=8)
	int32	hTable[16]; // Table of Blocks
		// two item for every block (adr,len)
	int32	Dummy[5];	// reserv to expand up to 96 bytes
 };

struct	_DPFHeader DPFHeader ;

 void InputProgram(sptr fname) {
 FILE *fh;	// file handler
 int32	Alen,Adr,Len,n,BNum;   
 
//	printf ("DSSP program is being read from file '%s' \n", fname);
	if ((fh=fopen(fname,"rb"))==NULL) {
	   printf("It is impossible to read file '%s' \n",fname);
	   exit(2);
	}

	// read Dssp-program Header
	Alen=fread(&DPFHeader,sizeof(byte),sizeof(DPFHeader),fh); 
	if // header wasn't read wholly  or has wrong marker 
	  ( (Alen<sizeof(DPFHeader))||
	    (strcmp(DPFHeader.idDPF,"DPF")!=0) ) {
		printf ("Wrong header of DSSP program \n");
		fclose(fh);	exit(5);
	}
		
	free(DsspMem);
	DsspMem = AllocDsspMemory(DPFHeader.hMemSize);
	if (DsspMem==NULL) {
		printf ("There is not enough memory for DSSP program \n");
		fclose(fh);	exit(3);
	}
	
	BNum=DPFHeader.hBlkNum; // number of block, which have to read
	// read program into DSSP-memory up to END_MEM bytes
	for (n=0; n<2*BNum; n+=2 ) {
		// read next block 
		Adr= DPFHeader.hTable[n];   // address where to read
		Len= DPFHeader.hTable[n+1]; // length of read block (in bytes)
		if (Len>0) { // read block
		  Alen= fread(DsspMem+Adr,sizeof(byte),Len,fh); 
		  if (Alen<Len) {
		    printf ("Program haven't been read wholly \n");
		    free(DsspMem);
		    fclose(fh);	exit(4);
		  }
		}
    }//while
		
	//if ( !feof(fh) ) {
	//	printf ("Program haven't been read up to end of file \n");
	//	free(DsspMem);
	//	fclose(fh);	exit(4);
    //}

	if ( ferror(fh) ) {
		printf ("Error during reading DSSP program \n");
		free(DsspMem);
		fclose(fh);	exit(4);
	}
	
	// DSSP program have been read normally
	fclose(fh);	

 } //InputProgram
