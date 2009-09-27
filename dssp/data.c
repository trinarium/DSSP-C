/*********************************************************/
/***     KERNEL for DSSP INTERPRETER  III VARIANT      ***/
/***  -----------------------------------------------  ***/
/***           DATA STRUCTURE  OPERATIONS              ***/
/***  -----------------------------------------------  ***/
/***         started by Burtsev A.A. 23 march    1998  ***/
/***              last modification  17 november 1998  ***/
/*********************************************************/

////////////////////   DSSP  DATA  REGISTERS  ////////////////////
// Global variables, which is valid during data access operation
lptr    DBAdr  ;    // pointer to Data Body
lptr    TypeAdr;    // pointer to Type Decsriptor
// And address of data is pushed in A-stack
//////////////////////////////////////////////////////////////////

////////////   DATA  REGISTERS  ACCESS   OPERATIONS    ///////////

void _GetDBAdr (void)  {   //  @DATA
    APUSH(DBAdr);  // pointer to Data Body of Variable
}

void _SetDBAdr (void)  {   //  !DATA
    DBAdr= (lptr)ATPOP; // set pointer to Data Body of Variable
}

void _GetTypeAdr (void)    {   //  @TYPE
    APUSH(TypeAdr);    // pointer to  Body of Type Descriptor
}

 void _SetTypeAdr (void)   {   //  !TYPE
    TypeAdr= (lptr)ATPOP;  // set pointer to Body of Type Descriptor
}

/********************** CONST BODY **********************/
//
//  | CNSTBODY |  const value |
//      ^- cmd
void _CnstBody (void) {
     APUSH(ML(cmd+1));
}

/********************** TEXT CONST BODY ******************/
//
//  | fn_TxtCnst |  string address | string length |
//     ^- cmd
void _TxtCnst (void) {
     APUSH(ML(cmd+1));
     APUSH(ML(cmd+2));
}

/******************* PRINT TEXT CONST BODY ****************/
//
//  | fn_PTxtCnst |  string address | string length |
//     ^- cmd
void _PTxtCnst (void) {
    _TxtCnst();
     EXECMD_(ML(Adr_TOS)); // execute TOS operation
}

/********************  BODY of VAR DATA *****************/
//
//  fn_GetAdr | VARCODE (fn_VARNAME) | V_TYPE | V_ADDRESS | ...
//                ^- cmd

 /*------ Offsets of Fields in VARiable Descriptor --------*/

//#define   V_ADRFLAG   (-1)    // Flag of memory mode:
//                            // =0 (global), =1 (own)
#define V_GETADR    (-1)    // offset of GetAdr operation
     // V_NAME      (0)     // pointer to OnName operation
#define V_TYPE      (+1)    // pointer to type descriptor
#define V_ADDRESS   (+2)    // address of data
#define V_COUNT     (+3)    // number of elements (only for array)
#define V_TYPEI     (+4)    // pointer to type of item (for array)
#define V_SIZE      (+5)    // size of data item in bytes (for array)
#define V_DIMENSION (+6)    // dimension (number of indexes of array)
    // for universal array it is followed by block
    // of the limits L1,..,Lk for every index
#define V_LIMITS    (+7)    // begining of block of index limits


/*------- Offsets of Fields in TYPE Descriptor  --------*/

#define T_SIZE  (-1)    // size of data item in bytes
//      T_NAME  (0)     // default operation (@)
#define T_ST    (+1)    // operation ! (to store value)
#define T_ADR   (+2)    // operation ' (to get address)
#define T_ALLSET (+3)   // operation !!! (to set value for all )
#define T_ST1   (+4)    // operation !1 (to set value 1 )
#define T_ST0   (+5)    // operation !0 (to set value 0 )
#define T_ADD   (+6)    // operation !+ (to add )
#define T_SUB   (+7)    // operation !- (to subtract )
#define T_DIMQ  (+8)    // operation DIM? (number of items )
#define T_SIZEQ (+9)    // operation SIZE? ( size of one item)
#define T_INC   (+10)   // operation !1+ (to increment )
#define T_DEC   (+11)   // operation !1- (to decrement )
#define T_CAPQ  (+12)   // operation CAP? (size of whole data )
                        // (for record/structure)

/* ---- MACROs for DATA ACCESS OPERATIONs  ****/

   // Set cmd = DSSP-address of body of variable
#define DA_SETDB    DBAdr=(lptr)DATADR(cmd)

   // call GetVarAdr function
#define DA_GETADR   EXECMD_(ML(DBAdr+V_GETADR))
//#define   DA_GETADR   GetDataAdr() // OLD Variant
   // Set TypeAdr = DSSP-address of type descriptor
#define DA_SETTYPE  TypeAdr=(lptr)DATADR(ML(DBAdr+V_TYPE))
   // call Operation function N
#define DA_EXEC(N)  EXECMD_(ML(TypeAdr+(N)))

// Execute prefix Data-operation N -------- //
// ( address of data body is placed in next command location )
void pDA_Operation (int32 N)   {
   GETCMD;
   DA_SETDB;
   DA_GETADR;
   DA_SETTYPE;
   DA_EXEC(N);
}

// Execute Data operation N ------//
// (, taking address of data body from A-stack )
void sDA_Operation (int32 N)   {
   cmd= (lptr)ATPOP;
   DA_SETDB;
   DA_GETADR;
   DA_SETTYPE;
   DA_EXEC(N);
}

//------------  Operation, called by var Name ------------//
void _VARNAME (void)   {   // @Data - On-name operation
   DA_SETDB;
   DA_GETADR;
   DA_SETTYPE;
   DA_EXEC(0);
}

//--------------  PREFIX DATA OPERATIONS :         ----------//

void _VARST (void) {   // !
   // [V] ! X  []  // assign X:=V
   pDA_Operation(T_ST);
}
void _VARADRD (void)    {   // '
   // [] ' X  ['X]  // address of X
   pDA_Operation(T_ADR);
}
void _VARALLSET (void) {   // !!!
   // [V] !!! X  []  // assign V for every item of X
   pDA_Operation(T_ALLSET);
}
void _VARST1 (void)    {   // !1
       // [] !1 X  []  //  X := 1
   pDA_Operation(T_ST1);
}
void _VARST0 (void)    {   // !0
       // [] !0 X  []  //  X := 0
   pDA_Operation(T_ST0);
}
void _VARADD (void)    {   // !+
       // [V] !+ X  []  //  X := X+V
   pDA_Operation(T_ADD);
}
void _VARSUB (void)    {   // !-
       // [V] !- X  []  //  X := X-V
   pDA_Operation(T_SUB);
}
void _VARDIMQ (void)   {   // DIM?
       // [] DIMQ? X  [Dim]  //
   pDA_Operation(T_DIMQ);
}
void _VARSIZEQ (void)  {   // SIZE?
       // [] SIZEQ? X  [Size]  //
   pDA_Operation(T_SIZEQ);
}
void _VARINC (void)    {   // !1+
       // [] !1+ X  []  //  X := X+1
   pDA_Operation(T_INC);
}
void _VARDEC (void)    {   // !1-
       // [] !1- X  []  //  X := X-1
   pDA_Operation(T_DEC);
}
void _VARCAPQ (void)   {   // CAP?
       // [] CAPQ? X  [Cap]  //
   pDA_Operation(T_CAPQ);
}

//--------------  A-Stack DATA OPERATIONS :   ----------//
void _VARDIMQs (void)   {   // DIM?'
       // [] '' X DIMQ?'  [Dim]  //
   sDA_Operation(T_DIMQ);
}
void _VARSIZEQs (void) {   // SIZE?'
       // [] '' X SIZEQ?' [Size]  //
   sDA_Operation(T_SIZEQ);
}

//-------------  Get Var address Operation --------------//

void _GETADR (void)    {   //  GET adress of normal data
   // cmd points to body of variable
   APUSH( DATADR(ML(DBAdr+V_ADDRESS)) );
}

/*
void _GETADR_OWN_ (void)   {   //  GET adress of own data
   // cmd points to body of variable
   APUSH( DATADR(ML(DBAdr+V_ADDRESS))+ OwnBase );
}
*/
void _GETADR_OWN (void)    {   //  GET adress of own data - BEG_OWN
   // cmd points to body of variable
   APUSH( DATADR(ML(DBAdr+V_ADDRESS))+ OwnBase );
}

void _GETADR_REL (void)    {   //  GET adress of relative data
   // cmd points to body of variable
   AT = AT + ML(DBAdr+V_ADDRESS); // relatively A-top
}

// //  Get address of data and push it in A-stack
//  void GetDataAdr (void)  {
//  // cmd points to body of variable
//  if ( DATADR(ML(DBAdr+V_ADRFLAG))==0 ) {
//      // global data
//      APUSH( DATADR(ML(DBAdr+V_ADDRESS)) );
//  } else {
//      // own data
//      APUSH( DATADR(ML(DBAdr+V_ADDRESS))+ OwnBase );
//  }
//  }

//////////////////////////////////////////////////////////////
///////////////     SCALAR TYPE DESCRIPTORS :   //////////////
//////////////////////////////////////////////////////////////


//--------------  TYPE  Long Int VAR  --------------//
 cmnd LVarType [ ] = {  /* body of Descriptor for Long Var */
           4,  // size of data
   cmd_LVarVal, // or cmd_LVal // for operation on Name
   cmd_LVarSt, // or cmd_TL    // for operation !
   cmd_VarAdr, // or cmd_Nop   // for operation '
   cmd_LVarSt,                 // for operation !!!
   cmd_LVarSt1,                // for operation !1
   cmd_LVarSt0,                // for operation !0
   cmd_LVarAdd,                // for operation !+
   cmd_LVarSub,                // for operation !-
   cmd_T1,                     // for operation DIM?
   cmd_T4,                     // for operation SIZE?
   cmd_LVarInc,                // for operation !1+
   cmd_LVarDec,                // for operation !1-
   cmd_T4                      // for operation CAP?
 };
//------- Specifical Operations :
void _VarAdr (void)    {   // Push Address of Var in A-stack
    // like NOP
}
void _LVarVal (void)   {   // Get Long Var value
    _LVal();   // like @L
   // AT= ML(AT);
}
void _LVarSt (void)    {   // Set (store) Long Var value
   _TL();  // like !TL
   // ML(AT)= AITEM(1);
   // ADELn(2);
}
void _LVarSt1 (void)   {   // Assign Var value 1
    ML(ATPOP)= (1);
}
void _LVarSt0 (void)   {   // Assign Var value 0
    ML(ATPOP)= (0);
}
void _LVarAdd (void)   {   // Add value to Var
    ML(AT)+= AITEM(1);
    ADELn(2);
}
void _LVarSub (void)   {   // Sub value from Var
    ML(AT)-= AITEM(1);
    ADELn(2);
}
void _LVarInc (void)   {   // Add 1 to Var
    ML(ATPOP)+= (1);
}
void _LVarDec (void)   {   // Sub 1 from Var
    ML(ATPOP)-= (1);
}

//--------------  TYPE  ACT VAR  --------------//
 cmnd AVarType [ ] = {  /* body of Descriptor for Act Var */
           4,  // size of data
   cmd_AVarVal, // execute     // for operation on Name
   cmd_LVarSt, // or cmd_TL    // for operation !
   cmd_VarAdr, // or cmd_Nop   // for operation '
   cmd_LVarSt,                 // for operation !!!
   cmd_LVarSt1,                // for operation !1
   cmd_LVarSt0,                // for operation !0
   cmd_LVarAdd,                // for operation !+
   cmd_LVarSub,                // for operation !-
   cmd_T1,                     // for operation DIM?
   cmd_T4,                     // for operation SIZE?
   cmd_LVarInc,                // for operation !1+
   cmd_LVarDec,                // for operation !1-
   cmd_T4                      // for operation CAP?
 };
//------- Specifical Operations :
void _AVarVal (void)   {   // Execute Procedure of Act Var
    EXECMD_(ML(ATPOP)); // execute proc, which address is the Var value
}

//--------------  TYPE  Word Int VAR  --------------//
 cmnd WVarType [ ] = {  /* body of Descriptor for Word Var */
           2,  // size of data
   cmd_WVarVal,// or cmd_WVal  // for operation on Name
   cmd_WVarSt, // or cmd_TW    // for operation !
   cmd_VarAdr, // or cmd_Nop   // for operation '
   cmd_WVarSt,                 // for operation !!!
   cmd_WVarSt1,                // for operation !1
   cmd_WVarSt0,                // for operation !0
   cmd_WVarAdd,                // for operation !+
   cmd_WVarSub,                // for operation !-
   cmd_T1,                     // for operation DIM?
   cmd_T2,                     // for operation SIZE?
   cmd_WVarInc,                // for operation !1+
   cmd_WVarDec,                // for operation !1-
   cmd_T2                      // for operation CAP?
 };
//------- Specifical Operations :
void _WVarVal (void)   {   // Get Word Var value
   _WVal();  // like @W
}
void _WVarSt (void)    {   // Set (store) Word Var value
   _TW(); // like !TW
}
void _WVarSt1 (void)   {   // Assign Var value 1
    MW(ATPOP)= (1);
}
void _WVarSt0 (void)   {   // Assign Var value 0
    MW(ATPOP)= (0);
}
void _WVarAdd (void)   {   // Add value to Var
    MW(AT)+= (word)AITEM(1);
    ADELn(2);
}
void _WVarSub (void)   {   // Sub value from Var
    MW(AT)-= (word)AITEM(1);
    ADELn(2);
}
void _WVarInc (void)   {   // Add 1 to Var
    MW(ATPOP)+= (1);
}
void _WVarDec (void)   {   // Sub 1 from Var
    MW(ATPOP)-= (1);
}

//--------------  TYPE  Int VAR (signed) --------------//
 cmnd IVarType [ ] = {  /* body of Descriptor for Word Var */
           2,  // size of data
   cmd_IVarVal,                // for operation on Name
   cmd_WVarSt, // or cmd_TW    // for operation !
   cmd_VarAdr, // or cmd_Nop   // for operation '
   cmd_WVarSt,                 // for operation !!!
   cmd_WVarSt1,                // for operation !1
   cmd_WVarSt0,                // for operation !0
   cmd_WVarAdd,                // for operation !+
   cmd_WVarSub,                // for operation !-
   cmd_T1,                     // for operation DIM?
   cmd_T2,                     // for operation SIZE?
   cmd_WVarInc,                // for operation !1+
   cmd_WVarDec,                // for operation !1-
   cmd_T2                      // for operation CAP?
 };
//------- Specifical Operations :
void _IVarVal (void)   {   // Get signed Word Var value
   AT= (int16)MW(AT); // signed 16-bit integer !
}

//--------------  TYPE  BYTE VAR  --------------//
 cmnd BVarType [ ] = {  /* body of Descriptor for Byte Var */
           1,  // size of data
   cmd_BVarVal,// or cmd_BVal  // for operation on Name
   cmd_BVarSt, // or cmd_TB    // for operation !
   cmd_VarAdr, // or cmd_Nop   // for operation '
   cmd_BVarSt,                 // for operation !!!
   cmd_BVarSt1,                // for operation !1
   cmd_BVarSt0,                // for operation !0
   cmd_BVarAdd,                // for operation !+
   cmd_BVarSub,                // for operation !-
   cmd_T1,                     // for operation DIM?
   cmd_T1,                     // for operation SIZE?
   cmd_BVarInc,                // for operation !1+
   cmd_BVarDec,                // for operation !1-
   cmd_T1                      // for operation CAP?
 };
//------- Specifical Operations :
void _BVarVal (void)   {   // Get Byte Var value
   _BVal();  // like @B
}
void _BVarSt (void)    {   // Set (store) Byte Var value
   _TB(); // like !TB
}
void _BVarSt1 (void)   {   // Assign Var value 1
    MB(ATPOP)= (1);
}
void _BVarSt0 (void)   {   // Assign Var value 0
    MB(ATPOP)= (0);
}
void _BVarAdd (void)   {   // Add value to Var
    MB(AT)+= (byte)AITEM(1);
    ADELn(2);
}
void _BVarSub (void)   {   // Sub value from Var
    MB(AT)-= (byte) AITEM(1);
    ADELn(2);
}
void _BVarInc (void)   {   // Add 1 to Var
    MB(ATPOP)+= (1);
}
void _BVarDec (void)   {   // Sub 1 from Var
    MB(ATPOP)-= (1);
}

//--------------  TYPE  SBYTE  VAR (Signed BYTE) ------------//
 cmnd SBVarType [ ] = {  /* body of Descriptor for SByte Var */
           1,  // size of data
   cmd_SBVarVal,// or cmd_BVal // for operation on Name
   cmd_BVarSt, // or cmd_TB    // for operation !
   cmd_VarAdr, // or cmd_Nop   // for operation '
   cmd_BVarSt,                 // for operation !!!
   cmd_BVarSt1,                // for operation !1
   cmd_BVarSt0,                // for operation !0
   cmd_BVarAdd,                // for operation !+
   cmd_BVarSub,                // for operation !-
   cmd_T1,                     // for operation DIM?
   cmd_T1,                     // for operation SIZE?
   cmd_BVarInc,                // for operation !1+
   cmd_BVarDec,                // for operation !1-
   cmd_T1                      // for operation CAP?
 };
//------- Specifical Operations :
void _SBVarVal (void)  {   // Get SByte Var value
   AT= (int8)MB(AT); // signed byte !
}

//////////////////////////////////////////////////////////////
//////////   UNIVERSAL  VECTOR TYPE DESCRIPTOR :   //////////
//////////////////////////////////////////////////////////////

// Set TypeAdr = DSSP-address of type descriptor of element
#define DA_SETTYPEI TypeAdr=(lptr)DATADR(ML(DBAdr+V_TYPEI))

void _TstVctrInd (void) {  // Test index out of range
   if ( AITEM(1)>=ML(DBAdr+V_COUNT) ) {
       printf(" index out of range \n");
       exit(77);   // Wrong Index !!!!!
   }
}
void _VctrAdrI (void)  { // Calculate Address of Vctr[i]
   // AITEM(1) = index; AT= base address of Vector
   _TstVctrInd();
   AITEM(1)=AT+AITEM(1)*ML(DBAdr+V_SIZE);
   ADEL;
   // AT= address of item Vector[index]
}
// Execute prefix Data Access operation N for VECTOR -------- //
// DBAdr points to body of vector variable
// TypeAdr points to type descriptor
// Top points to location of begining of vector
// SubTop = index of element (i), if needed
void pVA_Operation (int32 N)   {
   _VctrAdrI();
   DA_SETTYPEI;
   DA_EXEC(N);
}

/* body of Descriptor for Universal Vector */
cmnd VctrType [ ] = {
          0,  // size of data
  cmd_VctrVal,    // for operation on Name
  cmd_VctrSt,     // for operation !
  cmd_VctrAdr,    // for operation '
  cmd_VctrAllSet, // for operation !!!
  cmd_VctrSt1,    // for operation !1
  cmd_VctrSt0,    // for operation !0
  cmd_VctrAdd,    // for operation !+
  cmd_VctrSub,    // for operation !-
  cmd_VDimQ,      // for operation DIM?
  cmd_VSizeQ,     // for operation SIZE?
  cmd_VctrInc,    // for operation !1+
  cmd_VctrDec,    // for operation !1-
  cmd_VCapQ       // for operation CAP?
};
//------- Specifical VECTOR Operations :
void _VctrVal (void)   { // Get Value of element [i] of vector
   // [i] X  [ X(i)]
   pVA_Operation(0);
}
void _VctrSt (void)    {   // ! Assign value Vctr[i]
   // [V,i] ! X  []  // assign X[i]:=V
   pVA_Operation(T_ST);
}
void _VctrAdr (void)   {   // '
   // [V,i] ' X  ['X(i)]
   pVA_Operation(T_ADR);
}
void _VctrSt1 (void)   {   // Assign value 1 to Vctr[i]
   pVA_Operation(T_ST1);
}
void _VctrSt0 (void)   {   // Assign value 0 to Vctr[i]
   pVA_Operation(T_ST0);
}
void _VctrAdd (void)   {   // Add value to Vctr[i]
   pVA_Operation(T_ADD);
}
void _VctrSub (void)   {   // Sub value from Vctr[i]
   pVA_Operation(T_SUB);
}
void _VctrInc (void)   {   // Add 1 to Vctr[i]
   pVA_Operation(T_INC);
}
void _VctrDec (void)   {   // Sub 1 from Vctr[i]
   pVA_Operation(T_DEC);
}
void _VctrAllSet (void) {  // Assign value to every item Vctr[i]
   // Top= base address of Vector, SupTop = value to be assigned
   int32 i,cnt,size,A,V;
   cnt=ML(DBAdr+V_COUNT);
   size=ML(DBAdr+V_SIZE);
   DA_SETTYPEI;
   A= ATPOP; V= ATPOP;
   for (i=0; i<cnt; i++) {
       APUSH(V); APUSH(A+i*size);
       DA_EXEC(T_ALLSET);  // !!! for Vctr[i]
   }
}
void _VDimQ (void) {   // number of item in Vector
   // AT= base address of Vector
   AT= ML(DBAdr+V_COUNT);
}
void _VSizeQ (void)    {   // size of one item of Vector
   // AT= base address of Vector
   AT= ML(DBAdr+V_SIZE);
}
void _VCapQ (void) {   // size of whole Vector
   // AT= base address of Vector
    AT = ML(DBAdr+V_COUNT)*ML(DBAdr+V_SIZE);
}

//////////////////////////////////////////////////////////////
//////////   UNIVERSAL  ARRAY TYPE DESCRIPTOR :     //////////
//////////////////////////////////////////////////////////////

void _ArrAdrI (void)   { // Calculate Address of Arr[i1,..,ik]
   // AITEM(p) = index (ip) of item, p=1,..,k
   // AT= base address of Vector
   int32 p,i,k;
   k= ML(DBAdr+V_DIMENSION);
   i=0; // calculate I= (...(I1*L2+I2)*L2+...)*Lk+Ik
   for (p=1; p<=k; p++) {
       i= i*ML(DBAdr+V_LIMITS+p-1)+AITEM(p);
       // Test index out of range
       if ( AITEM(p)>=ML(DBAdr+V_LIMITS+p-1) ) {
           printf(" index out of range \n");
           exit(77);   // Wrong Index !!!!!
       }//if
   }//for
   AITEM(k)=AT+i*ML(DBAdr+V_SIZE);
   ADELn(k);
   // AT= address of item Arr[i1,..,ik]
}
// Execute prefix Data Access operation N for ARRAY -------- //
// DBAdr points to body of vector variable
// TypeAdr points to type descriptor
// Top points to location of begining of vector
// AITEM(p) = index (ip) of item, p=1,..,k
void pAA_Operation (int32 N)   {
   _ArrAdrI();
   DA_SETTYPEI;
   DA_EXEC(N);
}

/* body of Descriptor for Universal Array */
cmnd ArrType [ ] = {
          0,  // size of data
  cmd_ArrVal,     // for operation on Name
  cmd_ArrSt,      // for operation !
  cmd_ArrAdr,     // for operation '
  cmd_VctrAllSet,  // for operation !!!
  cmd_ArrSt1,     // for operation !1
  cmd_ArrSt0,     // for operation !0
  cmd_ArrAdd,     // for operation !+
  cmd_ArrSub,     // for operation !-
  cmd_VDimQ,      // for operation DIM?
  cmd_VSizeQ,     // for operation SIZE?
  cmd_ArrInc,     // for operation !1+
  cmd_ArrDec,     // for operation !1-
  cmd_VCapQ       // for operation CAP?
};
//------- Specifical ARRAY Operations :
void _ArrVal (void)    { // Get Value of element Arr[i1,..,ik]
   // [i] X  [ X(i)]
   pAA_Operation(0);
}
void _ArrSt (void) {   // !  Assign value to Arr[i1,..,ik]
   // [V,i] ! X  []  // assign X[i]:=V
   pAA_Operation(T_ST);
}
void _ArrAdr (void)    {   // '
   // [V,i] ' X  ['X(i)]
   pAA_Operation(T_ADR);
}
void _ArrSt1 (void)    {   // Assign value 1 to Arr[i1,..,ik]
   pAA_Operation(T_ST1);
}
void _ArrSt0 (void)    {   // Assign value 0 to Arr[i1,..,ik]
   pAA_Operation(T_ST0);
}
void _ArrAdd (void)    {   // Add value to Arr[i1,..,ik]
   pAA_Operation(T_ADD);
}
void _ArrSub (void)    {   // Sub value from Arr[i1,..,ik]
   pAA_Operation(T_SUB);
}
void _ArrInc (void)    {   // Add 1 to Arr[i1,..,ik]
   pAA_Operation(T_INC);
}
void _ArrDec (void)    {   // Sub 1 from Arr[i1,..,ik]
   pAA_Operation(T_DEC);
}

//////////////////////////////////////////////////////////////
//// for UNIVERSAL STRUCTURE (RECORD) TYPE DESCRIPTOR : //////
//////////////////////////////////////////////////////////////

/* body of Descriptor for Universal STRUCTURE */
cmnd StructType [ ] = {
          0,  // size of data structure (in bytes)
  cmd_Nop,        // for operation on Name
  cmd_StructSt,   // for operation !
  cmd_Nop,        // for operation '
  cmd_StructSt,   // for operation !!!
  cmd_Nop,        // for operation !1
  cmd_StructSt0,  // for operation !0
  cmd_Nop,        // for operation !+
  cmd_Nop,        // for operation !-
  cmd_T1,         // for operation DIM?
  cmd_StructSize ,// for operation SIZE?
  cmd_Nop,        // for operation !1+
  cmd_Nop,        // for operation !1-
  cmd_StructSize  // for operation CAP?
};
//------- Specifical STRUCTURE Operations :

void _StructSt (void) { // Assign values to Structure from the address
   int32   Adr;
    Adr = AT; AT=ML(TypeAdr+T_SIZE);
    APUSH(Adr);
    // A-Astack contains now: [Address, Number of bytes, StructAddress ]
    _MoveB (); // execute !!!SB
}
void _StructSt0 (void) { // Assign zero to every byte of Structure
   int32   Adr;
    Adr = AT; AT=0;
    APUSH(Adr);
    APUSH(ML(TypeAdr+T_SIZE));
    // A-Astack contains now: [0, Address, Number of bytes ]
    _SetB(); // execute !!!MB
}
void _StructSize (void) { // Assign AT= size of structure (in bytes)
    AT=ML(TypeAdr+T_SIZE);
}

//////////////////////////////////////////////////////////////
////                     STRINGS                        //////
//////////////////////////////////////////////////////////////

/*------ Offsets of Fields in TYPE Descriptor for STRINGs -----*/
// for prefix operations

#define T_STSET  (+1)   // operation !      (to store value)
#define T_STRAD  (+2)   // operation '      (to get address)
#define T_STSETS (+3)   // operation !!!    (to set value for all )
#define T_SOVR   (+4)   // operation SOVR   (to set value 1 )
#define T_SCLR   (+5)   // operation SCLR   (clear string)
#define T_SADD   (+6)   // operation SADD   (add byte)
#define T_SDSB   (+7)   // operation SDSB   (del byte)
#define T_SDIMQ  (+8)   // operation DIM?   (number of items )
#define T_SSIZEQ (+9)   // operation SIZE?  (size of one item)
#define T_SINS   (+10)  // operation SINS   (insert byte)
#define T_SDEL   (+11)  // operation SDEL   (delete byte)
#define T_SCAPQ  (+12)  // operation CAP?   (size of whole data )
#define T_SCUT   (+13)  // operation SCUT
#define T_SISB   (+14)  // operation SISB
#define T_SSSB   (+15)  // operation SSSB
#define T_SSSBI  (+16)  // operation SSSBI
#define T_SLEN   (+17)  // operation SLEN
#define T_SADDB  (+18)  // operation SADDB
#define T_SCMP   (+19)  // operation SCMP   (compare strings)
#define T_SRPL   (+20)  // operation SRPL
#define T_SBYTE  (+21)  // operation SBYTE

//--------------  PREFIX DATA OPERATIONS for STRINGS:    ----------//
void _SByte(void)  {
   pDA_Operation(T_SBYTE);
}
void _SRpl(void)   {
   pDA_Operation(T_SRPL);
}
void _SCmp(void)   {
   pDA_Operation(T_SCMP);
}
void _SAddB(void)  {
   pDA_Operation(T_SADDB);
}
void _SLen(void)   {
   pDA_Operation(T_SLEN);
}
void _SSsbi(void)  {
   pDA_Operation(T_SSSBI);
}
void _SSsb(void)   {
   pDA_Operation(T_SSSB);
}
void _SIsb(void)   {
   pDA_Operation(T_SISB);
}
void _SCut(void)   {
   pDA_Operation(T_SCUT);
}
void _SCap(void)   {
   pDA_Operation(T_SCAPQ);
}
void _SDel(void)   {
   pDA_Operation(T_SDEL);
}
void _SIns(void)   {
   pDA_Operation(T_SINS);
}
// void sVDimQ(void)
// void sVSizeQ(void)
void _SDsb(void)   {
   pDA_Operation(T_SDSB);
}
void _SAdd(void)   {
   pDA_Operation(T_SADD);
}
void _SClr(void)   {
   pDA_Operation(T_SCLR);
}
void _SOvr(void)   {
   pDA_Operation(T_SOVR);
}
// void sStSets(void)
// void sStrAd(void)
// void sStSet(void)
// void sStVal(void)

  /* body of Descriptor for STRINGs */
cmnd StrType[]={  // string description
            0,  // size of data structure (in bytes)
    cmd_sStVal,   // operation on Name;
    cmd_sStSet,
    cmd_sStrAd,
    cmd_sStSets,
    cmd_sSOvr,
    cmd_sSClr,
    cmd_sSAdd,
    cmd_sSDsb,
    cmd_VDimQ,
    cmd_T1,
    cmd_sSIns,
    cmd_sSDel,
    cmd_sSCap,
    cmd_sSCut,
    cmd_sSIsb,
    cmd_sSSsb,
    cmd_sSSsbi,
    cmd_sSLen,
    cmd_sSAddB,
    cmd_sSCmp,
    cmd_sSRpl,
    cmd_sSByte
};

//------- Specifical STRING Operations :

// local vars used during only every string operations
// declared as global vars for optimization reasons

int32 dest,source,lend,lens,len;
int32 i,j,i1,j1,dest1;
char sEnd,dEnd,*result;
char symb;

void sSByte(void)  {
   dest=ATPOP;
   dest+=(4+AT);
   AT=MB(dest);
}

void sSCmp(void)
{dest=ATPOP;
 len=ML(dest);
 dest=dest+(int32)DsspMem+4;
 lend=ML(DBAdr+V_COUNT);
 lens=ATPOP;
 source=ATPOP+(int32)DsspMem;
 sEnd=*(char*)(source+lens);
 *(char*)(source+lens)=0;
 dEnd=*(char*)(dest+len);
 *(char*)(dest+len)=0;
 AINS;
 AT=-(int32)strcmp((char*)dest,(char*)source);
 *(char*)(dest+len)=dEnd;
 *(char*)(source+lens)=sEnd;
}

void sSAddB(void)  {
   dest=ATPOP;
   len=ML(dest);
   lend=ML(DBAdr+V_COUNT);
   if (len<lend)  {
     ML(dest)++;
     dest=dest+(int32)DsspMem+4+len;
     *(char*)dest=(char)ATPOP;
   } else { ADEL;
   }
}

void sSLen(void)
{   AT=ML(AT);
}

void sSSsbi(void)  {
   dest=ATPOP;
   len=ML(dest);
   dest=dest+(int32)DsspMem+4+ATPOP;
   lend=ML(DBAdr+V_COUNT);
   lens=ATPOP;
   source=ATPOP+(int32)DsspMem;
   sEnd=*(char*)(source+lens); *(char*)(source+lens)=0;
   dEnd=*(char*)(dest+len); *(char*)(dest+len)=0;
   result=strstr((char*)dest,(char*)source);
   *(char*)(dest+len)=dEnd;
   *(char*)(source+lens)=sEnd;
   AINS; AT=(int32)result-dest;
   AINS;
   if (result==NULL) AT=0; else AT=1;
}
void sSSsb(void)   {
   dest=ATPOP;
   len=ML(dest);
   dest=dest+(int32)DsspMem+4;
   lend=ML(DBAdr+V_COUNT);
   lens=ATPOP;
   source=ATPOP+(int32)DsspMem;
   sEnd=*(char*)(source+lens); *(char*)(source+lens)=0;
   dEnd=*(char*)(dest+len); *(char*)(dest+len)=0;
   result=strstr((char*)dest,(char*)source);
   *(char*)(dest+len)=dEnd;
   *(char*)(source+lens)=sEnd;
   AINS; AT=(int32)result-dest;
   AINS;
   if (result==NULL) AT=0; else AT=1;
}
void sSIsb(void)   {
   dest=ATPOP;
   j=ATPOP;
   len=ML(dest);
   lend=ML(DBAdr+V_COUNT);
   lens=ATPOP;
   source=ATPOP+(int32)DsspMem;
   if (!(len<j))  {
     ML(dest)=min(lend,len+lens);
     dest=dest+(int32)DsspMem+4;
     dest=dest+j;
     for (i=(lend-j-lens);i>1;i--)
          *((char*)dest+i)=*((char*)dest+i-lens);
     for (i=0;i<min(lens,lend-j);i++)
          *((char*)dest+i)=*((char*)source+i);
   }
}
void sSCut(void)   {
   dest=ATPOP;
   len=ML(dest);
   lens=ATPOP;
   ML(dest)=max(0,len-lens);
}
void sSCap(void)   {
}
void sSDel(void)   {
   dest=ATPOP;
   len=ML(DBAdr+V_COUNT);
   lend=ML(dest);
   if ((!(len<AT))&&(!(lend<AT)))   {
     if (lend>0)
       ML(dest)--;
     dest=dest+(int32)DsspMem+4+AT;
     j=ATPOP;
     for (i=1;i<(lend-j);i++)
          *((char*)dest+i-1)=*((char*)dest+i);
   }  else   {
       ADEL;
   }
}
void sSIns(void)   {
   dest=ATPOP;
   len=ML(DBAdr+V_COUNT);
   lend=ML(dest);
   if (!(len<AT))   {
     if (lend<len)
       ML(dest)++;
     dest=dest+(int32)DsspMem+4+AT;
     j=ATPOP;
     for (i=(lend-j);i>=1;i--)
          *((char*)dest+i)=*((char*)dest+i-1);
     *(char*)dest=(char)ATPOP;
   }  else   {
       ADEL;ADEL;
   }
}
// void sVDimQ(void)
// void sVSizeQ(void)
void sSDsb(void)   {   // [N,I,A]
   dest=ATPOP;
   lend=ML(DBAdr+V_COUNT);
   len=ML(dest);
   if ((!(len<AT))&&(!(lend<AT)))   {
     lens=ATPOP;
     if (lend>0)
       ML(dest)=max(0,len-AT);
     dest=dest+(int32)DsspMem+4+lens;
     j=ATPOP;
     for (i=j;i<(lend-lens);i++)
          *((char*)dest+i-j)=*((char*)dest+i);
   }  else  {
       ADEL;ADEL;
   }
}
void sSAdd(void)   {
   dest=ATPOP;
   len=ML(dest);
   lend=ML(DBAdr+V_COUNT);
   lens=ATPOP;
   source=ATPOP+(int32)DsspMem;
   ML(dest)=min(lend,len+lens);
   dest=dest+(int32)DsspMem+4;
   dest=dest+len;
   for (i=0;i<(lend-len);i++)   {
     *(char*)(dest++)=*(char*)(source++);
   }
}
void sSClr(void)   {
   ML(AT)=0;
   ADEL;
}
void sSRpl(void)   {
   dest1=ATPOP; j1=ATPOP; i1=ATPOP;
   AINS; AT=j1;
   AINS; AT=i1;
   AINS;AT=dest1;
   sSDsb();
   AINS; AT=i1;
   AINS;AT=dest1;
   sSIsb();
}
void sSOvr(void)   {
   dest=ATPOP;
   len=ML(DBAdr+V_COUNT);
   if (!(len<AT))   {
      dest=dest+(int32)DsspMem+ATPOP+4;
      *(char*)dest=(char)ATPOP;
   }  else   {
       ADEL;ADEL;
   }
}
void sStSets(void) {
   dest=ATPOP;
   len=ML(DBAdr+V_COUNT);
   symb=(char)ATPOP;
   ML(dest)=len;
   dest=dest+(int32)DsspMem+4;
   for (i=0;i<len;i++)
     *(char*)(dest++)=symb;
   //strnset((char*)dest,(char)symb,len);
}
void sStrAd(void)  {
   dest=ATPOP;
   dest+=(AT+4);
   AT=dest;
}
void sStSet(void)  {
   dest=ATPOP;
   lend=ML(DBAdr+V_COUNT);
   lens=ATPOP;
   source=(int32)DsspMem+ATPOP;
   len=min(lens,lend);
   ML(dest)=len;
   dest=dest+(int32)DsspMem+4;
   strncpy((char*)dest,(char*)source,len);
}
void sStVal(void)  {
   AT=AT+4;
   APUSHCOPYn(1);
   AT=AT-4;
   AT=ML(AT);
}

//////////////////////////////////////////////////////////////
////       Some operation for command line              //////
//////////////////////////////////////////////////////////////

void _arg2str(void) { // ARG2STR
    GETCMD;
    DA_SETDB;
    DA_GETADR;
    DA_SETTYPE;
    i=0;
    dest=ATPOP;
    lend=ML(DBAdr+V_COUNT);
    ML(dest)=len=0;
    dest+=((int32)DsspMem+4);
    for (j=param_st;j<argc_gl;j++)  {
           len=min((unsigned)(i+strlen(argv_gl[j])),(unsigned)lend);
           strncpy((char*)(dest+i),argv_gl[j],len-i);
           if ((i>=lend) || (j==argc_gl-1)) break;
           i=len;*(char*)(dest+i)=' ';i++;
    }
    dest-=((int32)DsspMem+4);
    ML(dest)=len;
}

void _dpfname(void) { // DPFNAME
    ADEL;
    dest=AT;
    dest+=((int32)DsspMem);
    strncpy((char*)(dest),namep_gl,strlen(namep_gl));
    AINS; AT=strlen(namep_gl);
}