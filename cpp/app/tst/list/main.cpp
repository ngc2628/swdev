
#include <stdio.h>
#include <stdlib.h>
#include <mkbase/mkmath.h>
#include <mkbase/mkconv.h>
#include <mkbase/mkla.h>
#include <mkbase/mkutil.h>
#include <app/tst/list/list.h>

extern int tstlist(struct tst_list *);

int usage() {

  printf ("\n");
  return 0;

}

int main(int argc,char **argv) {

  int ii=0,jj=0,idxl=-1,idxh=-1,cnt=17;
  double zero=.0,one=1.;
  mk_vertexnan(vv);
  mk_string strdbg;
  mk_stringset(strdbg,0);

  Mgr mgr(cnt);
  mgr.dertypeidL.cmp=tst_cmpTypeIdRef;
  tstlist(&mgr.dertypeidL);
  TypeId *tt=0;
  
  for (ii=0;ii<10;ii++) {
    tst_listat(&mgr.dertypeidL,ii,(void*)&tt);
    tt->toString(strdbg);
    printf("%d %s %p\n",__LINE__,&strdbg[0],(void*)tt);
    mk_stringset(strdbg,0);
  }

  TypeId look("[type5]");
  mgr.dertypeidL.cmp=tst_cmpTypeIdRefiType;
  tst_listfind2(&mgr.dertypeidL,(void*)&look,&idxl,&idxh);
  mgr.dertypeidL.cmp=tst_cmpTypeIdRef;
  printf("%d idx [%d,%d]\n",__LINE__,idxl,idxh);
  for (ii=MAX(0,idxl);ii<=idxh;ii++) {
    tst_listat(&mgr.dertypeidL,ii,(void*)&tt);
    tt->toString(strdbg);
    printf("%d %s\n",__LINE__,&strdbg[0]);
    mk_stringset(strdbg,0);
    tst_listat(&((DerTypeId*)tt)->vertexL,0,(void*)&vv);
    mk_vertexdbg(vv,strdbg);
    printf("%d %s\n",__LINE__,&strdbg[0]);
    mk_stringset(strdbg,0);
  }
  
  vv[0]=mk_dnan;
  look=TypeId("[type4]");
  DerTypeId *res=mgr.dertypeid(look);
  if (res)
    tst_listat(&res->vertexL,0,(void*)&vv);
  mk_vertexdbg(vv,strdbg);
  printf("%d %s %p\n",__LINE__,&strdbg[0],(void*)res);

  while (mgr.dertypeidL.count>0) {
    tst_listremove(&mgr.dertypeidL,mgr.dertypeidL.count-1,(void *)&tt);
    delete tt;
  }

  return 0;

}

