
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

#endif
