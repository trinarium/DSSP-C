/*********************************************************/
/***     KERNEL for DSSP INTERPRETER  III VARIANT      ***/
/***  -----------------------------------------------  ***/
/***        LIST of COMMAND ADDRESSes                  ***/
/***  -----------------------------------------------  ***/
/***         started by Burtsev A.A.  23 march 1998    ***/
/***              last modification   22 sept  1998    ***/
/*********************************************************/

/*********************************************************/
/****  LIST of MACROs for some important  DSSP-CODES  ****/
/*********************************************************/

#ifndef Watcom
#define INTELO          (FNum)(cmnd)(fn_Int)      // the begin of Dssp-procedure
#define VARTELO         (FNum)(cmnd)(fn_VARNAME)  // the begin of Dssp-Var
#define CNSTBODY        (FNum)(cmnd)(fn_CnstBody) // the begin of Dssp-Const
#endif
#ifdef Watcom
#define INTELO          fn_Int      // the begin of Dssp-procedure
#define VARTELO         fn_VARNAME  // the begin of Dssp-Var
#define CNSTBODY        fn_CnstBody // the begin of Dssp-Const
#endif

/***--------  AGREEMENTS about COMMAND NUMBERs --------***/

//------   START_CMD   - start number for commands
#define CMDADR(NUM)     ((NUM)*4+START_CMD)     // Address of Command Body

/////////////////////////////////////////////////////////////////////
// START_CMD  +0 ... +299 = KERNEL operation commands
//                      0   - main interpretation operations
//                     +10  - control operations
//                     +60  - common A-stack operations
//                     +80  - arithmetic stack operations
//                    +110  - logical and bit operations
//                    +140  - data operations
//                    +170  - data structure (prefix) operations
//                    +200  - input/output operations
//                    +250  - Win32 operations
//                    +280  - debug operations
//                    +290  - coroutine operations
//
//          +300 ... +499 = AUXILIARY operations
//                          (for data descriptors and others)
//          +500 ... +599 - SYSTEM BODies and
//          +600 ... +899 - DATA DESCRIPTORs
//
//START_CMD+999 ... +900  - operations for testing DSSP-kernel
//         +1000...+1100  - TEST BODies
//                          (of procedures and descriptors)
//
/////////////////////////////////////////////////////////////////////

/*********************************************************/
/****   LIST of COMMANDs for DSSP KERNEL-OPERATIONs   ****/
/*********************************************************/

/******      MAIN  INTERPRETATION   OPERATIONS     *******/
#define cmd_Halt        CMDADR(0)
#define cmd_Int         CMDADR(1)
#define cmd_K           CMDADR(2)
#define cmd_Nop         CMDADR(3)
#define cmd_UnOp        CMDADR(4)
#define cmd_UndefOp     CMDADR(5)
#define cmd_Call        CMDADR(6)

/******     CONTROL   OPERATIONS    *******/

#define cmd_Exec        CMDADR(10)
#define cmd_If0         CMDADR(11)
#define cmd_IfMi        CMDADR(12)
#define cmd_IfPl        CMDADR(13)
#define cmd_Br0         CMDADR(14)
#define cmd_BrMi        CMDADR(15)
#define cmd_BrPl        CMDADR(16)
#define cmd_BrS         CMDADR(17)
#define cmd_Br          CMDADR(18)
#define cmd_Else        CMDADR(19)
#define cmd_Br1         CMDADR(20)

        /*---  simple LOOPs  ---*/
#define cmd_DW          CMDADR(21)
#define cmd_Loop        CMDADR(22)
#define cmd_DOFor       CMDADR(23)

/***  Access to body and control stack ***/
#define cmd_GetPC       CMDADR(25)
#define cmd_SetPC       CMDADR(26)
#define cmd_GetR        CMDADR(27)
#define cmd_SetR        CMDADR(28)
#define cmd_GetAdr      CMDADR(29)
#define cmd_GTP         CMDADR(30)

/*--- LOOPs with TRAPs ( RP DO  EX ) ---*/
#define cmd_Rp          CMDADR(31)
#define cmd_Do          CMDADR(32)
#define cmd_Rp1         CMDADR(33)
#define cmd_Do1         CMDADR(34)
#define cmd_Ex          CMDADR(35)
#define cmd_Ex0         CMDADR(36)
#define cmd_ExMi        CMDADR(37)
#define cmd_ExPl        CMDADR(38)

/*--- Mechanism of variable Recovering (LVOST) ---*/
#define cmd_LVost       CMDADR(39)
#define cmd_LVosts      CMDADR(40)

/*--- Mechanism of Exception Handling ---*/
#define cmd_RtL         CMDADR(41)
#define cmd_RtLs        CMDADR(42)
#define cmd_NOn         CMDADR(43)
#define cmd_EOn         CMDADR(44)
#define cmd_ROn         CMDADR(45)
#define cmd_NtfOn       CMDADR(46)
#define cmd_EscOn       CMDADR(47)
#define cmd_RtrOn       CMDADR(48)

#define cmd_Raise_      CMDADR(49)
#define cmd_Raise       CMDADR(50)
#define cmd_ReRaise     CMDADR(51)


/******         COMMON A-STACK OPERATIONS          *******/
#define cmd_Cnst0       CMDADR(60)
#define cmd_Cnst1       CMDADR(61)
#define cmd_Cnst2       CMDADR(62)
#define cmd_Cnst4       CMDADR(63)

#define cmd_C           CMDADR(64)
#define cmd_C2          CMDADR(65)
#define cmd_C3          CMDADR(66)
#define cmd_C4          CMDADR(67)
#define cmd_CT          CMDADR(68)

#define cmd_E2          CMDADR(69)
#define cmd_E3          CMDADR(70)
#define cmd_E4          CMDADR(71)
#define cmd_ET          CMDADR(72)

#define cmd_D           CMDADR(73)
#define cmd_D2          CMDADR(74)
#define cmd_D3          CMDADR(75)
#define cmd_D4          CMDADR(76)
#define cmd_E2D         CMDADR(77)

#define cmd_DS          CMDADR(78)
#define cmd_Deep        CMDADR(79)

/******       ARITHMETIC STACK OPERATIONS          *******/
#define cmd_TN1         CMDADR(80)
#define cmd_T0          CMDADR(81)
#define cmd_T1          CMDADR(82)
#define cmd_T2          CMDADR(83)
#define cmd_T4          CMDADR(84)

#define cmd_Add         CMDADR(85)
#define cmd_Add1        CMDADR(86)
#define cmd_Add2        CMDADR(87)
#define cmd_Add3        CMDADR(88)
#define cmd_Add4        CMDADR(89)

#define cmd_Sub         CMDADR(90)
#define cmd_Sub1        CMDADR(91)
#define cmd_Sub2        CMDADR(92)
#define cmd_Sub3        CMDADR(93)
#define cmd_Sub4        CMDADR(94)

#define cmd_Neg         CMDADR(95)
#define cmd_Abs         CMDADR(96)
#define cmd_Sgn         CMDADR(97)
#define cmd_Min         CMDADR(98)
#define cmd_Max         CMDADR(99)

#define cmd_Mult        CMDADR(100)
#define cmd_Div         CMDADR(101)

/******       LOGICAL and BIT OPERATIONS           *******/
#define cmd_Lt          CMDADR(110)
#define cmd_LE          CMDADR(112)
#define cmd_Eq          CMDADR(113)
#define cmd_Gt          CMDADR(114)
#define cmd_GE          CMDADR(115)
#define cmd_Seg         CMDADR(116)

#define cmd_Not         CMDADR(117)
#define cmd_Inv         CMDADR(118)
#define cmd_And         CMDADR(119)
#define cmd_Or          CMDADR(120)
#define cmd_Xor         CMDADR(121)

#define cmd_Shl         CMDADR(122)
#define cmd_Shr         CMDADR(123)
#define cmd_Sht         CMDADR(124)

#define cmd_SwB         CMDADR(125)
#define cmd_SwW         CMDADR(126)
#define cmd_Lo          CMDADR(127)
#define cmd_Hi          CMDADR(128)
#define cmd_SetLo       CMDADR(129)
#define cmd_SetHi       CMDADR(130)
#define cmd_SgX         CMDADR(131)

/******           DATA   OPERATIONS                *******/
#define cmd_Cnst        CMDADR(140)

#define cmd_LVal        CMDADR(141)
#define cmd_WVal        CMDADR(142)
#define cmd_BVal        CMDADR(143)
#define cmd_CLVal       CMDADR(144)
#define cmd_CWVal       CMDADR(145)
#define cmd_CBVal       CMDADR(146)

#define cmd_TL          CMDADR(147)
#define cmd_TW          CMDADR(148)
#define cmd_TB          CMDADR(150)
#define cmd_TL_         CMDADR(151)
#define cmd_TW_         CMDADR(152)
#define cmd_TB_         CMDADR(153)

#define cmd_ValBit      CMDADR(154)
#define cmd_StBit       CMDADR(155)
#define cmd_StBit1      CMDADR(156)
#define cmd_StBit0      CMDADR(157)

#define cmd_SetL        CMDADR(158)
#define cmd_SetW        CMDADR(160)
#define cmd_SetB        CMDADR(161)
#define cmd_MoveL       CMDADR(162)
#define cmd_MoveW       CMDADR(163)
#define cmd_MoveB       CMDADR(164)
#define cmd_SrchB       CMDADR(165)
#define cmd_isk         CMDADR(166)

/******      DATA  STRUCTURE OPERATIONS             *******/

        /*--  Data Registers Access Operations  --*/
#define cmd_GetDBAdr    CMDADR(170)
#define cmd_SetDBAdr    CMDADR(171)
#define cmd_GetTypeAdr  CMDADR(172)
#define cmd_SetTypeAdr  CMDADR(173)

        /*-- Prefix Data Operations  --*/
#define cmd_VARST               CMDADR(174)
#define cmd_VARADR              CMDADR(175)
#define cmd_VARALLSET   CMDADR(176)
#define cmd_VARST1              CMDADR(177)
#define cmd_VARST0              CMDADR(178)
#define cmd_VARADD              CMDADR(179)
#define cmd_VARSUB              CMDADR(180)
#define cmd_VARDIMQ             CMDADR(181)
#define cmd_VARSIZEQ    CMDADR(182)
#define cmd_VARINC              CMDADR(183)
#define cmd_VARDEC              CMDADR(184)
#define cmd_VARCAPQ             CMDADR(185)

        /*-- A-Stack Data Operations  --*/
#define cmd_VARDIMQs    CMDADR(186)
#define cmd_VARSIZEQs   CMDADR(187)

        /*-- Get Adr  Operations  --*/
#define cmd_GETADR      CMDADR(188)
#define cmd_GETADR_OWN  CMDADR(189)
#define cmd_GETADR_REL  CMDADR(190)


/*------- Input/Output  console operations ----*/
#define cmd_Tobs        CMDADR(200)
#define cmd_Trbs_1      CMDADR(201)
#define cmd_Trbss_1     CMDADR(202)
#define cmd_Trbs_0      CMDADR(203)
#define cmd_Trbss_0     CMDADR(204)

#define cmd_OS          CMDADR(205)
#define cmd_WUp         CMDADR(206)
#define cmd_WDown       CMDADR(207)
#define cmd_WAtr        CMDADR(208)
#define cmd_RAtr        CMDADR(209)
#define cmd_WAtrL       CMDADR(210)
#define cmd_RCP         CMDADR(211)
#define cmd_SCP         CMDADR(212)

/*------- Input/Output file operations ----*/
#define cmd_FSFUN_C     CMDADR(213)
#define cmd_CLOS_C      CMDADR(214)
#define cmd_OPEN_C      CMDADR(215)
#define cmd_LSEEK_C     CMDADR(216)
#define cmd_RDWR_C      CMDADR(217)
#define cmd_CRT0_C      CMDADR(218)
#define cmd_CRT_C       CMDADR(219)
#define cmd_CRT_TMP_C   CMDADR(220)
#define cmd_REN_C       CMDADR(221)
#define cmd_DEL_C       CMDADR(222)

/*------- Directory operations ----*/
#define cmd_GETDRV_C    CMDADR(223)
#define cmd_CHDIR_C     CMDADR(224)
#define cmd_SETDRV_C    CMDADR(225)
#define cmd_GETCHDIR_C  CMDADR(226)
#define cmd_FFF_C       CMDADR(227)
#define cmd_FNF_C       CMDADR(228)
#define cmd_CLOSEALL_C  CMDADR(229)
#define cmd_PortB       CMDADR(230)
#define cmd_PortW       CMDADR(231)
#define cmd_PortD       CMDADR(232)
#define cmd_sPortB      CMDADR(233)
#define cmd_sPortW      CMDADR(234)
#define cmd_sPortD      CMDADR(235)

/*------- Win32API operations ------------*/
#define cmd_Win32API    CMDADR(250)
#define cmd_WinFnExec   CMDADR(251)
#define cmd_FormWndProc CMDADR(252)
#define cmd_RealADR     CMDADR(253)
#define cmd_DsspADR     CMDADR(254)

#define cmd_TestExecWndProc     CMDADR(278)
#define cmd_TestWin32API        CMDADR(279)



#define cmd_aAB         CMDADR(236)
#define cmd_aAW         CMDADR(237)
#define cmd_aAL         CMDADR(238)
#define cmd_TAB         CMDADR(239)
#define cmd_TAW         CMDADR(240)
#define cmd_TAL         CMDADR(241)
#define cmd_mallocD     CMDADR(242)
#define cmd_reallocD    CMDADR(243)
#define cmd_freeD       CMDADR(244)
#define cmd_NULL        CMDADR(246)
#define cmd_FDADR       CMDADR(247)
#define cmd_DFADR       CMDADR(248)

#define cmd_interr      CMDADR(249)


/*--------    Debug  operations    ----*/
#define cmd_G11         CMDADR(280)
#define cmd_Restart     CMDADR(281)

/*--------   Coroutine (Context) operations    ----*/
#define cmd_Transfer     CMDADR(286)
#define cmd_NewContext   CMDADR(287)
#define cmd_MainContext  CMDADR(288)
#define cmd_ForceContext CMDADR(289)

/*--- Interrupt Mehanism operations ---*/
#define cmd_RaiseIntrpt    CMDADR(290)
#define cmd_GetIntVect     CMDADR(291)
#define cmd_SetIntVect     CMDADR(292)
#define cmd_RestoreIntVect CMDADR(293)
#define cmd_ION            CMDADR(294)
#define cmd_IOFF           CMDADR(295)
#define cmd_IRESET         CMDADR(296)
#define cmd_IntrON         CMDADR(297)
#define cmd_IntrOFF        CMDADR(298)

/*********************************************************/
/****    LIST of COMMANDs for AUXILIARY-OPERATIONs    ****/
/*********************************************************/

#define cmd_VarAdr      CMDADR(300)

        /*-- Long Var --*/
#define cmd_LVarDec     CMDADR(301)
#define cmd_LVarInc     CMDADR(302)
#define cmd_LVarSub     CMDADR(303)
#define cmd_LVarAdd     CMDADR(304)
#define cmd_LVarSt0     CMDADR(305)
#define cmd_LVarSt1     CMDADR(306)
#define cmd_LVarSt      CMDADR(307)
#define cmd_LVarVal     CMDADR(308)

        /*-- Word Var --*/
#define cmd_WVarDec     CMDADR(309)
#define cmd_WVarInc     CMDADR(310)
#define cmd_WVarSub     CMDADR(311)
#define cmd_WVarAdd     CMDADR(312)
#define cmd_WVarSt0     CMDADR(313)
#define cmd_WVarSt1     CMDADR(314)
#define cmd_WVarSt      CMDADR(315)
#define cmd_WVarVal     CMDADR(316)

        /*-- Byte Var --*/
#define cmd_BVarDec     CMDADR(317)
#define cmd_BVarInc     CMDADR(318)
#define cmd_BVarSub     CMDADR(319)
#define cmd_BVarAdd     CMDADR(320)
#define cmd_BVarSt0     CMDADR(321)
#define cmd_BVarSt1     CMDADR(322)
#define cmd_BVarSt      CMDADR(323)
#define cmd_BVarVal     CMDADR(324)

        /*-- Act Var --*/
#define cmd_AVarVal     CMDADR(325)

        /*-- Int Var --*/
#define cmd_IVarVal     CMDADR(326)
        /*-- SByte Var --*/
#define cmd_SBVarVal    CMDADR(327)

        /*-- Universal Vector --*/
#define cmd_VSizeQ      CMDADR(328)
#define cmd_VDimQ       CMDADR(329)
#define cmd_VCapQ       CMDADR(330)
#define cmd_VctrDec     CMDADR(331)
#define cmd_VctrInc     CMDADR(332)
#define cmd_VctrSub     CMDADR(333)
#define cmd_VctrAdd     CMDADR(334)
#define cmd_VctrSt0     CMDADR(335)
#define cmd_VctrSt1     CMDADR(336)
#define cmd_VctrAdr     CMDADR(337)
#define cmd_VctrAllSet  CMDADR(338)
#define cmd_VctrSt      CMDADR(339)
#define cmd_VctrVal     CMDADR(340)

        /*-- Universal Array --*/
#define cmd_ArrDec      CMDADR(372)
#define cmd_ArrInc      CMDADR(373)
#define cmd_ArrSub      CMDADR(374)
#define cmd_ArrAdd      CMDADR(375)
#define cmd_ArrSt0      CMDADR(376)
#define cmd_ArrSt1      CMDADR(377)
#define cmd_ArrAdr      CMDADR(378)
#define cmd_ArrSt       CMDADR(379)
#define cmd_ArrVal      CMDADR(380)

        /*-- Universal Structure (Record) --*/
#define cmd_StructSt    CMDADR(381)
#define cmd_StructSt0   CMDADR(382)
#define cmd_StructSize  CMDADR(383)

    /*-- string --*/
#define cmd_sSByte      CMDADR(390)
#define cmd_sSRpl       CMDADR(391)
#define cmd_sSCmp       CMDADR(392)
#define cmd_sSAddB      CMDADR(393)
#define cmd_sSLen       CMDADR(394)
#define cmd_sSSsbi      CMDADR(395)
#define cmd_sSSsb       CMDADR(396)
#define cmd_sSIsb       CMDADR(397)
#define cmd_sSCut       CMDADR(398)
#define cmd_sSCap       CMDADR(399)
#define cmd_sSDel       CMDADR(400)
#define cmd_sSIns       CMDADR(401)
#define cmd_sSDsb       CMDADR(402)
#define cmd_sSAdd       CMDADR(403)
#define cmd_sSClr       CMDADR(404)
#define cmd_sSOvr       CMDADR(405)
#define cmd_sStSets     CMDADR(406)
#define cmd_sStrAd      CMDADR(407)
#define cmd_sStSet      CMDADR(408)
#define cmd_sStVal      CMDADR(409)

    /*-- string prefixs--*/
#define cmd_SByte       CMDADR(410)
#define cmd_SRpl        CMDADR(411)
#define cmd_SCmp        CMDADR(412)
#define cmd_SAddB       CMDADR(413)
#define cmd_SLen        CMDADR(414)
#define cmd_SSsbi       CMDADR(415)
#define cmd_SSsb        CMDADR(416)
#define cmd_SIsb        CMDADR(417)
#define cmd_SCut        CMDADR(418)
#define cmd_SCap        CMDADR(419)
#define cmd_SDel        CMDADR(420)
#define cmd_SIns        CMDADR(421)
#define cmd_SDsb        CMDADR(422)
#define cmd_SAdd        CMDADR(423)
#define cmd_SClr        CMDADR(424)
#define cmd_SOvr        CMDADR(425)
#define cmd_arg2str     CMDADR(426)
#define cmd_dpfname     CMDADR(427)

#define cmd_ttin        CMDADR(428)


/*********************************************************/
/****  LIST of COMMAND ADDRESSes for SYSTEM BODIES    ****/
/*********************************************************/

        /****  Body for BR1  ***/
#define cmd_BR1  CMDADR(500)

        /****  Body for RTLT  ***/
#define cmd_RTLT        CMDADR(501)
        /****  Body for ANY situation  ***/
#define cmd_ANY         CMDADR(503)

        /****  Bodies for RP1 and DO1  ***/
#define cmd_RP1  CMDADR(507)
#define cmd_DO1  CMDADR(508)

        /****  Body for LVOST  ***/
#define cmd_LVOST       CMDADR(525)

        /**************/
#define cmd_NEXT        CMDADR(527)


        /**** Data structure Descriptors ***/
#define adr_LVARTYPE    CMDADR(600)     // + 14 !!!
#define cmd_LVARTYPE    CMDADR(600+1) // length + onname-op + 12 prefix ops

#define adr_WVARTYPE    CMDADR(614)
#define cmd_WVARTYPE    CMDADR(614+1)

#define adr_BVARTYPE    CMDADR(628)
#define cmd_BVARTYPE    CMDADR(628+1)

#define adr_AVARTYPE    CMDADR(642)
#define cmd_AVARTYPE    CMDADR(642+1)

#define adr_IVARTYPE    CMDADR(656)
#define cmd_IVARTYPE    CMDADR(656+1)

#define adr_SBVARTYPE   CMDADR(670)
#define cmd_SBVARTYPE   CMDADR(670+1)

#define adr_VCTRTYPE    CMDADR(684)
#define cmd_VCTRTYPE    CMDADR(684+1)

#define adr_ARRTYPE     CMDADR(698)
#define cmd_ARRTYPE     CMDADR(698+1)

#define adr_STRUCTTYPE  CMDADR(712)     // + 14 !!!
#define cmd_STRUCTTYPE  CMDADR(712+1)

#define adr_STRTYPE     CMDADR(726)
#define cmd_STRTYPE     CMDADR(726+1)

#define adr_NEWTYPE     CMDADR(748) // = 726+22 !!!

/*********************************************************/
/****  LIST of COMMAND ADDRESSes for TEST-OPERATIONs  ****/
/*********************************************************/

/*---------- TEST OPERATIONS  ---------------------------*/
#define cmd_TEST        CMDADR(900)

/*----------  DIALOG OPERATIONS  ------------------------*/
#define cmd_PrT         CMDADR(901)
#define cmd_PrTx        CMDADR(902)
#define cmd_PrS         CMDADR(903)
#define cmd_PrSx        CMDADR(904)
#define cmd_PrTOS       CMDADR(905)
#define cmd_TIS         CMDADR(906)

/*---------- SLOVAR OPERATIONS  -------------------------*/
#define cmd_FindWord    CMDADR(907)
#define cmd_GetWord     CMDADR(908)
#define cmd_PutWord     CMDADR(909)
#define cmd_UnWord      CMDADR(910)
#define cmd_AdrWord     CMDADR(911)


/*---------- Primitive Test OPERATIONS  ------------------*/

#define cmd_A0  CMDADR(921)
#define cmd_A1  CMDADR(922)
#define cmd_A2  CMDADR(923)
#define cmd_A3  CMDADR(924)
#define cmd_A4  CMDADR(925)
#define cmd_A5  CMDADR(926)
#define cmd_A6  CMDADR(927)
#define cmd_N   CMDADR(928)
#define cmd_E   CMDADR(929)

#define cmd_TestAdr     CMDADR(930)

#define cmd_ExecDSSP    CMDADR(999)

/*********************************************************/
/******     LIST of COMMAND cmds for TEST BODIES     *****/
/*********************************************************/

#define cmd_AA  CMDADR(1000)
#define cmd_A   CMDADR(1015)
#define cmd_Test  CMDADR(1023)
#define cmd_B   CMDADR(1025)

#define cmd_VARX        CMDADR(1043)
#define adr_X   CMDADR(1042)
#define cmd_SX  CMDADR(1046)
#define cmd_AX  CMDADR(1050)

#define cmd_VCTRV       CMDADR(1055)
#define adr_V   CMDADR(1054)
#define cmd_SV  CMDADR(1060)
#define cmd_AV  CMDADR(1064)

#define cmd_VCTRW       CMDADR(1069)
#define adr_W           CMDADR(1068)
#define cmd_SW          CMDADR(1074)
#define cmd_AW          CMDADR(1078)

#define cmd_VCTRBV      CMDADR(1083)
#define adr_BV          CMDADR(1082)
#define cmd_SBV         CMDADR(1088)
#define cmd_ABV         CMDADR(1092)

#define cmd_Sit         CMDADR(1096)
#define cmd_TEST_ON CMDADR(1100)
#define cmd_Reac        CMDADR(1112)

#define cmd_TEST_DO     CMDADR(1116)
#define cmd_TEST_RP     CMDADR(1126)
#define cmd_TEST_STEP   CMDADR(1136)
#define cmd_Sit1        CMDADR(1150)

        /****  MAIN LOOP bodies ***/
#define cmd_MAINLOOP  CMDADR(1151)
#define cmd_EXECWORD  CMDADR(1156)

#define cmd_CX  CMDADR(1164)

#define cmd_NEXTTEST   CMDADR(1166)
