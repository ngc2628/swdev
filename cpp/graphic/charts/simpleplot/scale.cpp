
//#include <sys/param.h>
#include <graphic/charts/simpleplot/scale.h>
#include <mkbase/mkconv.h>
#include <stdio.h>
#include <math.h>

namespace simpleplot {

Tic::Tic(double val,unsigned char sz,unsigned char drawable,unsigned char ucs) :
  m_val(val),m_size(sz),m_drawable(drawable),m_ucs(ucs) {
  
  mk_stringset(m_str,0);
  
    
}

Tic::Tic(const Tic &ass) :
  m_val(ass.m_val),m_size(ass.m_size),m_drawable(ass.m_drawable) {
      
  if (&ass!=this)
    mk_stringset(m_str,&ass.m_str[0]);
    
}

Tic &Tic::operator=(const Tic &ass) {
      
  m_val=ass.m_val;
  m_size=1;
  m_drawable=1;
  if (&ass!=this)
    mk_stringset(m_str,&ass.m_str[0]);
  return *this;
  
}

bool Tic::operator==(const Tic &cmp) const {
      
  int cc=0;
  mk_dbusted(m_val,cmp.m_val,&cc);
  return (cc==0);
  
}
    
bool Tic::operator<(const Tic &cmp) const {
      
  int cc=0;
  mk_dbusted(m_val,cmp.m_val,&cc);
  return (cc<0);
  
}

int Tic::setUcsText(const aux::Ucsstr &txt) {

  mk_stringset(m_str,0);
  int ii=0,ll=txt.length();
  if (ll>0) {
    if (ll>mk_sz/2-2)
      ll=mk_sz/2-2;
    for (ii=0;ii<ll;ii++) {
      mk_stringsetat(m_str,mk_sz,(char)((txt[ii]>>8)&255));
      mk_stringsetat(m_str,mk_sz,(char)(txt[ii]&255));
    }
    m_ucs=1;
  }
  return 0;

}

int Tic::ucsText(aux::Ucsstr *res) const {

  int ii=0,ll=mk_stringlength(m_str);
  if ((m_ucs&1)>0) {
    ll-=(ll%2);
    int jj=0;
    unsigned short cc=0;
    res->setLength(ll/2);
    for (ii=0;ii<ll-1;ii+=2) {
      cc=(unsigned short)(m_str[ii]<<8)+(unsigned short)m_str[ii+1];
      res->set(jj++,cc);
    }
  }
  else {
    res->setLength(ll);
    *res=&m_str[0];
  }
  return 0;

}

Scale::Scale() : TypeId("linearscale"), 
  m_interval(1.,mk_dprec) {

  mk_listalloc(&m_range,sizeof(Range),128);
  Range range(.0,1.);
  mk_listappend(&m_range,(void*)&range);

}

Scale::~Scale() {

  mk_listfree(&m_range);  

}

Range Scale::currRange() const {

  Range range;
  mk_listat(&m_range,m_range.count-1,(void*)&range);
  return range;

}

int Scale::range(double *min,double *max,int *option) const {

  Range range;
  mk_listat(&m_range,m_range.count-1,(void*)&range);
  if (min)
    *min=range.m_min;
  if (max)
    *max=range.m_max;
  if (option)
    *option=range.m_option;
  return 0;

}

int Scale::effRange(double *min,double *max,int *option) const {

  Range range;
  mk_listat(&m_range,m_range.count-1,(void*)&range);
  if (m_tics.count()>1) {
    if (min)
      *min=m_tics[0].m_val;
    if (max)
      *max=m_tics[m_tics.count()-1].m_val;
  }
  else {
    if (min)
      *min=range.m_cmin;
    if (max)
      *max=range.m_cmax;
  }
  if (option)
    *option=range.m_option;
  return 0;

}

double Scale::setRange(double min,double max,int option) {

  if (mk_isbusted(min)!=0 && mk_isbusted(max)!=0)
    return mk_dnan;
  Range range;
  mk_listat(&m_range,m_range.count-1,(void*)&range);
  if (mk_isbusted(min)!=0)
    min=range.m_min;
  if (mk_isbusted(max)!=0)
    max=range.m_max;
  if (min>max)
    mk_swapf(&min,&max);
  range.m_min=min;
  range.m_max=max;
  mk_listsetat(&m_range,(void*)&range,m_range.count-1,0); 

  if (option>=0)
    rangeOption(&option);  

  double sgn0=mk_dsign(min),sgn1=mk_dsign(max),
         bamin=fabs(min),bamax=fabs(max);
  bool sgns=(bamin>bamax);
  if (sgns)
    mk_swapf(&bamin,&bamax);
  double valdiff=bamax-bamin;
  int magmax=mk_mag(bamax),magmin=mk_mag(bamin),magdiff=mk_mag(valdiff),rdprec=-magmax;
  valdiff=mk_diff(valdiff,.0,mk_ipow10(magdiff-3));
  if (abs(magmax-magmin)>1 || (valdiff==.0 && sgn0!=sgn1)) {
    bamax=mk_roundup(bamax,rdprec);
    if (sgn0==sgn1)
      bamin=.0;
    else
      bamin=bamax;
  }
  else if (valdiff==.0 && sgn0==sgn1) {
    bamin-=mk_ipow10(-rdprec);
    bamax+=mk_ipow10(-rdprec);
  }
  else {
    rdprec=-magdiff;
    bamin=mk_rounddown(bamin,rdprec);
    bamax=mk_roundup(bamax,rdprec);
  }
  if (sgns) {
    mk_swapf(&bamin,&bamax);
    sgn1=mk_dsign(bamax);
  }
  bamin*=sgn0;
  bamax*=sgn1;

  mk_listat(&m_range,m_range.count-1,(void*)&range);
  range.m_cmin=mk_round2(bamin,rdprec);
  range.m_cmax=mk_round2(bamax,rdprec);
  mk_listsetat(&m_range,(void*)&range,m_range.count-1,0); 

  return calcInterval(11,0);

}

double Scale::stackRange(double min,double max,int option) {

  if (m_range.count>=maxRangeStack)
    return mk_dnan;
  Range range;
  mk_listat(&m_range,m_range.count-1,(void*)&range);
  mk_listsetat(&m_range,(void*)&range,m_range.count-1,1); 
  return setRange(min,max,option);

}

int Scale::unstackRange(int ndown) {

  int cnt=m_range.count-1;
  if (ndown<0 || ndown>cnt)
    ndown=cnt;
  Range range;
  while (ndown>0) {
    mk_listremove(&m_range,cnt,(void*)&range);
    cnt--;
    ndown--;
  }
  return cnt;
  
}

int Scale::rangeOption (int *mod) {

  Range range;
  mk_listat(&m_range,m_range.count-1,(void*)&range);

  if (mod && (*mod)>=0) {
    if (((*mod)&typeBoundAutoMin)>0)
      range.m_option|=typeBoundAutoMin;
    else
      range.m_option&=(typeBoundAutoMax|typeBoundStaticMin|typeBoundStaticMax);
    if (((*mod)&typeBoundAutoMax)>0)
      range.m_option|=typeBoundAutoMax;
    else
      range.m_option&=(typeBoundAutoMin|typeBoundStaticMin|typeBoundStaticMax);
    if (((*mod)&typeBoundStaticMin)>0)
      range.m_option|=typeBoundStaticMin;
    else
      range.m_option&=(typeBoundAutoMin|typeBoundAutoMax|typeBoundStaticMax);
    if (((*mod)&typeBoundStaticMax)>0)
      range.m_option|=typeBoundStaticMax;
    else
      range.m_option&=(typeBoundAutoMin|typeBoundAutoMax|typeBoundStaticMin);
    mk_listsetat(&m_range,(void*)&range,m_range.count-1,0);
  }
  return range.m_option;

}

int Scale::tics(aux::TVList<Tic> *ticL) const {

  if (ticL)
    *ticL=m_tics;
  //int ii=0;
  //for (ii=0;ii<m_tics.count();ii++)
    //printf ("%d %d : tic=%s\n",__LINE__,ii,(const char *)m_tics.at(ii)->m_str.asciistr());
  return m_tics.count();

}

aux::Rounded Scale::interval() const {
      
  return m_interval;
  
}

void Scale::clear() {

  m_tics.clear();

}

int Scale::calcTics(int maxtics,aux::TVList<Tic> *ticL) {

  m_tics.clear();
  if (ticL)
    ticL->clear();
  Range range=currRange();
  int ii=0,idx=-1,ntics=0;
  double interval=calcInterval(maxtics,&ntics),pos=range.m_cmin;

  if (mk_isnan(interval) || maxtics<=2)
    return failsave(ticL);
  for (ii=0;ii<ntics;ii++) {
    m_tics.inSort(Tic(pos));
    if (pos>=range.m_cmax)
      break;
    pos+=interval;
  }
  ntics=m_tics.count();
  if (ntics<=2)
    return failsave(ticL);
  if ((currRange().m_option&typeBoundStaticMin)>0) {
    Tic tb0=Tic(range.m_min);
    int idxl=m_tics.findNextIndex(tb0);
    if (idxl<1)
      return failsave(ticL);
    for (ii=idxl-1;ii>-1;ii--)
      m_tics.remove(ii); 
    idx=m_tics.inSort(tb0);
    m_tics.at(idx)->m_drawable=0;
    ntics=m_tics.count();
  }
  if ((range.m_option&typeBoundStaticMax)>0) {
    Tic tb1=Tic(range.m_max);
    int idxh=m_tics.findNextIndex(tb1);
    if (idxh>=ntics)
      return failsave(ticL);
    for (ii=ntics-1;ii>=idxh;ii--)
      m_tics.remove(m_tics.count()-1);
    idx=m_tics.inSort(tb1);
    m_tics.at(idx)->m_drawable=0;
    ntics=m_tics.count();
  }
  if (ntics<=2)
    return failsave(ticL);

  Tic *tic=0;
  for (ii=0;ii<ntics;ii++) {
    tic=m_tics.at(ii);
    mk_d2a(tic->m_val,tic->m_str,m_interval.m_a);
  }

  setMajTics();
  return tics(ticL);

}

int Scale::doShift(double amount) {

  int ii=0,ntics=m_tics.count();
  if (ntics<2)
    return 0;
  double sam=amount,interval=mk_round2(m_interval.m_d,m_interval.m_a);
  // do not rely on highest resolution for the calculation of differences but
  // use a tenth either of the interval size or the shift amount (whatever number is smaller)
  double eps=mk_ipow10(mk_mag(((fabs(sam)>interval) ? interval : sam))-2);
  // do not do nothing when there is nothing to be done
  if (mk_diff(sam,.0,eps)==.0)
    return m_tics.count();
  double direction=mk_dsign(sam);
  // acoording to shift direction higher/lower end must grow/shrink
  int adaptotheredge=0;
  Tic *tic=m_tics.at(0);
  tic->m_val+=sam;
  tic=m_tics.at(ntics-1);
  tic->m_val+=sam;
  double diffleft=mk_diff(m_tics.at(0)->m_val,m_tics.at(1)->m_val,eps),
         diffright=mk_diff(m_tics.at(ntics-1)->m_val,m_tics.at(ntics-2)->m_val,eps);
  bool matchl=(diffleft==.0),matchr=(diffright==.0);
  // shift to the left
  if (direction>.0) {
    // loop as long as the ticmark array does not match the new spanning
    while (diffleft>=.0 && !matchl) {
      // shift ticmark array positions to the left clear the first one (not needed anymore)
      // and generate the suiting new one at the right edge
      rollInnerTics(direction);
      diffleft=mk_diff(m_tics.at(0)->m_val,m_tics.at(1)->m_val,eps);
      diffright=mk_diff(m_tics.at(ntics-1)->m_val,m_tics.at(ntics-2)->m_val,eps);
      matchl=(diffleft==.0);
      matchr=(diffright==.0);
    }
    if (!matchr) {
      if (mk_diff(diffright,interval,eps)>0.0)
        adaptotheredge=1;
      if (diffright<.0)
        adaptotheredge=-1;
    }
    ntics=adaptShiftedTicArray(direction,matchl,adaptotheredge);
  }
  // shift to the right
  else if (direction<.0) {
    while (diffright<=.0 && !matchr) {
      rollInnerTics(direction);
      diffleft=mk_diff(m_tics.at(0)->m_val,m_tics.at(1)->m_val,eps);
      diffright=mk_diff(m_tics.at(ntics-1)->m_val,m_tics.at(ntics-2)->m_val,eps);
      matchl=(diffleft==.0);
      matchr=(diffright==.0);
    }
    if (!matchl) {
      if (mk_diff(fabs(diffleft),interval,eps)>.0)
        adaptotheredge=1;
      if (diffleft>.0)
        adaptotheredge=-1;
    }
    ntics=adaptShiftedTicArray(direction,matchr,adaptotheredge);
  }

  // shifted positions should not be displayed because they usually have no correspondence with the interval size
  //if (CNumUtl::diff(fabs(CNumUtl::diff(m_tarr[0],m_tarr[1])),m_intervalSize)!=0.0) {
  tic=m_tics.at(0);
  if (mk_diff(m_tics.at(1)->m_val-tic->m_val,interval,eps)!=.0)
    tic->m_drawable=0;
  tic=m_tics.at(ntics-1);
  if (mk_diff(tic->m_val-m_tics.at(ntics-2)->m_val,interval,eps)!=.0)
    tic->m_drawable=0;

  setMajTics();
  for (ii=0;ii<ntics;ii++) {
    tic=m_tics.at(ii);
    mk_d2a(tic->m_val,tic->m_str,m_interval.m_a);
  }
  return ntics;

}

double Scale::calcInterval(int maxtics,int *ntics) {

  int cmp=0,db=mk_dbusted(currRange().m_cmin,currRange().m_cmax,&cmp);
  if (db!=0 || 0<=cmp)
    return mk_dnan;
  double bdiff=currRange().m_cmax-currRange().m_cmin,interval=mk_ipow10(mk_mag(bdiff)-2);
  int nntics=(int)mk_roundup(bdiff/interval,0);
  while (nntics<maxtics) {
    interval/=10.;
    nntics=(int)mk_roundup(bdiff/interval,0)+1;
  }
  while (maxtics>2 && nntics>maxtics) {
    interval=growIntervalSize(interval);
    nntics=(int)mk_roundup(bdiff/interval,0)+1;
  }
  int prec=-mk_mag(interval);
  if (mk_round2(interval,prec)>interval)
    prec++; // 2.5 intv
  m_interval.m_d=interval;
  m_interval.m_a=(short)prec;
  if (ntics)
    *ntics=nntics;

  return m_interval.rounded();

}

void Scale::setMajTics () {

  int ii=0,ntics=m_tics.count();
  if (ntics<=0)
    return;
  for (ii=0;ii<ntics;ii++)
    m_tics.at(ii)->m_size=0;
  double val=.0,interval=mk_round2(m_interval.m_d,m_interval.m_a);
  int idx=-1,magintv=mk_mag(interval),
      iinterval=(int)mk_round2(10.*interval/mk_ipow10(magintv));
  Tic *tic=0;
  for (ii=0;ii<ntics;ii++) {
    tic=m_tics.at(ii);
    val=fabs(tic->m_val);
    if (mk_diff(val,.0,mk_ipow10(magintv-1))==.0) {
      tic->m_size=1;
      continue;
    }
    if (mk_diff(mk_roundup(val,-magintv-1),
             mk_rounddown(val,-magintv-1),mk_ipow10(magintv))==.0) {
      idx=ii;
      break;
    }
    if (iinterval==10 &&
        mk_diff(mk_roundup(2.*val,-magintv-1),
             mk_rounddown(2.*val,-magintv-1),mk_ipow10(magintv))==.0) {
      idx=ii;
      break;
    }
  }
  if (idx>=0) {
    int rangeoption=rangeOption();
    m_tics.at(idx)->m_size=1;
    if (iinterval==50 && (idx-2>0 || (idx-2==0 && (rangeoption&typeBoundStaticMin)==0)))
      m_tics.at(idx-2)->m_size=1;
    while (idx<ntics) {
      if (iinterval==10 || iinterval==20)
        idx+=5;
      else if (iinterval==25)
        idx+=4;
      else if (iinterval==50)
        idx+=2;
      if (idx<ntics-1 || (idx==ntics-1 && (rangeoption&typeBoundStaticMax)==0))
        m_tics.at(idx)->m_size=1;
    }
  }
  
}

double Scale::growIntervalSize(double size) {

  double lggrow=log10(2.),tmp=0.,lower=log10(size),higher=lower+lggrow;
  if (((int)higher>(int)lower) || (mk_dsign(higher)!=mk_dsign(lower))) {
    tmp=mk_ipow10((int)floor(higher));
    if (tmp>size)
      return tmp;
  }
	// in case we want an interval like 2.5 rather than 4
	if (((int)mk_round2(higher)>(int)mk_round2(lower)) || 
       (mk_dsign(higher)!=mk_dsign(lower))) {
		tmp=mk_ipow10((int)mk_round2(higher))/4.0;
    if (tmp>size)
      return tmp;
	}
	//
  lower+=lggrow;
  higher+=lggrow;
  if (((int)higher>(int)lower) || (mk_dsign(higher)!=mk_dsign(lower))) {
    tmp=mk_ipow10((int)floor(higher))/2.0;
    if (tmp>size)
      return tmp;
  }

  return 2.0*size;

}

void Scale::rollInnerTics(double direction) {

  int ii=0,ntics=m_tics.count();
  if (ntics<3)
    return;
  Tic *tic=0;
  double interval=mk_round2(m_interval.m_d,m_interval.m_a);
  if (direction>.0) {
    for (ii=2;ii<(ntics-2);ii++)
      m_tics.replace(ii-1,*m_tics.at(ii));
    tic=m_tics.at(ntics-2);
    tic->m_val=m_tics.at(ntics-3)->m_val+interval;
  }
  if (direction<.0) {
    for (ii=(ntics-2);ii>1;ii--)
      m_tics.replace(ii,*m_tics.at(ii-1));
    tic=m_tics.at(1);
    tic->m_val=m_tics.at(2)->m_val-interval;
  }
  m_tics.sort(true);

}

int Scale::adaptShiftedTicArray(double direction,bool clearedge,int adaptotheredge) {

  int ii=0,ntics=m_tics.count();
  if (ntics<3 || direction==.0)
    return 0;
  Tic *tic=0;
  double interval=mk_round2(m_interval.m_d,m_interval.m_a);
  if (direction>.0) {
    if (clearedge) {
      for (ii=1;ii<ntics;ii++)
        m_tics.replace(ii-1,*m_tics.at(ii));
      m_tics.remove(ntics-1);
      ntics=m_tics.count();
    }
    if (adaptotheredge==1) {
      tic=m_tics.at(ntics-2);
      m_tics.replace(ntics-1,*tic);
      tic->m_val+=interval;
      m_tics.inSort(*tic);
      ntics=m_tics.count();
    }
    else if (adaptotheredge==-1) {
      m_tics.replace(ntics-2,*m_tics.at(ntics-1));
      m_tics.remove(ntics-1);
      ntics=m_tics.count();
    }
  }
  else {
    if (clearedge) {
      for (ii=ntics-1;ii>0;ii--)
        m_tics.replace(ii,*m_tics.at(ii-1));
      m_tics.remove(0);
      ntics=m_tics.count();
    }
    if (adaptotheredge==1) {
      tic=m_tics.at(ntics-1);
      tic->m_val+=interval;
      m_tics.inSort(*tic);
      ntics=m_tics.count();
      for (ii=ntics-1;ii>1;ii--)
        m_tics.replace(ii,*m_tics.at(ii-1));
      tic=m_tics.at(1);
      tic->m_val=m_tics.at(2)->m_val-interval;
      m_tics.replace(1,*tic);
    }
    else if (adaptotheredge==-1) {
      for (ii=1;ii<ntics-1;ii++)
        m_tics.replace(ii,*m_tics.at(ii+1));
      m_tics.remove(ntics-1);
      ntics=m_tics.count();
    }
  }
  m_tics.sort(true);
  return m_tics.count();

}

int Scale::failsave(aux::TVList<Tic> *ticL) {

  m_tics.clear();
  Range currrange=currRange();
  int cmp=0,db=mk_dbusted(currrange.m_min,currrange.m_max,&cmp);
  if (db!=0)
    setRange(.0,1.,currrange.m_option);
  else if (cmp==0) {
    currrange.m_min-=mk_ipow10(mk_mag(currrange.m_min)-mk_dprec);
    currrange.m_max+=mk_ipow10(mk_mag(currrange.m_max)-mk_dprec);
    setRange(currrange.m_min,currrange.m_max,currrange.m_option);
  }
  else if (0<cmp) {
    mk_swapf(&currrange.m_min,&currrange.m_max);
    setRange(currrange.m_min,currrange.m_max,currrange.m_option);
  }
  currrange=currRange();
  m_interval.m_d=calcInterval(11,0);
  m_interval.m_a=-(short)mk_mag(m_interval.m_d)+1;
  Tic tic(currrange.m_cmin,1,1);
  mk_d2a(currrange.m_cmin,tic.m_str,m_interval.m_a);
  m_tics.inSort(tic);
  tic=Tic((currrange.m_cmax+currrange.m_cmin)/2.,1,1);
  mk_d2a((currrange.m_cmax+currrange.m_cmin)/2.,tic.m_str,m_interval.m_a+1);
  m_tics.inSort(tic);
  tic=Tic(currrange.m_cmax,1,1);
  mk_d2a(currrange.m_cmax,tic.m_str,m_interval.m_a);
  m_tics.inSort(tic);
  return tics(ticL);

}

} // namespace simpleplot
