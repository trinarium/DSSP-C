/*********************************************************/
/***     KERNEL for DSSP INTERPRETER  III VARIANT      ***/
/***  -----------------------------------------------  ***/
/***  COROUTINEs: CONTEXT  OPERATIONS  (Transfer ...)  ***/
/***  -----------------------------------------------  ***/
/***       started by Burtsev A.A.  24 june      1999  ***/
/***           last modification    29 august    1999  ***/
/*********************************************************/

//********************************************************/
//****    ╠┼╒└═╚╟╠ ╤╬╧╨╬├╨└╠╠═╬╔ ╧┼╨┼─└╫╚ ╙╧╨└┬╦┼═╚▀  ****/
//********************************************************/
//*** эрўрЄю 24-06-99  ╧юёыхфэ   ьюфшЇшърЎш  03-08-99 ****/


//*********************  ╩╬═╥┼╩╤╥ ╧╨╬╓┼╤╤└ ***************/
//
// ╩╬═╥┼╩╤╥ - шэЇюЁьрЎш , ъюЄюЁє■ эхюсїюфшью ёюїЁрэ Є№ т ьюьхэЄ 
//           яЁшюёЄрэютъш яЁюЎхёёр фы  хую яюёыхфє■∙хую ъюЁЁхъЄэюую 
//           тючюсэютыхэш , Єръ, ўЄюс√ юэ ьюу ЁрчтштрЄ№ё  фрыхх,
//           ъръ сєфЄю с√ ш эх яЁшюёЄрэртыштрыё  тютёх
//
// ─╬╧╙╤╥╚╠, ўЄю :
//    1) ╧хЁхъы■ўхэшх эр ъюэЄхъёЄ фЁєуюую ─╤╤╧-яЁюЎхёёр тючьюцэю 
// ╥╬╦▄╩╬ яю юъюэўрэшш яЁшьшЄштр (─╤╤╧-ъюьрэф√ т ьр°шээюь ъюфх) ;
//    2) ╧Ёш яхЁхъы■ўхэшш ъюэЄхъёЄр 
//      р) тхЁ°шэр рЁшЇь. ёЄхър (AT) єяЁ Єрэр т ёЄхъ юяхЁрэфют;
//      с) єърчрЄхы№ ъюьрэф√ яю Єхыє (PC) єяЁ Є√трхЄё  т єяЁрт.ёЄхъ;
//
// ╥╬├─└ фы  ъюЁЁхъЄэюую тюёёЄрэютыхэш  ъюэЄхъёЄр ─╤╤╧-яЁюЎхёёр 
// фюёЄрЄюўэю ёюїЁрэ Є№ ыш°№ ёыхфє■∙шх тхышўшэ√, хую юяЁхфхы ■∙шх :
//    1) єърчрЄхы№  Єхъє∙хую яюыюцхэш  ёЄхър тючтЁрЄют ( CSP ) ;
//    2) єърчрЄхы№  Єхъє∙хую яюыюцхэш  ёЄхър юяхЁрэфют ( OPSP) ;
//    3) єърчрЄхы№  эрўрыр  (уЁрэшЎ√)  ёЄхър тючтЁрЄют ( CSB)  ;
//    4) єърчрЄхы№  эрўрыр  (уЁрэшЎ√)  ёЄхър юяхЁрэфют ( OPSB) ;
//    5) єърчрЄхы№  эрўрыр юсырёЄш ёюсёЄтхээ√ї яхЁхьхээ√ї (OWNBASE) ;
// └ Єръцх фюяюыэшЄ.тхышўшэ√, шёяюы№чєхь√х фы  ЁрсюЄ√ ё фрээ√ьш:
//    6) єърчрЄхы№ (DBAdr) эр Єхыю ¤ъчхьяы Ёр фрээ√ї ;
//    7) єърчрЄхы№ (TypeAdr) эр фхёъЁшяЄюЁ Єшяр ;
//    
//  ╟└╠┼╫└═╚┼.
//    р) єърчрЄхы№  ЇшъЄштэющ ыютє°ъш т юёэютрэшш єяЁ.ёЄхър (BTRAP) 
//       сєфхЄ т√ўшёы Є№ё  чрэютю т чртшёшьюёЄш юЄ єърчрЄхы  эрўрыр 
//       ёЄхър тючтЁрЄют
//    с) т ъюэЄхъёЄ х∙х сєфхЄ фюсртыхэ Їырцюъ Ёхцшьр юсЁрсюЄъш 
//       яЁхЁ√трэшщ т ─╤╤╧ (юЄъЁ√Є шыш чръЁ√Є, хую єЁютхэ№ )
//    т) ╧ю¤Єюьє ўрёЄ№ яЁюёЄЁрэЄётр ЁхчхЁтшЁєхЄё  
//
// ─ы  ёюїЁрэхэш  ъюэЄхъёЄр яЁюЎхёёр шёяюы№чєхЄё  сыюъ шч 40 срщЄют.
// ▌ЄюЄ сыюъ сєфхь эр¤√трЄ№ тхышўшэющ Єшяр CONTEXT :
// ╤ЄЁєъЄєЁр сыюър-╩╬═╥┼╩╤╥└ юяЁхфхыхэр Єшяюь:
// Structure of CONTEXT block :
 typedef  struct  _CNTXT {/* SLOVO - Item of dictionary */
	//  csp, opsp, csb, opsb, OwnBase, DBAdr, TypeAdr, ...
    lptr    _csp, _opsp, _csb, _opsb ;
	int32   _OwnBase;        
	lptr	_DBAdr  ;	// pointer to Data Body
	lptr	_TypeAdr;	// pointer to Type Decsriptor
	bool	_IEFlag ;	// interrupt enable flag
	byte	_Reserv[10];  // reserve for future
} CNTXT, *pCNTXT ;

// ╤ЄЁєъЄєЁр ╨└┴╬╫┼╔ ╬┴╦└╤╥╚ ╧╨╬╓┼╤╤└ :
// Structure of Work Space of process :
// |operand stack -> ... <- control stack| own region... |
// ^-Addr         Len                csb-^-OwnBase OwnLen 
    
//*****************************************************************
//***   NEWCONTEXT - ╩╬╠└═─└ ╚═╚╓╚└╦╚╟└╓╚╚ ═╬┬╬├╬ ╩╬═╥┼╩╤╥└    **** 
//***************************************************************** 
// ╬┴╟└╟┼╓ т√чютр: 
//          [ PRA, CA, Addr, Len, OwnLen ] NEWCONTEXT [ ]
// уфх :
//  PRA - рфЁхё яЁюЎхфєЁ√, юяЁхфхы ■∙хщ Єхыю яЁюЎхёёр 
//   ( ¤Єю Єхыю фюыцэю с√Є№ шэЄхЁяЁхЄшЁєхь√ь, Є.х. т яЁюЎ.ъюфх )         
//  CA - рфЁхё сыюър-ъюэЄхъёЄр, ъюЄюЁ√щ эрфю яЁюшэшЎшрышчшЁютрЄ№
//  Addr,Len - рфЁхё ш фышэр Ёрсюўхщ юсырёЄш, юЄтюфшьющ яюф ёЄхъш 
//  OwnLen - фышэр Own-юсырёЄш  
//
 void _NewContext (void) {     // NEWCONTEXT
	pCNTXT pCA;	// pointer to structure of new context-block
	int32 Addr;		// DSSP-address for workspace of new process
	lptr SaveCsp;	// for saving csp
	int32 i,n;		// for copying bytes in own region

	pCA = (pCNTXT)ADR_ML(AITEM(3));
	Addr= AITEM(2);

	// let's set fields of new CNTXT structure :

	// opsb, opsp
	pCA->_opsb= (lptr)Addr;
	pCA->_opsp= (lptr)Addr;

	// csb, OwnBase
    Addr = Addr + AITEM(1) ; // Addr + Len
	pCA->_csb = (lptr)Addr ;
	pCA->_OwnBase= Addr;

	// prepare control stack and set its pointer :
	SaveCsp = csp ; // save csp
	csp= (lptr)Addr;
	Put_BTrap ();		// prepare Bottom Trap
	CPUSH((lptr)AITEM(4)+1);	// prepare pc 
	pCA->_csp= csp;
	csp = SaveCsp ;  // restore csp

	//  now initiate other fields of CNTXT :

	// registers for data : DBAdr, TypeAdr 
	// don't need to be initiated

	//      ............     (for future)
  	//// initiate interrupt flag of new process 
	//// to be the same as that of current process
	pCA->_IEFlag= IEFlag;

	// now let's initiate region of own variables
	//// to be the same as that of current process
    n= AT;
    // copy n bytes from OwnBase to Addr
    for ( i=0; i<n; i++ )
       MB(Addr+i)= MB(OwnBase+i);

	ADELn(5); // delete parameters of NEWCONTEXT from stack

 }//_NewContext

//*******************************************************************
//***     TRANSFER  -  ╩╬╠└═─└ ╧┼╨┼╩╦▐╫┼═╚▀ ╩╬═╥┼╩╤╥╬┬      *********
//*******************************************************************
// ╬┴╟└╟┼╓ т√чютр: 
//                 [ CA, CB ]  TRANSFER [CC]
// уфх :
// CA - рфЁхё сыюър фы  ёюїЁрэхэш  Єхъє∙хую ъюэЄхъёЄр
//  CB - рфЁхё сыюър-ъюэЄхъёЄр, эр ъюЄюЁ√щ эрфю яхЁхъы■ўшЄ№ё 
//  CC - рфЁхё сыюър-ъюэЄхъёЄр, ё ъюЄюЁюую ёхщўрё яхЁхъы■ўшышё№
// ╟рьхЄшь, ўЄю CC ьюцхЄ эх юс чрЄхы№эю ёютярфрЄ№ ё ╤B
//
void _Transfer (void) {     // TRANSFER
	pCNTXT	pCB,pCA;

	APOP(pCB);
	pCB=(pCNTXT)ADR_ML(pCB); // pointer to new context structure
	pCA=(pCNTXT)ADR_ML(AT); // pointer to current context structure

	//------------ let's save current CONTEXT in pCA-> structure
	// opsp, pc,csp
	CPUSH(pc);
	pCA->_opsp= opsp;
	pCA->_csp = csp ;

	// opsb, csb, OwnBase
	// this saving is not necessary :
	pCA->_opsb= opsb;
	pCA->_csb = csb ;
	pCA->_OwnBase= OwnBase;
	// because they don't been modify during process functionality

	//  now save other fields of CNTXT :

	// registers for data :
	pCA->_DBAdr  = DBAdr  ;
	pCA->_TypeAdr= TypeAdr;

	//      ............     (for future saving)
  	//// save interrupt flag of process 
	pCA->_IEFlag= IEFlag;

	// ------------ let's restore new CONTEXT in pCB-> structure
	// in the same order :

	// opsp, pc,csp
	opsp= pCB->_opsp ;
	csp = pCB->_csp  ;
	CPOP(pc);

	// opsb, csb, OwnBase,
	opsb= pCB->_opsb ;
	csb = pCB->_csb ;
	OwnBase = pCB->_OwnBase;

	//  now restore  other fields of CNTXT :

	// registers for data :
	DBAdr  = pCB->_DBAdr ;
	TypeAdr= pCB->_TypeAdr;
	
	//      ............ (for future saving)
  	//// restore interrupt flag of process 
	IEFlag= pCB->_IEFlag;
	  // and test state of interrupt handling
	TEST_INTR();  
	  // if IFlag then   ENABLE DSSP-interrupts     
	  //           else DISABLE DSSP-interrupts     

	// set parameter CC as DSSP-address of old context-block
	AT= DSSPADR(pCA);
}// _Transfer

//******************************************************************
//****   INITMAIN - ╚═╚╓╚└╦╚╟└╓╚▀ ╩╬═╥┼╩╤╥└ ├╦└┬═╬├╬ ╧╨╬╓┼╤╤└  ***** 
//****************************************************************** 
// ╬┴╟└╟┼╓ т√чютр: 
//                    [ CMAIN ] MAINCONTEXT [ ]
// уфх CMAIN - рфЁхё сыюър-ъюэЄхъёЄр фы  уыртэюую яЁюЎхёёр

void _MainContext (void) {     // MAINCONTEXT
	pCNTXT pCA;	// pointer to main context structure
	APOP(pCA); pCA= (pCNTXT)ADR_ML(pCA); 

	//----------- let's store current CONTEXT in pCA-> structure
	// opsp, pc,csp
	pCA->_opsp= opsp;
	CPUSH(pc);
	pCA->_csp = csp ;
	CPOP(pc);

	// opsb, csb, OwnBase
	pCA->_opsb= opsb;
	pCA->_csb = csb ;
	pCA->_OwnBase= OwnBase;

	//  now store other fields of CNTXT :

	// registers for data :
	pCA->_DBAdr  = DBAdr  ;
	pCA->_TypeAdr= TypeAdr;

	//      ............     (for future saving)
  	//// store interrupt flag of process 
	pCA->_IEFlag = IEFlag;

}// _MainContext

//*******************************************************************
//*** FORCECONTEXT - ╩╬╠└═─└ ┬╤╥└┬╩╚ ┬█╟╬┬└ ╧╨╬╓┼─╙╨█ т ╩╬═╥┼╩╤╥ **** 
//******************************************************************* 
// ╬┴╟└╟┼╓ т√чютр: 
//          [ PROC, CA ] FORCECONTEXT [ ]
// уфх :
//  PROC - рфЁхё Єхыр яЁюЎхфєЁ√, ъюЄюЁє■ эрфю чрёЄртшЄ№ т√чтрЄ№ 
//   ( ¤Єю Єхыю фюыцэю с√Є№ шэЄхЁяЁхЄшЁєхь√ь, Є.х. т яЁюЎ.ъюфх )         
//  CA - рфЁхё сыюър-ъюэЄхъёЄр, т ъюЄюЁюь эрфю юсхёяхўшЄ№ ¤ЄюЄ т√чют 
//

void _ForceContext (void) {     // FORCECONTEXT
	lptr	cmdProc;
	pCNTXT pCA;	// pointer to forced context-block
	APOP(pCA); pCA= (pCNTXT)ADR_ML(pCA); 
	APOP(cmdProc);
	cmdProc++;  // address of the begining of threaded code body 
	// push it in control stack of forced context :
	ML(--pCA->_csp)= (int32)cmdProc;
}
	
/*****************************************************************/