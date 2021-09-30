
#ifndef _decl_h_
#define _decl_h_

union tp_ucpd {
  unsigned char ucd[8];
  double dd;
};
extern union tp_ucpd tst_defdnan; 
extern union tp_ucpd tst_defdsnan;
extern union tp_ucpd tst_defdinf;
extern union tp_ucpd tst_defdsinf;
#define tst_dnan tst_defdnan.dd
#define tst_dsnan tst_defdsnan.dd
#define tst_dinf tst_defdinf.dd
#define tst_dsinf tst_defdsinf.dd

#define tst_uidnan 9221120237041090560ULL
/* #define tst_fuidnan() ( { unsigned long long int uixxx=tst_uidnan; *((double*)&uixxx); } ) */
#define tst_fuidnan() ( { union tp_ucpd uixxx={{0,0,0,0,0,0,248,255}}; uixxx.dd; } )
#define tst_dnann tst_fuidnan() 

#define tst_uidinf 9218868437227405312ULL
#define tst_fuidinf() ( { unsigned long long int uixxx=tst_uidinf; *((double*)&uixxx); } )
#define tst_dinff tst_fuidinf() 

#endif
