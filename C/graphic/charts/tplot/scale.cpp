
#include <stdio.h>
#include <math.h>
#include <sys/param.h>
#include <graphic/charts/simpleplot/scale.h>

using namespace aux;

namespace simpleplot {

Scale::Scale() : TypeId("linearscale"),m_interval(0.),m_staticBounds(0) {

  m_tics.resize(1024);
  setBounds(aux::MinMax(0.,0.));
  
}

Scale::~Scale() {
  
}

void Scale::clear() {

  //printf ("ticsz=%d ticcnt=%d\n",m_tics.size(),m_tics.count());
  m_tics.clear(true);

}

aux::MinMax Scale::setBounds(aux::MinMax bounds) {

  if (bounds.busted(3)) {
    if (finite(bounds[0])) bounds[1]=bounds[0];
    else if (finite(bounds[1])) bounds[0]=bounds[1];
    else bounds[0]=bounds[1]=.0;
  }
  if (bounds[0]>bounds[1]) swap(&bounds[0],&bounds[1]);
  if (m_bounds==bounds) return m_calcedBounds;
  m_bounds=bounds;
  aux::MinMax b=m_bounds;
  double sgn0=dsign(b[0]),sgn1=dsign(b[1]);
  b[0]=fabs(b[0]);
  b[1]=fabs(b[1]);
  bool sgns=(b[0]>b[1]);
  if (sgns) swap(&b[0],&b[1]);
  int magmax=mag(b[1]),magmin=mag(b[0]),rdprec=-magmax;
  double valdiff=.0;
  bool eq=deq(b[0],b[1],ipow10(MIN(magmax,magmin)),&valdiff);  
//printf ("setbounds %f,%f sgn0=%f sgn1=%f b0=%f b1=%f\n",m_bounds[0],m_bounds[1],sgn0,sgn1,b[0],b[1]);
  if (abs(magmax-magmin)>1 || (eq && sgn0!=sgn1)) {
    b[1]=roundUp(b[1],rdprec);
    if (sgn0==sgn1) b[0]=.0;
    else b[0]=b[1];  
  }
  else if (eq && sgn0==sgn1) {
    b[0]-=ipow10(-rdprec);
	  b[1]+=ipow10(-rdprec);  
  }
  else {
    rdprec=-mag(valdiff);
    b[0]=roundDown(b[0],rdprec);
    b[1]=roundUp(b[1],rdprec);
  }

  if (sgns) {
    swap(&b[0],&b[1]);
    sgn1=dsign(b[1]);
  }
  b[0]*=sgn0;
  b[1]*=sgn1;
//printf ("calcedbounds %f,%f\n",b[0],b[1]); 
  
  m_calcedBounds=b;
  m_interval=Dbl(b[1]-b[0],-mag(b[1]-b[0])+1); 
      
  return m_calcedBounds;

}

int Scale::failsave(TPList<Tic> *ticL) {

  m_tics.clear(true);
  if (ticL) ticL->clear();
  double t1=(m_staticBounds&2 ? m_bounds[1] : m_calcedBounds[1]),
         t0=(m_staticBounds&1 ? m_bounds[0] : m_calcedBounds[0]);
  int prec=-mag(t1-t0)+1;
  m_interval=Dbl(t1-t0,prec);
  m_tics.inSort(new Tic(Dbl(t0,prec)));
  m_tics.inSort(new Tic(Dbl(t1,prec)));
  
  return tics(ticL);

}

double Scale::growIntervalSize(double size) {

  double lggrow=log10(2.),tmp=0.,lower=log10(size),higher=lower+lggrow;
  if (((int)higher>(int)lower) || (dsign(higher)!=dsign(lower))) {
    tmp=ipow10((int)floor(higher));
    if (tmp>size) return tmp;
  }
	// in case we want an interval like 2.5 rather than 4
	if (((int)round2(higher)>(int)round2(lower)) || (dsign(higher)!=dsign(lower))) {
		tmp=ipow10((int)round2(higher))/4.0;
    if (tmp>size) return tmp;
	}
	//
  lower+=lggrow;
  higher+=lggrow;
  if (((int)higher>(int)lower) || (dsign(higher)!=dsign(lower))) {
    tmp=ipow10((int)floor(higher))/2.0;
    if (tmp>size) return tmp;
  }

  return 2.0*size;

}

double Scale::calcInterval(int maxtics,int *ntics) {

  double bdiff=m_calcedBounds[1]-m_calcedBounds[0];
  if (bdiff==.0) {
    setBounds(m_bounds);
    bdiff=m_calcedBounds[1]-m_calcedBounds[0];
  }
  double interval=ipow10(mag(bdiff)-2);
  int nntics=roundUp(bdiff/interval,0);
  while (nntics<maxtics) {
    interval/=10.;
    nntics=roundUp(bdiff/interval,0)+1;
  }
  while (maxtics>2 && nntics>maxtics) {
    interval=growIntervalSize(interval);
    nntics=roundUp(bdiff/interval,0)+1;
  }
  int prec=-mag(interval);
  if (round2(interval,prec)>interval) prec++; // 2.5 intv
  m_interval=Dbl(interval,prec);
  if (ntics) *ntics=nntics;
  
  return (double)m_interval;

}

int Scale::calcTics(int maxtics,TPList<Tic> *ticL) {

  m_tics.clear(true);
  if (ticL) ticL->clear();
  if (maxtics<=2) return failsave(ticL);
  int ntics=0;
  calcInterval(maxtics,&ntics);
  double pos=m_calcedBounds[0];
  int i=0,prec=m_interval.prec(); 
  for (i=0;i<ntics;i++) {
    m_tics.inSort(new Tic(Dbl(pos,prec)));
    if (pos>=m_calcedBounds[1]) break;
    pos=round2(pos+m_interval.raw(),prec);
  }
  ntics=m_tics.count();
  if (ntics<=2) return failsave(ticL);
  if (m_staticBounds&1) {
    Tic *tb0=new Tic(Dbl(m_bounds[0],prec));
    int idxl=m_tics.findNextIndex(tb0);
    for (i=0;i<idxl;i++) m_tics.remove(i);
    m_tics.inSort(tb0);
    ntics=m_tics.count();
  }
  if (m_staticBounds&2) {
    Tic *tb1=new Tic(Dbl(m_bounds[1],prec));
    int idxh=m_tics.findNextIndex(tb1);
    for (i=idxh;i<ntics;i++) m_tics.remove(m_tics.count()-1);
    m_tics.inSort(tb1);
    ntics=m_tics.count();
  }
  if (ntics<=2) return failsave(ticL);

  setMajTics();
  return tics(ticL);

}

int Scale::tics(TPList<Tic> *ticL) {
      
  int i=0,cnt=m_tics.count();
  if (ticL) {
    if (ticL->size()<cnt) ticL->resize(cnt);
    ticL->clear();
    for (i=0;i<cnt;i++) ticL->inSort(m_tics[i]);
  }
  return cnt;
  
}

void Scale::setMajTics () {

  int i=0,ntics=m_tics.count();
  if (ntics<=0) return;
  for (i=0;i<ntics;i++) m_tics[i]->m_size=0;
  double val=.0,interval=(double)m_interval;
  int idx=-1,magintv=mag(interval),iinterval=(int)round2(10.*interval/ipow10(magintv));
  for (i=0;i<ntics;i++) {
    val=fabs((double)(m_tics[i]->m_val));  
    if (deq(val,.0,ipow10(magintv-1))) {
      m_tics[i]->m_size=1;
      continue;
    }
    if (deq(roundUp(val,-magintv-1),roundDown(val,-magintv-1),ipow10(magintv))) {
      idx=i;
      break;
    }
    if (iinterval==10 && deq(roundUp(2.*val,-magintv-1),roundDown(2.*val,-magintv-1),ipow10(magintv))) {
      idx=i;
      break;
    }
  }
  if (idx>=0) {
    m_tics[idx]->m_size=1;
    if (iinterval==50 && (idx-2>0 || (idx-2==0 && (m_staticBounds&1)==0))) m_tics[idx-2]->m_size=1;
    while (idx<ntics) {
      if (iinterval==10 || iinterval==20) idx+=5;
      else if (iinterval==25) idx+=4;
      else if (iinterval==50) idx+=2;
      if (idx<ntics-1 || (idx==ntics-1 && (m_staticBounds&2)==0)) m_tics[idx]->m_size=1;
    }
  }
  
}

int Scale::doShift(double amount) {

  int ntics=m_tics.count();
  if (ntics<2) return 0;
  double sam=amount,interval=(double)m_interval;
  // do not rely on highest resolution for the calculation of differences but
  // use a tenth either of the interval size or the shift amount (whatever number is smaller)
  double eps=ipow10(mag(((fabs(sam)>interval) ? interval : sam))-2);
  // do not do nothing when there is nothing to be done
  if (deq(sam,0.0,eps)) return m_tics.count();
  double direction=dsign(sam);
  // acoording to shift direction higher/lower end must grow/shrink
  int adaptotheredge=0;
  m_tics[0]->m_val.setRaw(m_tics[0]->m_val.raw()+sam);
  m_tics[ntics-1]->m_val.setRaw(m_tics[ntics-1]->m_val.raw()+sam);
  double diffleft=0.0,diffright=0.0;
  bool matchl=deq(m_tics[0]->m_val.raw(),m_tics[1]->m_val.raw(),eps,&diffleft),
       matchr=deq(m_tics[ntics-1]->m_val.raw(),m_tics[ntics-2]->m_val.raw(),eps,&diffright);
	// shift to the left
  if (direction>0.0)	{
    // loop as long as the ticmark array does not match the new spanning
    while (diffleft>=0.0 && !matchl) {
      // shift ticmark array positions to the left clear the first one (not needed anymore)
      // and generate the suiting new one at the right edge
      rollInnerTics(direction);
      matchl=deq(m_tics[0]->m_val.raw(),m_tics[1]->m_val.raw(),eps,&diffleft);
      matchr=deq(m_tics[ntics-1]->m_val.raw(),m_tics[ntics-2]->m_val.raw(),eps,&diffright);
    }
    if (!matchr) {
      if (diff(diffright,interval,eps)>0.0) adaptotheredge=1;
      if (diffright<0.0) adaptotheredge=-1;
    }
    ntics=adaptShiftedTicArray(direction,matchl,adaptotheredge);
  }
  // shift to the right
  else if (direction<0.0) {
    while (diffright<=0.0 && !matchr) {
      rollInnerTics(direction);
      matchl=deq(m_tics[0]->m_val.raw(),m_tics[1]->m_val.raw(),eps,&diffleft);
      matchr=deq(m_tics[ntics-1]->m_val.raw(),m_tics[ntics-2]->m_val.raw(),eps,&diffright);
    }
    if (!matchl) {
      if (diff(fabs(diffleft),interval,eps)>0.0) adaptotheredge=1;
      if (diffleft>0.0) adaptotheredge=-1;
    }
    ntics=adaptShiftedTicArray(direction,matchr,adaptotheredge);
  }
  
  // shifted positions should not be displayed because they usually have no correspondence with the interval size
  //if (CNumUtl::diff(fabs(CNumUtl::diff(m_tarr[0],m_tarr[1])),m_intervalSize)!=0.0) {
    
  if (!deq(m_tics[1]->m_val.raw()-m_tics[0]->m_val.raw(),m_interval,eps)) m_tics[0]->m_drawable=0;
  if (!deq(m_tics[ntics-1]->m_val.raw()-m_tics[ntics-2]->m_val.raw(),interval,eps)) m_tics[ntics-1]->m_drawable=0;

	setMajTics();
  return ntics;

}

void Scale::rollInnerTics(double direction) {

  int i=0,ntics=m_tics.count();
  if (ntics<3) return;
  if (direction>0.0) {
    for (i=2;i<(ntics-2);i++) *(m_tics[i-1])=*(m_tics[i]);
    m_tics[ntics-2]->m_val.setRaw((double)m_tics[ntics-3]->m_val+(double)m_interval);
  }
  if (direction<0.0) {
    for (i=(ntics-2);i>1;i--) *(m_tics[i])=*(m_tics[i-1]);
    m_tics[1]->m_val.setRaw((double)m_tics[2]->m_val-(double)m_interval);
  }
  m_tics.sort(true);

}

int Scale::adaptShiftedTicArray(double direction,bool clearedge,int adaptotheredge) {

  int i=0,ntics=m_tics.count(),prec=m_interval.prec();
  if (ntics<3 || direction==.0) return 0;
  if (direction>0.0) {
    if (clearedge) {
      for (i=1;i<ntics;i++) *(m_tics[i-1])=*(m_tics[i]);
      m_tics.remove(ntics-1);
      ntics=m_tics.count();
    }
    if (adaptotheredge==1) {
      *(m_tics[ntics-1])=*(m_tics[ntics-2]);
      m_tics.inSort(new Tic(Dbl((double)m_tics[ntics-2]->m_val+(double)m_interval,prec)));
      ntics=m_tics.count();
    }
    else if (adaptotheredge==-1) {
      *(m_tics[ntics-2])=*(m_tics[ntics-1]);
      m_tics.remove(ntics-1);
      ntics=m_tics.count();
    }
  }
  else {
    if (clearedge) {
      for (i=ntics-1;i>0;i--) *(m_tics[i])=*(m_tics[i-1]);
      m_tics.remove(0);
      ntics=m_tics.count();
    }
    if (adaptotheredge==1) {
      m_tics.inSort(new Tic(Dbl((double)m_tics[ntics-1]->m_val+(double)m_interval,prec)));
      ntics=m_tics.count();
      for (i=ntics-1;i>1;i--) *(m_tics[i])=*(m_tics[i-1]);
      m_tics[1]->m_val.setRaw((double)m_tics[2]->m_val-(double)m_interval);
    }
    else if (adaptotheredge==-1) {
      for (i=1;i<ntics-1;i++) *(m_tics[i])=*(m_tics[i+1]);
      m_tics.remove(ntics-1);
      ntics=m_tics.count();
    }
  }
  m_tics.sort(true);
  return m_tics.count();

}

} // namespace simpleplot
