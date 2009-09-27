#include <stdio.h>
#include <stdlib.h>
// 
#include <string.h> 
#include <ctype.h> 
// 

char word_t[30];
char str[300]; 
char findword_t[50]; 
long findlen,findlen8; 
int nch;   
#define VOC(a,x)    voc_n++; strcpy(voc[voc_n].name,a); voc[voc_n].entry=&x; 
#define PTM(a,n,ptr,t)    ptm_n++; strcpy(ptm[ptm_n].s,a); ptm[ptm_n].l=n;ptm[ptm_n].type=t;ptm[ptm_n].adr_use=(unsigned char*)ptr; 
 
struct vocabulary               /* vocabulary */ 
{ 
char name[30]; 
void (*entry)(); 
}; 
 
struct vocabulary voc[600]; 
 
struct ptm_word 
{ 
	char s[255]; 
	long l; 
	unsigned char *adr_use; 
	int type; // 0 - const; 1 - ."" 2 - "" 3 - undefword  
		  // 4 - undefword in voc 
}; 
struct ptm_word ptm[300]; 
 
unsigned char memory[99000]; 
unsigned char vs0[90000]; 
unsigned char sds0[4000]; 
unsigned char wds0[4000]; 
 
unsigned char *v1,*nb,*d1,*wd1,*yd1,*nv,*az; 
long htw,hnw,nopcode,noplen,ba; 
long azp; 
	 
long tdummyv,ldatav,dtypev,tabsolutev,fix_v,own_v; 
 
unsigned char header[4000]; 
 
#define start_v1 2000 
#define sumheader 96 
#define sysarea_len 1024


FILE *in;
struct file_info
{
FILE *inf_t;
int pos;
};
struct file_info inf_t[10];

long delhd=1; 
long opsp_t=0;
long  st_op[200];

long csp_t=0;
long st_c[200];
int num;
int info_n=0;
int isnum=1;
int findok=0;
int z=0;
int voc_n=0;                    /* number of voc entries */
int ptm_n=0; 
int base=10;
int fi=0; 

// voc
void load();
void b16();
void b10();
void b8();
void b2();
void sets();
void cr(); 
void add(); 
void sub(); 
void mul(); 
void div_(); 
void pIF(); 
void pFI(); 
void tt(); 
void InitDSSP(); 
void dtdt(); 
void pbit(); 
void kk(); 
void cmpltr(); 
void word_(); 
void byte_(); 
void act_(); 
void long_(); 
void var(); 
void fix_(); 
void own_(); 
void vctr(); 
void value(); 
void absolute(); 
void setv(); 
void gadrv(); 
void trap(); 
void wimlen(); 
void channel(); 
void sub1(); 
void stl(); 
void vafa(); 
void copyw(); 
void codew(); 
void int_(); 
void copy(); 
void drop(); 
void busew(); 
void musew(); 
void nopcodew(); 
void noplenw(); 
void typeptr(); 
void fqnd(); 
void neopr(); 
void lnk(); 
void svs(); 
 
//var 
long nomredv,nomverv,beg_yd1v,end_yd1v,beg_wd1v,end_wd1v,csbv,opsbv; 
long beg_ptmv,end_ptmv,end_memv,beg_d1v; 
void end_mem(); 
void end_memset(); 
void beg_d1(); 
void beg_d1set(); 
void beg_ptm(); 
void beg_ptmset(); 
void end_ptm(); 
void end_ptmset(); 
void csb_t(); 
void csbset(); 
void opsb_t(); 
void opsbset(); 
void end_wd1(); 
void end_wd1set(); 
void beg_wd1(); 
void beg_wd1set(); 
void end_yd1(); 
void end_yd1set(); 
void beg_yd1(); 
void beg_yd1set(); 
void nomver(); 
void nomverset(); 
void nomred(); 
void nomredset(); 
void __pusk(); 
void __puskset(); 
void main_own();
void main_ownset();

 
long __tosoutv,prtlenv,__intrprocv,__tzv,__raisev,undefcodev; 
long intcodev,varcodev,cnstcodev,txtcodev,ptxtcodev,intlenv; 
long varlenv,cnstlenv,__lvarpv,__wvarpv,__bvarpv,__avarpv; 
long __ivarpv,__svarpv,__lvctpv,__wvctpv,__bvctpv,__avctpv; 
long __getadrv,__getadr_ownv,__basepv,__nbv,__d1v,__yd1v,__wd1v; 
long __v1v,__nv1v,__puskv; 
long __vctrpv,main_ownv;
 
void __tosout(); 
void prtlen(); 
void __intrproc();  
void __tz();  
void __raise();  
void undefcode();  
void intcode();  
void varcode();  
void cnstcode();  
void txtcode();  
void ptxtcode();  
void intlen(); 
void varlen(); 
void cnstlen();  
void __lvarp();  
void __wvarp();  
void __bvarp();  
void __avarp(); 
void __ivarp();  
void __svarp();  
void __lvctp();  
void __wvctp();  
void __bvctp();  
void __avctp(); 
void __vctrp(); 
void __getadr();  
void __getadr_own();  
void __basep();  
void __nb();  
void __d1();  
void __yd1();  
void __wd1(); 
void __v1();  
void __nv1(); 
 
void __tosoutset();  
void prtlenset();  
void __intrprocset();   
void __tzset_t();   
void __raiseset();   
void undefcodeset();   
void intcodeset();   
void varcodeset();   
void cnstcodeset();   
void txtcodeset();   
void ptxtcodeset();   
void intlenset();  
void varlenset();  
void cnstlenset();   
void __lvarpset();   
void __wvarpset();   
void __bvarpset();   
void __avarpset();  
void __ivarpset();   
void __svarpset();   
void __lvctpset();   
void __wvctpset();   
void __bvctpset();   
void __avctpset();  
void __vctrpset(); 
void __getadrset();   
void __getadr_ownset();   
void __basepset();   
void __nbset();   
void __d1set();   
void __yd1set();   
void __wd1set();  
void __v1set();   
void __nv1set();  
void _notst();
void define_(); 
 
 
// work procedures
void init_voc();
void next_word();
void not_found();
int next_voc();
void exec();
int atob(char str[]);
int isDIGIT(char ch);
 
void BuildDPFFile(char *arg)
{  
// int argc;
// char * argv[];
  in=fopen(arg,"r");
  if (in!=NULL)
    {
      info_n++; inf_t[info_n].inf_t=in; inf_t[info_n].pos=0;
    }
   else
    {
      printf ("Input Error in %s\n",arg); return;
    } 
  init_voc();
  do 
   {
     do
       { 
		 word_t[0]=0;
	 next_word();
	 if (word_t[0]) exec();
       }
     while (!feof(in)); 
     fclose(in);
     info_n--;in=inf_t[info_n].inf_t;
   }
  while (info_n>0);
}

void init_voc()
{ 
  VOC("LOAD",load); 
  VOC("B2",b2); 
  VOC("B8",b8); 
  VOC("B10",b10); 
  VOC("B16",b16); 
  VOC("__",sets); 
  VOC("CR",cr); 
  VOC("NOMVERSET",nomverset); 
  VOC("NOMVER",nomver); 
  nomverv=6; 
  VOC("NOMREDSET",nomredset); 
  VOC("NOMRED",nomred); 
  nomredv=0; 
  VOC("BEG_YD1SET",beg_yd1set); 
  VOC("BEG_YD1",beg_yd1); 
  beg_yd1v=300000; 
  VOC("END_YD1SET",end_yd1set); 
  VOC("END_YD1",end_yd1); 
  end_yd1v=369996; 
  VOC("BEG_WD1SET",beg_wd1set); 
  VOC("BEG_WD1",beg_wd1); 
  main_ownv=370000;
  VOC("MAIN_OWNSET",main_ownset); 
  VOC("MAIN_OWN",main_own); 
  //beg_wd1v=370000; 
  beg_wd1v=0; 
  VOC("END_WD1SET",end_wd1set); 
  VOC("END_WD1",end_wd1); 
  //end_wd1v=439996; 
  end_wd1v=439996-370000; 
  VOC("OPSBSET",opsbset); 
  VOC("OPSB",opsb_t); 
  opsbv=440000; 
  VOC("CSBSET",csbset); 
  VOC("CSB",csb_t); 
  csbv=459980; 
  VOC("BEG_PTMSET",beg_ptmset); 
  VOC("BEG_PTM",beg_ptm); 
  beg_ptmv=460000; 
  VOC("END_PTMSET",end_ptmset); 
  VOC("END_PTM",end_ptm); 
  end_ptmv=469990; 
  VOC("BEG_D1SET",beg_d1set); 
  VOC("BEG_D1",beg_d1); 
  beg_d1v=470000; 
  VOC("END_MEMSET",end_memset); 
  VOC("END_MEM",end_mem); 
  end_memv=900000; 
  VOC("''TOSOUT",__tosout); 
  VOC("PRTLEN",prtlen); 
  VOC("''INTRPROC",__intrproc); 
  VOC("'';",__tz); 
  VOC("''RAISE",__raise); 
  VOC("UNDEFCODE",undefcode); 
  VOC("INTCODE",intcode); 
  VOC("VARCODE",varcode); 
  VOC("CNSTCODE",cnstcode); 
  VOC("TXTCODE",txtcode); 
  VOC("PTXTCODE",ptxtcode); 
  VOC("INTLEN",intlen); 
  VOC("VARLEN",varlen); 
  VOC("CNSTLEN",cnstlen); 
  VOC("''LVAR@",__lvarp); 
  VOC("''WVAR@",__wvarp); 
  VOC("''BVAR@",__bvarp); 
  VOC("''AVAR@",__avarp); 
  VOC("''IVAR@",__ivarp); 
  VOC("''SVAR@",__svarp); 
  VOC("''LVCT@",__lvctp); 
  VOC("''WVCT@",__wvctp); 
  VOC("''BVCT@",__bvctp); 
  VOC("''AVCT@",__avctp); 
  VOC("''VCTR@",__vctrp); 
  VOC("''GETADR",__getadr); 
  VOC("''GETADR_OWN",__getadr_own); 
  VOC("''BASE@",__basep); 
  VOC("''NB",__nb); 
  VOC("''D1",__d1); 
  VOC("''YD1",__yd1); 
  VOC("''WD1",__wd1); 
  VOC("''V1",__v1); 
  VOC("''NV",__nv1); 
  VOC("''PUSK",__pusk); 
  VOC("''PUSKSET",__puskset); 
 
  VOC("''TOSOUTSET",__tosoutset); 
  VOC("PRTLENSET",prtlenset); 
  VOC("''INTRPROCSET",__intrprocset); 
  VOC("'';SET",__tzset_t); 
  VOC("''RAISESET",__raiseset); 
  VOC("UNDEFCODESET",undefcodeset); 
  VOC("INTCODESET",intcodeset); 
  VOC("VARCODESET",varcodeset); 
  VOC("CNSTCODESET",cnstcodeset); 
  VOC("TXTCODESET",txtcodeset); 
  VOC("PTXTCODESET",ptxtcodeset); 
  VOC("INTLENSET",intlenset); 
  VOC("VARLENSET",varlenset); 
  VOC("CNSTLENSET",cnstlenset); 
  VOC("''LVAR@SET",__lvarpset); 
  VOC("''WVAR@SET",__wvarpset); 
  VOC("''BVAR@SET",__bvarpset); 
  VOC("''AVAR@SET",__avarpset); 
  VOC("''IVAR@SET",__ivarpset); 
  VOC("''SVAR@SET",__svarpset); 
  VOC("''LVCT@SET",__lvctpset); 
  VOC("''WVCT@SET",__wvctpset); 
  VOC("''BVCT@SET",__bvctpset); 
  VOC("''AVCT@SET",__avctpset); 
  VOC("''VCTR@SET",__vctrpset); 
  VOC("''GETADRSET",__getadrset); 
  VOC("''GETADR_OWNSET",__getadr_ownset); 
  VOC("''BASE@SET",__basepset); 
  VOC("''NBSET",__nbset); 
  VOC("''D1SET",__d1set); 
  VOC("''YD1SET",__yd1set); 
  VOC("''WD1SET",__wd1set); 
  VOC("''V1SET",__v1set); 
  VOC("''NVSET",__nv1set); 
  
  VOC("+",add); 
  VOC("-",sub); 
  VOC("*",mul); 
  VOC("/",div_); 
  VOC("%IF",pIF); 
  VOC("%FI",pFI); 
  VOC("..",tt); 
  VOC("InitDSSP",InitDSSP); 
  VOC("::",dtdt); 
  VOC("PBIT",pbit); 
  VOC("''",kk); 
  VOC(":",cmpltr); 
  VOC("WORD",word_); 
  VOC("BYTE",byte_); 
  VOC("ACT",act_); 
  VOC("LONG",long_); 
  VOC("VAR",var); 
  VOC("FIX",fix_); 
  fix_v=0; 
  VOC("OWN",own_); 
  own_v=0; 
  VOC("VCTR",vctr); 
  VOC("VALUE",value); 
  VOC("ABSOLUT",absolute); 
  tabsolutev=0; 
  VOC("!",setv); 
  VOC("'",gadrv); 
  VOC("TRAP",trap); 
  VOC("WIMLEN",wimlen); 
  VOC("CHANNEL",channel); 
  VOC("1-",sub1); 
  VOC("!TL",stl); 
  VOC("VA-FA",vafa); 
  VOC("COPYW",copyw); 
  VOC("CODE",codew); 
  VOC(",INT",int_); 
  VOC("C",copy); 
  VOC("D",drop); 
  VOC("BUSE",busew); 
  VOC("MUSE",musew); 
  VOC("NOPCODE",nopcodew); 
  VOC("NOPLEN",noplenw); 
  VOC("TYPEPTR",typeptr); 
  VOC("FQND",fqnd); 
  VOC("NEOPR",neopr); 
  VOC("LNK",lnk); 
  VOC("SVS",svs); 
  VOC("NOT",_notst); 
  VOC("DEFINE?",define_); 
}
void wimlen() 
{ 
	st_op[++opsp_t]=0x20; 
} 
 
void stl() 
{ 
	long *adr; 
	adr=(long*)st_op[opsp_t--]; 
	*adr=(long)st_op[opsp_t--]; 
} 
void vafa() 
{ 
	st_op[opsp_t]+=((long)memory); 
} 

void out_str()
 {
   char ch; 
   for (;;)
    {
     ch=fgetc(in);
     if (ch=='"'||feof(in)) {fgetc(in);return;}
     if (!fi) putchar(ch);
   }
 }

void next_word()
{
  char ch; 
  isnum=1;
  nch=0;
  do
   {
    ch=fgetc(in);
    if (ch=='[')
      { do ch=fgetc(in); while (ch!=']'); ch=fgetc(in);}
   }
  while (isspace(ch));
  if ((!isdigit(ch))&&(ch!='-'))  
	  isnum=0; 
   

  if (feof(in)) return;
  do
    {
      if ((ch=='"') && (word_t[0]=='.') && (nch==1))
	 {word_t[1]=ch; out_str(); return; }
      if ((!isDIGIT(ch))&&(nch>0)) isnum=0; 
      word_t[nch++]=ch;
      ch=fgetc(in);
    }
  while ((!isspace(ch))&&(!feof(in)));  
  if ((nch==1)&&((word_t[0]=='-')||(word_t[0]=='+')||(word_t[0]=='*')||(word_t[0]=='/')))
	  isnum=0;
  word_t[nch]=0;
// printf(" w= %s\n",word);
}

void not_found()
 {
   printf("%s not found in comp. voc\n",word_t);
 }

int next_voc()
 {
  z++; findok=0;
  if (z>voc_n) return 0;
  if (strcmp(voc[z].name,word_t)==0)
    { findok=1;return 0;}
   else
     {return 1;}
 }
 
void symb_t() 
 { 
	if (word_t[1]==0)  
		st_op[++opsp_t]=(int)32;        
	else 
		st_op[++opsp_t]=(int)word_t[1];       //printf("Num=%d\n",(int)word_t[1]); 
 }

void exec()
{
   if (isnum)
     {
       num=atob(word_t); st_op[++opsp_t]=num;
//       printf("Num=%d\n",num); 
       return;
     }
   if ((word_t[1]=='"') && (word_t[0]=='.') && (nch==1)) {return; }
   if (word_t[0]=='#') {symb_t();return;}
   z=0;
   for(;;)
      if (next_voc()==0) break;   
//   printf(" %s %lx %lx\n", voc[z].name,(long)*(voc[z].entry),z);
   if (findok)
     (*(voc[z].entry))();
    else
      not_found();

}

int dig(char ch)
 {
   if (isdigit(ch)) {return (ch-'0');} else {return (toupper(ch)-'A'+10);}
 }
    
int atob (char *str)
 {
   int i,fl;
   int bs=1;
   int res=0; 
   if (str[0]=='-')  
   { 
	   str[0]='0'; 
	   fl=-1;  
   } 
   else  
	   fl=1; 

   for (i=strlen(str)-1;i>-1;i--)
     {res+=dig(str[i])*bs;bs=bs*base;}
   return res*fl;
 }
     
int isDIGIT(char ch)
 {
  switch (base) 
   {
     case 16: return (isxdigit(ch));
     case 10: return (isdigit(ch));
     case 8: return (isdigit(ch) && ch!='8' && ch!='9');
     case 2: return (ch=='0' || ch=='1');
  }
return 0;
 }
     

void load()
{
  next_word();
//  printf("\nLoading %s\n",word_t);
  inf_t[info_n].pos=ftell(in);
  in=fopen(word_t,"r");
  if (in!=NULL)
    {
      info_n++; inf_t[info_n].inf_t=in; inf_t[info_n].pos=0;
    }
   else
    {
      printf ("Input Error in %s\n",word_t);in=inf_t[info_n].inf_t;
    }    
}

void b16()
 { base=16;}
void b10()
 { base=10;}
void b8()
 { base=8;}
void b2()
 { base=2;}
void sets() 
{ 
	int lens; 
	next_word();lens=strlen(word_t);word_t[lens]='S'; 
	word_t[lens+1]='E';       word_t[lens+2]='T';       word_t[lens+3]=0; 
	exec(); 
} 
void cr() 
{ 
	printf("\n"); 
} 
 
long a,b; 
void add() 
{ 
	a=st_op[opsp_t--]; 
	b=st_op[opsp_t]; 
	st_op[opsp_t]=a+b; 
} 
void sub() 
{ 
	a=st_op[opsp_t--]; 
	b=st_op[opsp_t]; 
	st_op[opsp_t]=b-a; 
} 
void mul() 
{ 
	a=st_op[opsp_t--]; 
	b=st_op[opsp_t]; 
	st_op[opsp_t]=a*b; 
} 
void div_() 
{ 
	a=st_op[opsp_t--]; 
	b=st_op[opsp_t]; 
	st_op[opsp_t]=(int)b/a; 
	st_op[++opsp_t]=b-((int)b/a)*a; 
} 
void pIF() 
{ 
	if (st_op[opsp_t--]!=0) return; 
	for (;;) 
	{ 
		fi=1; 
		next_word(); 
		if (strcmp(word_t,"%FI")==0) {fi=0;return;} 
	} 
} 
void pFI() {fi=0;} 
 
void tt() 
{ 
	int i; 
	printf("%d pos\n[",opsp_t); 
	for (i=1;i<=opsp_t;i++) 
		printf(" %ld",st_op[i]); 
	printf(" ]\n"); 
} 
 
void dsreset() 
{ 
	tdummyv=0; 
	ldatav=2; 
	dtypev=1; 
	tabsolutev=0; 
	fix_v=0; 
	own_v=0; 
} 
 
void movevoc() 
{ 
	long *i; 
	for (i=(long*)nv;i>=(long*)v1;i++) 
		*(i+80/4)=*i; 
	nv+=80; 
	v1+=80; 
} 
void *buse(long q) 
{ 
	long *nbo; 
	if (v1-vs0<30) movevoc(); 
//      if (((int)(q/4))*4==q) 
//              nb=(unsigned char*)(((long)nb+3)&(~3)); 
//      else 
//              if (((int)(q/2))*2==q) 
//                      nb=(unsigned char*)(((long)nb+1)&(~1)); 
	nbo=(long*)nb; 
	nb+=q; 
	return (void*)nbo; 
} 
void nv_s(long a) 
{ 
	long *ptr=(long*)nv; 
	*ptr=a; nv+=4; 
} 
void l_s(long d) 
{ 
	long *ptr; 
	ptr=(long*)buse(4); 
	*ptr=d; 
} 
 
void InitDSSP() 
{ 
	char newword_t[50]; 
	long sizef=0,posf=0; 
	int i,j; 
	int nlen,nlen8; 
	unsigned char *nb1; 
	FILE *voc1; 
	voc1=fopen("dssp.voc","rb"); 
    if (voc1==NULL) 
    { 
      printf ("Input Error in DSSP.VOC\n");return; 
    }     
	fread(header,sizeof(char),sumheader,voc1); 
	posf=ftell(voc1); 
    fseek(voc1,0,SEEK_END); 
	sizef=ftell(voc1); 
	fseek(voc1,posf,SEEK_SET); 
	fread(memory,sizeof(char),sizef-sumheader,voc1); 
	htw=*((long*)(header+4)); 
	hnw=*((long*)(header+8)); 
	nopcode=*((long*)(header+12)); 
	noplen=4; 
	nb=memory+htw; 
	yd1=sds0; 
	wd1=wds0; 
	d1=(unsigned char*)beg_d1v; 
	dsreset(); 
	v1=vs0+start_v1; 
	nv=v1; 
    nv_s(0); nv_s(0); nv_s(0); nv_s(0); nv_s(0); nv_s(0); 
	nb1=nb; 
	for (i=1;i<=hnw;i++) 
	  {  
		nv_s(*(long*)nb1);nb1+=4; 
		nlen=strlen((char*)nb1); 
		nlen8=((nlen+2)+3)&(~3); 
		strcpy((char*)(newword_t+nlen8-nlen-2),(char*)nb1);nb1+=32; 
		newword_t[nlen8]=0; 
		newword_t[nlen8-1]=0x40;//Dbit 
		newword_t[nlen8-2]=(int)nlen8/4-1; 
		for (j=0;j<nlen8-nlen-2;j++) 
			newword_t[j]=32; 
		for (j=0;j<=newword_t[nlen8-2];j++) 
			nv_s(*(long*)((newword_t)+j*4)); 
		/*write voc*/ 
	  } 
	nv_s(0); nv_s(nb-memory);  
	l_s(nopcode); //,NOP 
	l_s(yd1-sds0+beg_yd1v); 
	*(long*)yd1=-1;yd1+=4; 
	strcpy((char*)nv,"SYSTEM");nv+=6; 
	*(nv++)=01;*(nv++)=0; 
	nv_s(1); 
} 
 
void dtdt() //:: 
{ 
	delhd=0; 
} 
 
 
 
void is0_4() 
{ 
	az-=4; 
	for (;;) 
	{ 
		if (*(long*)az==0) break; 
		az-=4; 
	} 
} 
 
void isk_t() 
{ 
	int i,notf; 
	char *imch; 
	long im1,im2,count; 
	for(;;) 
	{ 
		if (*(long*)az==0) break; 
		if (*(char*)(az+3)<0) 
			{az=az+(*(long*)az); }
		else 
		{ 
			im1=*(long*)az; 
			imch=(char*)&im1; 
			*(imch+3)&=0x04; 
			*(imch+2)&=0x07; 
			im2=*(long*)(findword_t+findlen8-4); 
			if (im1==im2) 
			{ 
				notf=0; 
				count=((long)*(az+2))&0x07; 
				for(i=1;i<=(count);i++) 
					if (*(long*)(az-i*4)!=*(long*)(findword_t+findlen8-(i+1)*4)) {notf=1;break;} 
				if (!notf) {break;} 
				az=az-((count)*4+4); 
				az-=4; 
			} 
			else 
			{ 
				count=((long)*(az+2))&0x07; 
				az=az-((count)*4+4); 
				az-=4; 
			} 
		}        
	} 
	if (*(long*)az==0)  
		ba=0; 
	else 
		ba=*(long*)(az-((count)*4+4)); 
} 
 
void find() 
{ 
	//unsigned char *a; 
	long count; 
	long tmp; 
	az=nv; 
	for (;;) 
	{ 
		az-=8; 
		if (*(long*)az==0) break; 
		isk_t(); 
		if (ba!=0) {break;} 
		tmp=((*(long*)(az+4))+noplen); 
		tmp=(*(long*)(memory+tmp))-beg_yd1v; 
		if (*(long*)(sds0+tmp)<0)  
		{ 
			az-=4; 
			isk_t(); 
			//if (ba!=0)  
				break; 
		} 
		else 
			is0_4(); 
	} 
// cba 
	if (*(long*)az==0)  
		ba=0; 
	else 
	{ 
		count=((long)*(az+2))&0x07; 
		ba=*(long*)(az-((count)*4+4)); 
	} 
} 
 
 
void pbit() 
{ 
	next_word();  
	findlen=strlen(word_t);findlen8=(findlen+2+3)&(~3); 
	strcpy(findword_t,"     "); 
	strcpy(findword_t+(findlen8-findlen-2),word_t); 
	findword_t[findlen8-2]=findlen8/4-1; 
	findword_t[findlen8-1]=0; 
	find(); 
	if (ba==0) printf("Unknow %s\n",word_t); else *(char*)(az+3)|=0x20; 
} 
void kk() 
{ 
	next_word();  
	findlen=strlen(word_t);findlen8=(findlen+2+3)&(~3); 
	strcpy(findword_t,"     "); 
	strcpy(findword_t+(findlen8-findlen-2),word_t); 
	findword_t[findlen8-2]=findlen8/4-1; 
	findword_t[findlen8-1]=0; 
	find(); 
	if (ba==0) printf("Unknow K'' %s\n",word_t); else st_op[++opsp_t]=ba; 
} 
 
 
//var 
void end_mem()    {     st_op[++opsp_t]=end_memv; } 
void end_memset() {     end_memv=st_op[opsp_t--]; } 
void beg_d1()     {     st_op[++opsp_t]=beg_d1v;  } 
void beg_d1set()  {     beg_d1v=st_op[opsp_t--];  } 
void beg_ptm()    {     st_op[++opsp_t]=beg_ptmv; } 
void beg_ptmset() {     beg_ptmv=st_op[opsp_t--]; } 
void end_ptm()    {     st_op[++opsp_t]=end_ptmv; } 
void end_ptmset() {     end_ptmv=st_op[opsp_t--]; } 
void csb_t()                {     st_op[++opsp_t]=csbv;             } 
void csbset()     {     csbv=st_op[opsp_t--];             } 
void opsb_t()               {     st_op[++opsp_t]=opsbv;    } 
void opsbset()    {     opsbv=st_op[opsp_t--];    } 
void end_wd1()    {     st_op[++opsp_t]=end_wd1v; } 
void end_wd1set() {     end_wd1v=st_op[opsp_t--]; } 
void beg_wd1()    {     st_op[++opsp_t]=beg_wd1v; } 
void beg_wd1set() {     beg_wd1v=st_op[opsp_t--]; } 
void end_yd1()    {     st_op[++opsp_t]=end_yd1v; } 
void end_yd1set() {     end_yd1v=st_op[opsp_t--]; } 
void beg_yd1()    {     st_op[++opsp_t]=beg_yd1v; } 
void beg_yd1set() {     beg_yd1v=st_op[opsp_t--]; } 
void nomver()    {      st_op[++opsp_t]=nomverv;  } 
void nomverset() {      nomverv=st_op[opsp_t--];  } 
void nomred()    {      st_op[++opsp_t]=nomredv;  } 
void nomredset() {      nomredv=st_op[opsp_t--];  } 
 
void __tosout()  {      st_op[++opsp_t]=__tosoutv;} 
void prtlen()    {      st_op[++opsp_t]=prtlenv;  } 
void __intrproc() {     st_op[++opsp_t]=__intrprocv; } 
void __tz()              {      st_op[++opsp_t]=__tzv;    } 
void __raise()   {      st_op[++opsp_t]=__raisev; } 
void undefcode() {      st_op[++opsp_t]=undefcodev;  } 
void intcode()   {      st_op[++opsp_t]=intcodev; } 
void varcode()   {      st_op[++opsp_t]=varcodev; } 
void cnstcode()  {      st_op[++opsp_t]=cnstcodev;} 
void txtcode()   {      st_op[++opsp_t]=txtcodev; } 
void ptxtcode()  {      st_op[++opsp_t]=ptxtcodev;} 
void intlen()    {      st_op[++opsp_t]=intlenv;  } 
void varlen()    {      st_op[++opsp_t]=varlenv;  } 
void cnstlen()   {      st_op[++opsp_t]=cnstlenv; } 
void __lvarp()   {      st_op[++opsp_t]=__lvarpv; } 
void __wvarp()   {      st_op[++opsp_t]=__wvarpv; } 
void __bvarp()   {      st_op[++opsp_t]=__bvarpv; } 
void __avarp()   {      st_op[++opsp_t]=__avarpv; } 
void __ivarp()   {      st_op[++opsp_t]=__ivarpv; } 
void __svarp()   {      st_op[++opsp_t]=__svarpv; } 
void __lvctp()   {      st_op[++opsp_t]=__lvctpv; } 
void __wvctp()   {      st_op[++opsp_t]=__wvctpv; } 
void __bvctp()   {      st_op[++opsp_t]=__bvctpv; } 
void __avctp()   {      st_op[++opsp_t]=__avctpv; } 
void __vctrp()   {      st_op[++opsp_t]=__vctrpv; } 
void __getadr()  {      st_op[++opsp_t]=__getadrv;} 
void __getadr_own() {   st_op[++opsp_t]=__getadr_ownv; } 
void __basep()   {      st_op[++opsp_t]=__basepv; } 
void __nb()              {      st_op[++opsp_t]=__nbv;    } 
void __d1()              {      st_op[++opsp_t]=__d1v;    } 
void __yd1()     {      st_op[++opsp_t]=__yd1v;   } 
void __wd1()     {      st_op[++opsp_t]=__wd1v;   } 
void __v1()              {      st_op[++opsp_t]=__v1v;    } 
void __nv1()     {      st_op[++opsp_t]=__nv1v;   } 
 
void __tosoutset()  {   __tosoutv=st_op[opsp_t--];        } 
void prtlenset()    {   prtlenv=st_op[opsp_t--];          } 
void __intrprocset() {  __intrprocv=st_op[opsp_t--];      } 
void __tzset_t()      {   __tzv=st_op[opsp_t--];            } 
void __raiseset()   {   __raisev=st_op[opsp_t--];         } 
void undefcodeset() {   undefcodev=st_op[opsp_t--];       } 
void intcodeset()   {   intcodev=st_op[opsp_t--];         } 
void varcodeset()   {   varcodev=st_op[opsp_t--];         } 
void cnstcodeset()  {   cnstcodev=st_op[opsp_t--];        } 
void txtcodeset()   {   txtcodev=st_op[opsp_t--];         } 
void ptxtcodeset()  {   ptxtcodev=st_op[opsp_t--];        } 
void intlenset()    {   intlenv=st_op[opsp_t--];          } 
void varlenset()    {   varlenv=st_op[opsp_t--];          } 
void cnstlenset()   {   cnstlenv=st_op[opsp_t--];         } 
void __lvarpset()   {   __lvarpv=st_op[opsp_t--];         } 
void __wvarpset()   {   __wvarpv=st_op[opsp_t--];         } 
void __bvarpset()   {   __bvarpv=st_op[opsp_t--];         } 
void __avarpset()   {   __avarpv=st_op[opsp_t--];         } 
void __ivarpset()   {   __ivarpv=st_op[opsp_t--];         } 
void __svarpset()   {   __svarpv=st_op[opsp_t--];         } 
void __lvctpset()   {   __lvctpv=st_op[opsp_t--];         } 
void __wvctpset()   {   __wvctpv=st_op[opsp_t--];         } 
void __bvctpset()   {   __bvctpv=st_op[opsp_t--];         } 
void __avctpset()   {   __avctpv=st_op[opsp_t--];         } 
void __vctrpset()   {   __vctrpv=st_op[opsp_t--];         } 
void __getadrset()  {   __getadrv=st_op[opsp_t--];        } 
void __getadr_ownset() { __getadr_ownv=st_op[opsp_t--];} 
void __basepset()   {   __basepv=st_op[opsp_t--];         } 
void __nbset()          {       __nbv=st_op[opsp_t--];            } 
void __d1set()          {       __d1v=st_op[opsp_t--];            } 
void __yd1set()         {       __yd1v=st_op[opsp_t--];           } 
void __wd1set()         {       __wd1v=st_op[opsp_t--];           } 
void __v1set()          {       __v1v=st_op[opsp_t--];            } 
void __nv1set()         {       __nv1v=st_op[opsp_t--];           } 
void __pusk()           {       st_op[++opsp_t]=__puskv;          } 
void __puskset()        {       __puskv=st_op[opsp_t--];          } 
void main_own()			{       st_op[++opsp_t]=main_ownv;        } 
void main_ownset()		{       main_ownv=st_op[opsp_t--];          } 
 
 
void cfind(long c) 
{ 
	long *v1c,lw; 
 
	v1c=(long*)v1; 
	for (;;) 
	{ 
		lw=*v1c; 
		if (!lw) {ba=0;az=(unsigned char*)v1c;break;} 
		lw=*(long*)(memory+lw+cnstlenv); 
		if (c==lw) {ba=*v1c;az=(unsigned char*)v1c;break;} 
		v1c++; 
	} 
} 
 
void out_str_cmp() 
{ 
   char ch;  
   int nst=0; 
   for (;;) 
    { 
     ch=fgetc(in); 
     if (ch=='"'||feof(in)) {fgetc(in);break;} 
	 str[nst++]=ch; 
   } 
   str[nst]=0; 
} 
 
 
 // word = name proc 
void code1_(long bb) 
{ 
	int nlen,nlen8; 
	char newword_t[50],*azpc; 
	unsigned char *az_; 
	long *i,*ptr,lent,lw; 
	int j; 
	azp=*(long*)az; 
	az_=az; 
	az=nv; 
	is0_4(); 
	nlen=strlen(word_t); 
	nlen8=(nlen+2+3)&(~3); 
	strcpy(newword_t,"     "); 
	strcpy(newword_t+(nlen8-nlen-2),word_t); 
	newword_t[nlen8-2]=nlen8/4-1; 
	newword_t[nlen8-1]=(0x40*(char)delhd); 
	azpc=(char*)&azp; 
	if ((*(azpc+3))&0x10) 
	{ 
		printf(""); 
		lw=*(char*)(az_+2); 
		lw=(lw & 7)*4; 
		lent=*(long*)(az_-lw-8); 
		//reform table 
		for (j=0;j<(-lent/4)-1;j++) 
		{ 
			*(long*)(memory+*(long*)(az_-lw+lent+j*4-4))=(long)(bb); 
		} 
		*(long*)(az_-lw-4)=(long)(bb); 
//              *(long*)az_=(*(long*)az_)&((~0x50000000)); 
//              *(long*)az_=(*(long*)az_)|(0x40000000*(char)delhd); 
		*(char*)(az_+3)&=(~0x50); 
		*(char*)(az_+3)|=(0x40*(char)delhd); 
		//remove table 
		for (ptr=(long*)(az_-lw-4);ptr<=(long*)nv;ptr++) 
			*(ptr+lent/4)=*ptr; 
		nv+=lent; 
	} 
	else 
	{ 
		for (i=(long*)nv;i>=(long*)(az);i--) 
			*(i+nlen8/4+1)=*i; 
		*(long*)az=(long)(bb); 
		for (j=0;j<nlen8/4;j++) 
			*(long*)(az+j*4+4)=*(long*)(newword_t+j*4); 
		az=az_; 
		nv+=nlen8+4; 
	} 
	delhd=1; 
} 
 
 
 // word = name proc 
void code_() 
{ 
	int nlen,nlen8; 
	char newword_t[50],*azpc; 
	unsigned char *az_; 
	long *i,*ptr,lent,lw; 
	int j; 
	azp=*(long*)az; 
	az_=az; 
	az=nv; 
	is0_4(); 
	nlen=strlen(word_t); 
	nlen8=(nlen+2+3)&(~3); 
	strcpy(newword_t,"     "); 
	strcpy(newword_t+(nlen8-nlen-2),word_t); 
	newword_t[nlen8-2]=nlen8/4-1; 
	newword_t[nlen8-1]=(0x40*(char)delhd); 
	azpc=(char*)&azp; 
	if ((*(azpc+3))&0x10) 
	{ 
		printf(""); 
		lw=*(char*)(az_+2); 
		lw=(lw & 7)*4; 
		lent=*(long*)(az_-lw-8); 
		//reform table 
		for (j=0;j<(-lent/4)-1;j++) 
		{ 
			*(long*)(memory+*(long*)(az_-lw+lent+j*4-4))=(long)(nb-memory); 
		} 
		*(long*)(az_-lw-4)=(long)(nb-memory); 
		*(char*)(az_+3)&=(~0x50); 
		*(char*)(az_+3)|=(0x40*(char)delhd); 
//              *(long*)az_=(*(long*)az_)&((~0x50000000)); 
//              *(long*)az_=(*(long*)az_)|(0x40000000*(char)delhd); 
		//remove table 
		for (ptr=(long*)(az_-lw-4);ptr<=(long*)nv;ptr++) 
			*(ptr+lent/4)=*ptr; 
		nv+=lent; 
	} 
	else 
	{ 
		for (i=(long*)nv;i>=(long*)(az);i--) 
			*(i+nlen8/4+1)=*i; 
		*(long*)az=(long)(nb-memory); 
		for (j=0;j<nlen8/4;j++) 
			*(long*)(az+j*4+4)=*(long*)(newword_t+j*4); 
		az=az_; 
		nv+=nlen8+4; 
	} 
	delhd=1; 
} 
 
 
void next_word_cmp() //edit !!! 
{ 
  char ch;  
  isnum=1; 
  nch=0; 
  do 
   { 
    ch=fgetc(in); 
    if (ch=='[') 
      { do ch=fgetc(in); while (ch!=']'); ch=fgetc(in);} 
   } 
  while (isspace(ch)); 
  if ((!isdigit(ch))&&(ch!='-')) isnum=0; 
  if (feof(in)) return; 
  do 
    { 
      if ((ch=='"') && (word_t[0]=='.') && (nch==1)) 
	 { 
		   word_t[1]=word_t[2]=ch; word_t[3]=0; 
	       out_str_cmp(); return;  
	     } 
	  if ((ch=='"') && (nch==0))  
		 { 
		   word_t[0]=word_t[1]=ch; word_t[2]=0; out_str_cmp(); return;  
	     } 
      if ((!isDIGIT(ch))&&(nch>0)) isnum=0; 
      word_t[nch++]=ch; 
      ch=fgetc(in); 
    } 
  while ((!isspace(ch))&&(!feof(in)));   
  word_t[nch]=0; 
} 
void v1_s(long d) 
{ 
	v1-=4; 
	*(long*)v1=d; 
} 
 
void reform(int i) 
{ 
	unsigned int j; 
	long *ptr,lw,lent; 
//        printf("i=%d t=%ld s=%s adr=%lx l=%ld\n",i,ptm[i].type,ptm[i].s,(long)ptm[i].adr_use,ptm[i].l);
	switch (ptm[i].type) 
	{ 
	case 0:  
		{ 
			cfind(ptm[i].l); 
			if (ba==0) 
			{ 
				v1_s((long)(nb-memory)); 
				*(long*)(ptm[i].adr_use)=(long)(nb-memory); 
				l_s(cnstcodev); 
				l_s(ptm[i].l); 
			} 
			else 
			{ 
				*(long*)(ptm[i].adr_use)=ba; 
			} 
			break; 
		} 
	case 1: 
		{ 
			*(long*)(ptm[i].adr_use)=(long)(nb-memory); 
			l_s(txtcodev); 
			l_s(yd1-sds0+beg_yd1v); 
			l_s(strlen(ptm[i].s)); 
			for(j=0;j<=strlen(ptm[i].s);j++) 
				*(yd1++)=*(ptm[i].s+j); 
			break; 
		} 
	case 2:  
		{ 
			*(long*)(ptm[i].adr_use)=(long)(nb-memory); 
			l_s(ptxtcodev); 
			l_s(yd1-sds0+beg_yd1v); 
			l_s(strlen(ptm[i].s)); 
			for(j=0;j<=strlen(ptm[i].s);j++) 
				*(yd1++)=*(ptm[i].s+j); 
			break; 
		} 
//      int type; // 0 - const; 2 - ."" 1 - "" 3 - undefword  
		  // 4 - undefword in voc 
	case 3:  
		{ 
			strcpy(word_t,ptm[i].s); 
			findlen=strlen(word_t);findlen8=(findlen+2+3)&(~3); 
			strcpy(findword_t,"     "); 
			strcpy(findword_t+(findlen8-findlen-2),word_t); 
			findword_t[findlen8-2]=findlen8/4-1; 
			findword_t[findlen8-1]=0; 
			find(); 
			if (ba!=0)  
			{  
				ptm[i].type=4; 
//                              printf("------|\n");
//                                reform(i);
				strcpy(word_t,ptm[i].s); 
				findlen=strlen(word_t);findlen8=(findlen+2+3)&(~3); 
				strcpy(findword_t,"     "); 
				strcpy(findword_t+(findlen8-findlen-2),word_t); 
				findword_t[findlen8-2]=findlen8/4-1; 
				findword_t[findlen8-1]=0; 
				find(); 
				*(long*)(ptm[i].adr_use)=(long)(ba); 
				//move voc 
				lw=*(char*)(az+2); 
				lw=(lw & 7)*4; 
//                              lw=*(long*)az; lw=((lw>>16)&7)*4; 
				lent=*(long*)(az-lw-8); 
				lw=lw-lent+4; 
				for (ptr=(long*)nv;ptr>=(long*)(az-lw);ptr--) 
					*(ptr+1)=*ptr; 
				*(long*)(az-lw)=(long)(ptm[i].adr_use-memory); 
				*(long*)(az-lw-lent)-=4; 
				nv+=4; 
				break;                   
			break;
			} 
			code1_((long)(ptm[i].adr_use-memory)); 
			strcpy(word_t,ptm[i].s); 
			findlen=strlen(word_t);findlen8=(findlen+2+3)&(~3); 
			strcpy(findword_t,"     "); 
			strcpy(findword_t+(findlen8-findlen-2),word_t); 
			findword_t[findlen8-2]=findlen8/4-1; 
			findword_t[findlen8-1]=0; 
			find(); 
			//*(long*)(ptm[i].adr_use)=(long)(nb-memory); 
			//l_s(undefcodev); 
//                      *(long*)az=(*(long*)az)|0x10000000;  // mark undef 
			*(char*)(az+3)|=0x10;  // mark undef 
			//move voc 
			lw=*(char*)(az+2); 
			lw=(lw & 7)*4; 
//                      lw=*(long*)az; lw=((lw>>16)&7)*4; 
			for (ptr=(long*)nv;ptr>=(long*)(az-lw-4);ptr--) 
				*(ptr+2)=*ptr; 
			nv+=8; 
			*(long*)(az-lw-4)=(long)(ptm[i].adr_use-memory); 
			*(long*)(az-lw)=-8; 
			break; 
		} 
	case 4:  
		{ 
			strcpy(word_t,ptm[i].s); 
			findlen=strlen(word_t);findlen8=(findlen+2+3)&(~3); 
			strcpy(findword_t,"     "); 
			strcpy(findword_t+(findlen8-findlen-2),word_t); 
			findword_t[findlen8-2]=findlen8/4-1; 
			findword_t[findlen8-1]=0; 
			find(); 
			*(long*)(ptm[i].adr_use)=(long)(ba); 
			//move voc 
			lw=*(char*)(az+2); 
			lw=(lw & 7)*4; 
//                      lw=*(long*)az; lw=((lw>>16)&7)*4; 
			lent=*(long*)(az-lw-8); 
			lw=lw-lent+4; 
			for (ptr=(long*)nv;ptr>=(long*)(az-lw);ptr--) 
				*(ptr+1)=*ptr; 
			*(long*)(az-lw)=(long)(ptm[i].adr_use-memory); 
			*(long*)(az-lw-lent)-=4; 
			nv+=4; 
			break;                   
		} 
	default:  
		{ 
			printf("Error in cmpltr\n");break; 
		} 
	} 
} 
 
void cmpltr() 
{ 
	//long memword; 
	char *azpc; 
	long num; 
	int i; 
	next_word();  
	findlen=strlen(word_t);findlen8=(findlen+2+3)&(~3); 
	strcpy(findword_t,"     "); 
	strcpy(findword_t+(findlen8-findlen-2),word_t); 
	findword_t[findlen8-2]=findlen8/4-1; 
	findword_t[findlen8-1]=0; 
	find(); 
	azp=*(long*)az; 
	azpc=(char*)&azp; 
	if (ba!=0)  
		if (!((*(char*)(azpc+3))&0x10)) 
			printf("Found %s in voc\n",word_t); 
	code_(); 
	l_s(intcodev); 
	for (;;) 
	{ 
		next_word_cmp(); 
		if (word_t[0]=='#') 
		{ 
			num=word_t[1]; 
			if (num==0) num=' '; // ┬ юЁшушэрых ъюьяюэют∙шъ ЁрсюЄрхЄ ё тї. ёЄЁюъющ, р эх ё т√фхыхээ√ь ёыютюь 
			PTM("",num,nb,0); 
			l_s(0); 
			//write memword 
			continue; 
		} 
		findlen=strlen(word_t);findlen8=(findlen+2+3)&(~3); 
		strcpy(findword_t,"     "); 
		strcpy(findword_t+(findlen8-findlen-2),word_t); 
		findword_t[findlen8-2]=findlen8/4-1; 
		findword_t[findlen8-1]=0; 
		find(); 
		if (isnum) 
			if (ba==0) 
			{ 
				num=atob(word_t); 
				PTM("",num,nb,0); 
				l_s(0); 
				//write memword 
				continue; 
			} 
		if ((word_t[0]=='.' && word_t[1]=='"' && word_t[2]=='"')|| 
		    (word_t[0]=='"' && word_t[1]=='"')) 
		{ 
			PTM(str,0,nb,strlen(word_t)-1); 
			l_s(0); 
			//write memword 
			continue; 
		} 
		if (ba==0) //undefword 
		{ 
				PTM(word_t,0,nb,3); 
				l_s(undefcodev); 
		} 
		else  
			if      (((*(char*)(az+3)))&0x10) //undef in voc 
			{ 
				//write memword 
				PTM(word_t,0,nb,4); 
				l_s(ba); 
			} 
			else  
			{ 
				l_s(ba); 
				//write memword 
			} 
 
		if (strcmp(word_t,";")==0)  
		{ 
			break; 
		} 
	} 
	// reform undef table 
	for (i=1;i<=ptm_n;i++) 
	{ 
		reform(i); 
 
	} 
	ptm_n=0; 
} 
 
void word_() 
{ 
	ldatav=2;       dtypev=1; 
} 
void byte_() 
{ 
	ldatav=1;       dtypev=0; 
} 
void act_() 
{ 
	ldatav=4;       dtypev=3; 
} 
void long_() 
{ 
	ldatav=4;       dtypev=2; 
} 
void fix_() 
{ 
	fix_v=1; 
} 
void own_() 
{ 
	own_v=1; 
} 
void *muse(long q) 
{ 
	unsigned char *ptr1; 
	if (!own_v) 
	{  
		if (fix_v) ptr1=yd1;  
		else ptr1=d1;    
	} 
	else  
		ptr1=wd1;  
	if (((int)(q/4))*4==q) 
		ptr1=(unsigned char*)(((long)ptr1+3)&(~3)); 
	else 
		if (((int)(q/2))*2==q) 
			ptr1=(unsigned char*)(((long)ptr1+1)&(~1)); 
	if (!own_v) 
	{  
		if (fix_v) {yd1=(ptr1+q); ptr1=ptr1-sds0+(unsigned char*)beg_yd1v;} 
		else  
		{ 
			d1=ptr1+q;      //      printf("!!d1!!"); 
		} 
	} 
	else  
	{wd1=ptr1+q; ptr1=ptr1-wds0+(unsigned char*)(beg_wd1v/*+main_ownv*/);}  //!*!*!
	return (void*)(ptr1); 
} 
 
 
void var() 
{ 
	char *azpc; 
	next_word(); 
	if (own_v) 
	{ 
		l_s(__getadr_ownv); 
	} 
	else 
	{ 
		l_s(__getadrv); 
	} 
	findlen=strlen(word_t);findlen8=(findlen+2+3)&(~3); 
	strcpy(findword_t,"     "); 
	strcpy(findword_t+(findlen8-findlen-2),word_t); 
	findword_t[findlen8-2]=findlen8/4-1; 
	findword_t[findlen8-1]=0; 
	find(); 
	azp=*(long*)az; 
	azpc=(char*)&azp; 
	if (ba!=0)  
		if (!((*(char*)(azpc+3))&0x10)) 
			printf("Found %s in voc\n",word_t); 
	code_(); 
	l_s(varcodev); 
	switch (dtypev) 
	{ 
	case 0:  
		{       l_s(__bvarpv);break;    } 
	case 1:  
		{       l_s(__wvarpv);break;    } 
	case 2:  
		{       l_s(__lvarpv);break;    } 
	case 3:  
		{       l_s(__avarpv);break;    } 
	default: {printf("Err\n");} 
	} 
	if (tabsolutev==0) 
	{ 
		if (tdummyv) 
			l_s((long)muse(0)); 
		else 
			l_s((long)muse(ldatav)); 
	} 
	else 
		l_s(tabsolutev); 
//      printf("var=%s dtype=%ld adr=%lx\n",word,(long)dtypev,*(long*)(nb-4));
	dsreset(); 
} 
 
void vctr() 
{ 
	char *azpc; 
	next_word(); 
	st_op[opsp_t]++; 
	if (own_v) 
	{ 
		l_s(__getadr_ownv); 
	} 
	else 
	{ 
		l_s(__getadrv); 
	} 
	findlen=strlen(word_t);findlen8=(findlen+2+3)&(~3); 
	strcpy(findword_t,"     "); 
	strcpy(findword_t+(findlen8-findlen-2),word_t); 
	findword_t[findlen8-2]=findlen8/4-1; 
	findword_t[findlen8-1]=0; 
	find(); 
	azp=*(long*)az; 
	azpc=(char*)&azp; 
	if (ba!=0)  
		if (!((*(char*)(azpc+3))&0x10)) 
			printf("Found %s in voc\n",word_t); 
	code_(); 
	l_s(varcodev); 
	l_s(__vctrpv);
/*      switch (dtypev) 
	{ 
	case 0:  
		{       l_s(__bvarpv);break;    } 
	case 1:  
		{       l_s(__wvarpv);break;    } 
	case 2:  
		{       l_s(__lvarpv);break;    } 
	case 3:  
		{       l_s(__avarpv);break;    } 
	default: {printf("Err\n");} 
	} 
*/ 
	if (tabsolutev==0) 
	{ 
		if (tdummyv) 
			l_s((long)muse(0)); 
		else 
			l_s((long)muse(ldatav*st_op[opsp_t])); 
	} 
	else 
		l_s(tabsolutev); 
	l_s(st_op[opsp_t--]); 
	switch (dtypev) 
	{ 
	case 0:  
		{       l_s(__bvarpv);break;    } 
	case 1:  
		{       l_s(__wvarpv);break;    } 
	case 2:  
		{       l_s(__lvarpv);break;    } 
	case 3:  
		{       l_s(__avarpv);break;    } 
	default: {printf("Err\n");} 
	} 
	dsreset(); 
} 
void value() 
{ 
	char *azpc; 
	next_word(); 
	findlen=strlen(word_t);findlen8=(findlen+2+3)&(~3); 
	strcpy(findword_t,"     "); 
	strcpy(findword_t+(findlen8-findlen-2),word_t); 
	findword_t[findlen8-2]=findlen8/4-1; 
	findword_t[findlen8-1]=0; 
	find(); 
	azp=*(long*)az; 
	azpc=(char*)&azp; 
	if (ba!=0)  
		if (!((*(char*)(azpc+3))&0x10)) 
			printf("Found %s in voc\n",word_t); 
	code_(); 
	l_s(cnstcodev); 
	l_s(st_op[opsp_t--]); 
} 
 
void absolute() 
{ 
	tabsolutev=st_op[opsp_t--]; 
} 
 
void setv() //! 
{ 
	char *azpc; 
	unsigned char *data_adr; 
	long *ptrd,*pdata; 
	long ne; 
	next_word(); 
	findlen=strlen(word_t);findlen8=(findlen+2+3)&(~3); 
	strcpy(findword_t,"     "); 
	strcpy(findword_t+(findlen8-findlen-2),word_t); 
	findword_t[findlen8-2]=findlen8/4-1; 
	findword_t[findlen8-1]=0; 
	find(); 
	azp=*(long*)az; 
	azpc=(char*)&azp; 
	if (ba!=0)  
		if (((*(char*)(azpc+3))&0x10)) 
			printf("Not found %s in voc\n",word_t); 
/*      pdata=(long*)(memory+ba); 
	printf("pdata=%lx *pdata=%lx *pdata+1=%lx *pdata+2=%lx *pdata+3=%lx *pdata+4=%lx \n",
			(long)pdata,*pdata, *(pdata+1), *(pdata+2), *(pdata+3), *(pdata+4)); 
*/
	pdata=(long*)(memory+ba+4); 
	data_adr=(unsigned char*)(memory+ba+8); 
    /*    if (*(pdata)==__vctrpv) data_adr+=4;  */
	data_adr=(unsigned char*)*(long*)data_adr; 
	for (;;) 
	{ 
		if (((long)data_adr>=beg_yd1v)&&((long)data_adr<end_yd1v))  
			{       ptrd=(long*)(data_adr-(unsigned char*)beg_yd1v+sds0); break;    } 
		if (((long)data_adr>=(beg_wd1v/*+main_own*/))&&((long)data_adr<(end_wd1v/*+main_own*/))&&(*(long*)(memory+ba-4)==__getadr_ownv))   // !*!*!
			{ 
			ptrd=(long*)(data_adr-(unsigned char*)(beg_wd1v/*+main_own*/)+wds0); break;    } 
		if ((long)data_adr<beg_d1v)  
			{       ptrd=(long*)((long)data_adr+memory); break;     } 
	} 
		for(;;) 
		{ 
/*                printf ("''bvar@=%lx ''wvar@=%lx ''lvar@=%lx ''avar@=%lx ''vctr@=%lx \n",
			__bvarpv,__wvarpv,__lvarpv,__avarpv,__vctrpv);
		printf("pdata=%lx *pdata=%lx *pdata+1=%lx *pdata+2=%lx *pdata+3=%lx *pdata+4=%lx \n",
			(long)pdata,*pdata, *(pdata+1), *(pdata+2), *(pdata+3), *(pdata+4));
*/                
		if(*(pdata)==__bvarpv) 
			{ 
				*(unsigned char*)ptrd=(unsigned char)st_op[opsp_t--];break; 
			} 
		if(*(pdata)==__wvarpv) 
			{ 
				*(unsigned short*)ptrd=(unsigned short)st_op[opsp_t--];break; 
			} 
		if(*(pdata)==__lvarpv) 
			{ 
				*(unsigned long*)ptrd=(unsigned long)st_op[opsp_t--];break; 
			} 
		if(*(pdata)==__avarpv) 
			{ 
				*(unsigned long*)ptrd=(unsigned long)st_op[opsp_t--];break; 
			} 
		if((*(pdata+3)==__bvarpv) && (*(pdata)==__vctrpv))
			{ 
				ne=st_op[opsp_t--]; 
				*((unsigned char*)ptrd+ne)=(unsigned char)st_op[opsp_t--];break; 
			} 
		if((*(pdata+3)==__wvarpv) && (*(pdata)==__vctrpv)) 
			{ 
				ne=st_op[opsp_t--]; 
				*((unsigned short*)ptrd+ne)=(unsigned short)st_op[opsp_t--];break; 
			} 
		if((*(pdata+3)==__lvarpv)  && (*(pdata)==__vctrpv))
			{ 
				ne=st_op[opsp_t--]; 
				*((unsigned long*)ptrd+ne)=(unsigned long)st_op[opsp_t--];break; 
			} 
		if((*(pdata+3)==__avarpv)  && (*(pdata)==__vctrpv))
			{ 
				ne=st_op[opsp_t--]; 
				*((unsigned long*)ptrd+ne)=(unsigned long)st_op[opsp_t--];break; 
			} 
		printf("Err in !\n"); break; 
		} 
} 
void gadrv() //'
{ 
	char *azpc; 
	unsigned char *data_adr; 
	long *ptrd,*pdata; 
	long ne; 
	next_word(); 
	findlen=strlen(word_t);findlen8=(findlen+2+3)&(~3); 
	strcpy(findword_t,"     "); 
	strcpy(findword_t+(findlen8-findlen-2),word_t); 
	findword_t[findlen8-2]=findlen8/4-1; 
	findword_t[findlen8-1]=0; 
	find(); 
	azp=*(long*)az; 
	azpc=(char*)&azp; 
	if (ba!=0)  
		if (((*(char*)(azpc+3))&0x10)) 
			printf("Not found %s in voc (') \n",word_t); 
	pdata=(long*)(memory+ba+4); 
	data_adr=(unsigned char*)(memory+ba+8); 
/*      if (*(pdata)==__vctrpv) data_adr+=4; */
	data_adr=(unsigned char*)*(long*)data_adr; 
	for (;;) 
	{ 
		if (((long)data_adr>=beg_yd1v)&&((long)data_adr<end_yd1v))  
			{       ptrd=(long*)(data_adr-(unsigned char*)beg_yd1v+sds0); break;    } 
		if (((long)data_adr>=(beg_wd1v/*+main_own*/))&&((long)data_adr<(end_wd1v/*+main_own*/)))  // !*!*!
			{       ptrd=(long*)(data_adr-(unsigned char*)(beg_wd1v/*+main_own*/)+wds0); break;    } 
		if ((long)data_adr<beg_d1v)  
			{       ptrd=(long*)((long)data_adr+memory); break;     } 
		{       ptrd=(long*)((long)data_adr); break;    } 
	} 
		for(;;) 
		{ 
		if(*(pdata)==__bvarpv) 
			{ 
				st_op[++opsp_t]=(long)ptrd;break; 
			} 
		if(*(pdata)==__wvarpv) 
			{ 
				st_op[++opsp_t]=(long)ptrd;break; 
			} 
		if(*(pdata)==__lvarpv) 
			{ 
				st_op[++opsp_t]=(long)ptrd;break; 
			} 
		if(*(pdata)==__avarpv) 
			{ 
				st_op[++opsp_t]=(long)ptrd;break; 
			} 
		if((*(pdata+3)==__bvarpv)  && (*(pdata)==__vctrpv))
			{ 
				ne=st_op[opsp_t--];
				st_op[++opsp_t]=(long)ptrd+ne*1;break; 
 
				*((unsigned char*)ptrd+ne)=(unsigned char)st_op[opsp_t--];break; 
			} 
		if((*(pdata+3)==__wvarpv)  && (*(pdata)==__vctrpv))
			{ 
				ne=st_op[opsp_t--]; 
				st_op[++opsp_t]=(long)ptrd+ne*2;break; 
			} 
		if((*(pdata+3)==__lvarpv)  && (*(pdata)==__vctrpv))
			{ 
				ne=st_op[opsp_t--]; 
				st_op[++opsp_t]=(long)ptrd+ne*4;break; 
			} 
		if((*(pdata+3)==__avarpv)  && (*(pdata)==__vctrpv))
			{ 
				ne=st_op[opsp_t--]; 
				st_op[++opsp_t]=(long)ptrd+ne*4;break; 
			} 
		printf("Err in '\n"); break; 
		} 
} 

 
void trap() 
{ 
	char *azpc; 
	//long memword; 
	long num; 
	int i; 
	next_word();  
	findlen=strlen(word_t);findlen8=(findlen+2+3)&(~3); 
	strcpy(findword_t,"     "); 
	strcpy(findword_t+(findlen8-findlen-2),word_t); 
	findword_t[findlen8-2]=findlen8/4-1; 
	findword_t[findlen8-1]=0; 
	find(); 
	azp=*(long*)az; 
	azpc=(char*)&azp; 
	if (ba!=0)  
		if (!((*(char*)(azpc+3))&0x10)) 
			printf("Found %s in voc\n",word_t); 
	code_(); 
	l_s(intcodev); 
	l_s(__raisev); 
	for(;;) 
	{ 
		next_word_cmp(); 
		if (word_t[0]=='#') 
		{ 
			num=word_t[1]; 
			PTM("",num,nb,0); 
			l_s(0); 
			//write memword 
//                      continue; 
			break; 
		} 
		findlen=strlen(word_t);findlen8=(findlen+2+3)&(~3); 
		strcpy(findword_t,"     "); 
		strcpy(findword_t+(findlen8-findlen-2),word_t); 
		findword_t[findlen8-2]=findlen8/4-1; 
		findword_t[findlen8-1]=0; 
		find(); 
		if (isnum) 
			if (ba==0) 
			{ 
				num=atob(word_t); 
				PTM("",num,nb,0); 
				l_s(0); 
				//write memword 
				//continue; 
				break; 
			} 
		if ((word_t[0]=='.' && word_t[1]=='"' && word_t[2]=='"')|| 
		    (word_t[0]=='"' && word_t[1]=='"')) 
		{ 
			PTM(str,0,nb,strlen(word_t)-1); 
			l_s(0); 
			//write memword 
			//continue; 
			break; 
		} 
		if (ba==0) //undefword 
		{ 
				PTM(word_t,0,nb,3); 
				l_s(0); 
		} 
		else  
			if      (((*(char*)(az+3)))&0x10) //undef in voc 
			{ 
//                      if      (((*(long*)az)>>24)&0x10) //undef in voc 
//                      { 
				//write memword 
				PTM(word_t,0,nb,4); 
				l_s(ba); 
			} 
			else  
			{ 
				l_s(ba); 
				//write memword 
			} 
 
//              if (strcmp(word,";")==0)  
//              { 
//                      break; 
//              } 
		break; 
	} 
	// reform undef table 
	l_s(__tzv); 
	for (i=1;i<=ptm_n;i++) 
	{ 
		reform(i); 
 
	} 
	ptm_n=0; 
} 
 
void channel() 
{ 
	char *azpc; 
	//long memword; 
	next_word();  
	findlen=strlen(word_t);findlen8=(findlen+2+3)&(~3); 
	strcpy(findword_t,"     "); 
	strcpy(findword_t+(findlen8-findlen-2),word_t); 
	findword_t[findlen8-2]=findlen8/4-1; 
	findword_t[findlen8-1]=0; 
	find(); 
	azp=*(long*)az; 
	azpc=(char*)&azp; 
	if (ba!=0)  
		if (!((*(char*)(azpc+3))&0x10)) 
			printf("Found %s in voc\n",word_t); 
	code_(); 
	l_s(nopcode); 
	l_s((long)muse(0x300)); 
} 
 
void sub1(void) 
{ 
	st_op[opsp_t]--; 
} 
 
 
 
 
 
void copyw() 
{ 
	char *azpc; 
	long ba1; 
	next_word();  
	findlen=strlen(word_t);findlen8=(findlen+2+3)&(~3); 
	strcpy(findword_t,"     "); 
	strcpy(findword_t+(findlen8-findlen-2),word_t); 
	findword_t[findlen8-2]=findlen8/4-1; 
	findword_t[findlen8-1]=0; 
	find(); 
	azp=*(long*)az; 
	azpc=(char*)&azp; 
//      if (ba==0)  
//              if (!((*(char*)(azpc+3))&0x10)) 
//                      printf("Found %s in voc\n",word_t); 
	if ((ba==0) || (((*(char*)(azpc+3))&0x10)!=0)) 
	{ 
		printf("%s not found [CW]\n",word_t); 
		next_word(); 
		return; 
	} 
	ba1=ba; 
	next_word(); 
	findlen=strlen(word_t);findlen8=(findlen+2+3)&(~3); 
	strcpy(findword_t,"     "); 
	strcpy(findword_t+(findlen8-findlen-2),word_t); 
	findword_t[findlen8-2]=findlen8/4-1; 
	findword_t[findlen8-1]=0; 
	find(); 
	azp=*(long*)az; 
	azpc=(char*)&azp; 
	if (ba!=0)  
		if (!((*(char*)(azpc+3))&0x10)) 
			printf("Found %s in voc\n",word_t); 
	code1_(ba1); 
} 
 
void codew() 
{ 
	char *azpc; 
	//long memword; 
	next_word();  
	findlen=strlen(word_t);findlen8=(findlen+2+3)&(~3); 
	strcpy(findword_t,"     "); 
	strcpy(findword_t+(findlen8-findlen-2),word_t); 
	findword_t[findlen8-2]=findlen8/4-1; 
	findword_t[findlen8-1]=0; 
	find(); 
	azp=*(long*)az; 
	azpc=(char*)&azp; 
	if (ba!=0)  
		if (!((*(char*)(azpc+3))&0x10)) 
			printf("Found %s in voc\n",word_t); 
	code_(); 
} 
 
void int_() 
{ 
	l_s(intcodev); 
} 
void copy() 
{ 
	long a; 
	a=st_op[opsp_t]; 
	st_op[++opsp_t]=a; 
} 
void drop() 
{ 
	opsp_t--; 
} 
 
void busew() 
{ 
	long *nbo,q; 
	q=st_op[opsp_t--]; 
	if (v1-vs0<30) movevoc(); 
	if (((int)(q/4))*4==q) 
		nb=(unsigned char*)(((long)nb+3)&(~3)); 
	else 
		if (((int)(q/2))*2==q) 
			nb=(unsigned char*)(((long)nb+1)&(~1)); 
	nbo=(long*)nb; 
	nb+=q; 
	st_op[++opsp_t]=(long)nbo; 
} 
 
void nopcodew() 
{ 
	st_op[++opsp_t]=nopcode; 
} 
void noplenw() 
{ 
	st_op[++opsp_t]=noplen; 
} 
 
void musew() 
{ 
	unsigned char *ptr1; 
	long q; 
	q=st_op[opsp_t--]; 
	if (!own_v) 
	{  
		if (fix_v) ptr1=yd1;  
		else ptr1=d1;    
	} 
	else  
		ptr1=wd1;  
	if (((int)(q/4))*4==q) 
		ptr1=(unsigned char*)(((long)ptr1+3)&(~3)); 
	else 
		if (((int)(q/2))*2==q) 
			ptr1=(unsigned char*)(((long)ptr1+1)&(~1)); 
	if (!own_v) 
	{  
		if (fix_v) {yd1=(ptr1+q); ptr1=ptr1-sds0+(unsigned char*)beg_yd1v;} 
		else  
		{ 
			d1=ptr1+q;      //      printf("!!d1!!"); 
		} 
	} 
	else  
		{wd1=ptr1+q; ptr1=ptr1-wds0+(unsigned char*)(beg_wd1v/*+main_own*/);} 
 
	st_op[++opsp_t]=(long)(ptr1); 
} 
 
void typeptr() 
{ 
	printf("v1=%ld, nv=%ld, nb=%ld, yd1=%ld, wd1=%ld, d1=%ld, ownbase=%ld\n", 
		    v1-vs0,nv-vs0,nb-memory,yd1-sds0+beg_yd1v,wd1-wds0+beg_wd1v/*+main_own*/,d1,main_ownv); 
} 
 
void flags(int fl,FILE *out) 
{ 
	if (fl&0x80) fputc('T',out); else fputc('.',out);  
	if (fl&0x40) fputc('D',out); else fputc('.',out);  
	if (fl&0x20) fputc('P',out); else fputc('.',out);  
	if (fl&0x10) fputc('U',out); else fputc('.',out);  
	fputc('.',out);  
	if (fl&0x04) fputc('C',out); else fputc('.',out);  
	fputc('.',out);  
	fputc('.',out);  
} 
 
void fqnd() 
{ 
	char names[32]; 
	long lw; 
	FILE *out; 
	az=nv-8; 
	out=fopen("qnd.txt","w"); 
	//out=stdout; 
//      lw=*(long*)az; 
//      lw=((lw>>16)&7)*4; 
	lw=(long)*(char*)(az+2); 
	lw=(lw & 7)*4; 
	strncpy((char*)names,(char*)(az-lw),lw+2); 
	names[lw+2]=0; 
	fprintf(out,"Voc %s\n\n",names); 
	az=nv;is0_4();az-=4; 
	for(;;) 
	{ 
		if (*(long*)(az)==0) break; 
		lw=(long)*(char*)(az+2); 
		lw=(lw & 7)*4; 
		flags(( ( *(char*)(az+3) )&0x0ff ),out); 
		strncpy(names,(char*)(az-lw),lw+2); 
		names[lw+2]=0; 
		fprintf(out," %32s   %08lx\n",names,*(long*)(az-lw-4)); 
		az-=(lw+8); 
		if (*(char*)(az+3)<0) 
		{ 
			for(lw=(*(long*)az+4);lw<0;lw+=4) 
			{ 
				fprintf(out,"  %08lx",*(long*)(az-4)); 
				az-=4; 
			} 
			fprintf(out,"\n");az-=4; 
		} 
	} 
	fclose(out); 
}        
 
void neopr() 
{ 
	char names[32]; 
	long lw,azp; 
	FILE *out; 
	char *azpc; 
	az=nv-8; 
//      out=fopen("qnd.txt","w"); 
	out=stdout; 
//      lw=*(long*)az; 
//      lw=((lw>>16)&7)*4; 
//      strncpy(names,az-lw,lw+2); 
//      names[lw+2]=0; 
//      fprintf(out,"Voc %s\n\n",names); 
	az=nv;is0_4();az-=4; 
	for(;;) 
	{ 
		if (*(long*)(az)==0) break; 
		lw=(long)*(char*)(az+2); 
		lw=(lw & 7)*4; 
//              lw=*(long*)az; 
//              lw=((lw>>16)&7)*4; 
		azp=*(long*)az; 
		azpc=(char*)&azp; 
		strncpy(names,(char*)(az-lw),lw+2); 
		names[lw+2]=0; 
		az-=(lw+8); 
		if (((*(char*)(azpc+3))&0x10)) 
		{ 
			fprintf(out,"Unknow %32s\n",names); 
			if (*(long*)az<0) 
			{ 
				for(lw=(*(long*)az+4);lw<0;lw+=4) 
				{ 
					fprintf(out,"  %08lx",*(long*)(az-4)); 
					az-=4; 
				}  
				fprintf(out,"\n");az-=4; 
			} 
		} 
	} 
//      fclose(out); 
}        
 
void setv_sp(char *str) //! 
{ 
	char *azpc; 
	unsigned char *data_adr; 
	long *ptrd,*pdata; 
	long ne; 
	strcpy(word_t,str); 
//      next_word(); 
	findlen=strlen(word_t);findlen8=(findlen+2+3)&(~3); 
	strcpy(findword_t,"     "); 
	strcpy(findword_t+(findlen8-findlen-2),word_t); 
	findword_t[findlen8-2]=findlen8/4-1; 
	findword_t[findlen8-1]=0; 
	find(); 
	azp=*(long*)az; 
	azpc=(char*)&azp; 
	if (ba!=0)  
		if ( (*(char*)(azpc+3))&0x10) 
			printf("Not found %s in voc\n",word_t); 
	pdata=(long*)(memory+ba+4); 
	data_adr=(unsigned char*)(memory+ba+8); 
	data_adr=(unsigned char*)*(long*)data_adr; 
	for (;;) 
	{ 
		if (((long)data_adr>=beg_yd1v)&&((long)data_adr<end_yd1v))  
			{       ptrd=(long*)(data_adr-(unsigned char*)beg_yd1v+sds0); break;    } 
		if (((long)data_adr>=(beg_wd1v/*+main_own*/))&&((long)data_adr<(end_wd1v/*+main_own*/)))  
			{       ptrd=(long*)(data_adr-(unsigned char*)(beg_wd1v/*+main_own*/)+wds0); break;    } 
		if ((long)data_adr<beg_d1v)  
			{       ptrd=(long*)((long)data_adr+memory); break;     } 
	} 
		for(;;) 
		{ 
		if(*(pdata)==__bvarpv) 
			{ 
				*(unsigned char*)ptrd=(unsigned char)st_op[opsp_t--];break; 
			} 
		if(*(pdata)==__wvarpv) 
			{ 
				*(unsigned short*)ptrd=(unsigned short)st_op[opsp_t--];break; 
			} 
		if(*(pdata)==__lvarpv) 
			{ 
				*(unsigned long*)ptrd=(unsigned long)st_op[opsp_t--];break; 
			} 
		if(*(pdata)==__avarpv) 
			{ 
				*(unsigned long*)ptrd=(unsigned long)st_op[opsp_t--];break; 
			} 
		if(*(pdata)==__bvctpv) 
			{ 
				ne=st_op[opsp_t--]; 
				*((unsigned char*)ptrd+ne)=(unsigned char)st_op[opsp_t--];break; 
			} 
		if(*(pdata)==__wvctpv) 
			{ 
				ne=st_op[opsp_t--]; 
				*((unsigned short*)ptrd+ne)=(unsigned short)st_op[opsp_t--];break; 
			} 
		if(*(pdata)==__lvctpv) 
			{ 
				ne=st_op[opsp_t--]; 
				*((unsigned long*)ptrd+ne)=(unsigned long)st_op[opsp_t--];break; 
			} 
		if(*(pdata)==__avctpv) 
			{ 
				ne=st_op[opsp_t--]; 
				*((unsigned long*)ptrd+ne)=(unsigned long)st_op[opsp_t--];break; 
			} 
		printf("Err in !\n"); break; 
		} 
} 
 
long kk_sp(char*str) 
{ 
	strcpy(word_t,str); 
//      next_word();  
	findlen=strlen(word_t);findlen8=(findlen+2+3)&(~3); 
	strcpy(findword_t,"     "); 
	strcpy(findword_t+(findlen8-findlen-2),word_t); 
	findword_t[findlen8-2]=findlen8/4-1; 
	findword_t[findlen8-1]=0; 
	find(); 
	if (ba==0) printf("Unknow K'' %s\n",word_t);  
	return ba; 
} 
 
 
void lnk() 
{ 
	st_op[++opsp_t]=(long)(nb-memory); 
	setv_sp("NB"); // ! NB 
	st_op[++opsp_t]=(long)(yd1-sds0+beg_yd1v); 
	setv_sp("YD1"); // ! YD1 
	st_op[++opsp_t]=(long)(wd1-wds0+beg_wd1v/*+main_own*/); 
	setv_sp("WD1"); // ! WD1 
	st_op[++opsp_t]=(long)(d1); 
	setv_sp("D1"); // ! D1 
	st_op[++opsp_t]=(long)(nb-memory); 
	setv_sp("V1"); // ! V1 [V1=NB] 
	st_op[++opsp_t]=(long)(nb-memory+(nv-v1)); 
	setv_sp("NV"); // ! NV 
	st_op[++opsp_t]=5; 
	setv_sp("NOMVERS"); // ! NOMVERS 
	st_op[++opsp_t]=01; 
	setv_sp("NOMRED"); // ! NOMRED 
	st_op[++opsp_t]=csbv; 
	ba=kk_sp("ARSB"); 
	*(long*)(memory+ba)=st_op[opsp_t--]; 
	st_op[++opsp_t]=opsbv; 
	ba=kk_sp("AOPSB"); 
	*(long*)(memory+ba)=st_op[opsp_t--]; 
	st_op[++opsp_t]=__puskv; 
	ba=kk_sp("APUSK"); 
	*(long*)(memory+ba)=st_op[opsp_t--]; 
	printf("\nAll memory     %08lx\n",end_memv); 
	*(long*)(header+4)=(long)end_memv; 
	*(long*)(header+8)=4; 
	printf("Body           %08lx %08lx\n",0,nb-memory); 
	*(long*)(header+12)=0; 
	*(long*)(header+16)=(long)(nb-memory); 
	printf("Voc            %08lx %08lx\n",nb-memory,nv-v1); 
	*(long*)(header+20)=(long)(nb-memory); 
	*(long*)(header+24)=(long)(nv-v1); 
	printf("Save data      %08lx %08lx\n",beg_yd1v,yd1-sds0); 
	*(long*)(header+28)=(long)(beg_yd1v); 
	*(long*)(header+32)=(long)(yd1-sds0); 
	printf("Own data       %08lx %08lx\n",beg_wd1v+main_ownv,wd1-wds0); // !*!*!
	*(long*)(header+36)=(long)(beg_wd1v+main_ownv);  // !*!*!
	*(long*)(header+40)=(long)(wd1-wds0); 
} 
  
void svs() 
{ 
	FILE *outv; 
	next_word(); 
	outv=fopen(word_t,"wb"); 
	if (outv==NULL) {printf("Error writing %s\n",word_t);return;} 
	printf("Write in file %s:\n",word_t); 
	strcpy((char*)header,"DPF"); 
	*(long*)(header+4)=(long)end_memv; 
	fwrite(header,sizeof(char),96,outv); 
	printf("      Header %08lx\n",96); 
	fwrite(memory,sizeof(char),(nb-memory),outv); 
	printf("        Body %08lx\n",(nb-memory)); 
	fwrite(v1,sizeof(char),(nv-v1),outv); 
	printf("         Voc %08lx\n",(nv-v1)); 
//      fclose(outv); 
	fwrite(sds0,sizeof(char),(yd1-sds0),outv); 
	printf("   Save data %08lx\n",(yd1-sds0)); 
	fwrite(wds0,sizeof(char),(wd1-wds0),outv); 
	printf("    Own data %08lx\n",(wd1-wds0)); 
	fclose(outv); 
}
void _notst() 
{ 
	if (st_op[opsp_t])  st_op[opsp_t]=0; else st_op[opsp_t]=1; 
} 

void define_()
{
	char *azpc; 
	//long memword; 
	next_word();  
	findlen=strlen(word_t);findlen8=(findlen+2+3)&(~3); 
	strcpy(findword_t,"     "); 
	strcpy(findword_t+(findlen8-findlen-2),word_t); 
	findword_t[findlen8-2]=findlen8/4-1; 
	findword_t[findlen8-1]=0; 
	find(); 
	azp=*(long*)az; 
	azpc=(char*)&azp; 
	if (ba!=0)  
		st_op[++opsp_t]=1;
	else
		st_op[++opsp_t]=0;
}
