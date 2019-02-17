
#ifndef _scale_H_
#define _scale_H_

#include <auxx/auxx.h>
#include <numeric/vertex.h>

namespace simpleplot {

const int typeBoundAutoMin=1;
const int typeBoundAutoMax=2;
const int typeBoundStaticMin=4;
const int typeBoundStaticMax=8;
const int numscaletypes=2;
const char *const scaletypes[numscaletypes]={"none","linearscale"};
const int maxRangeStack=127;

class oswinexp Tic {

  public:
    double m_val;
    unsigned char m_size,m_drawable,m_ucs; 
    mk_string m_str; 
    Tic(double val=mk_dnan,unsigned char sz=1,unsigned char drawable=1,unsigned char ucs=0);
    Tic(const Tic &);
    ~Tic() { }
    Tic &operator=(const Tic &);
    bool operator==(const Tic &) const;
    bool operator<(const Tic &) const;
    int setUcsText(const aux::Ucsstr &);
    int ucsText(aux::Ucsstr *) const;

};

class oswinexp Range {

  public:
    double m_min,m_max,m_cmin,m_cmax;
    int m_option;
    Range(double min=.0,double max=1.,int option=(typeBoundAutoMin|typeBoundAutoMax)) : 
      m_min(min),m_max(max),m_cmin(min),m_cmax(max),m_option(option) {
    }
    Range(const Range &ass) : 
      m_min(ass.m_min),m_max(ass.m_max),m_cmin(ass.m_min),m_cmax(ass.m_cmax),m_option(ass.m_option) {
    }
    ~Range() {
    }
    Range &operator=(const Range &ass) {
      m_min=ass.m_min;
      m_max=ass.m_max;
      m_cmin=ass.m_min;
      m_cmax=ass.m_cmax;
      m_option=ass.m_option;
      return *this;
    }
    bool operator==(const Range &cmp) const {
      return (m_min==cmp.m_min && m_max==cmp.m_max);
    }
    bool operator<(const Range &cmp) const {
      return (m_min<cmp.m_min || (m_min==cmp.m_min && m_max<cmp.m_max));
    }
    
};

class oswinexp Scale : public aux::TypeId {	
	
  protected:
    mk_list m_range;
    aux::TVList<Tic> m_tics;
    aux::Rounded m_interval;
    virtual double calcInterval(int,int *);
    virtual void setMajTics();
		virtual double growIntervalSize (double);
		virtual void rollInnerTics (double);
		virtual int adaptShiftedTicArray (double,bool,int);
		virtual int failsave(aux::TVList<Tic> *ticL=0);
    virtual Range currRange() const;
           	
	public:
		Scale ();
		virtual ~Scale ();
    bool operator==(const Scale &cmp) const {
      return ((const aux::TypeId*)this)->operator==((const aux::TypeId&)cmp);
    }
    bool operator<(const Scale &cmp) const {
      return ((const aux::TypeId*)this)->operator<((const aux::TypeId&)cmp);
    }
    virtual int range(double *min=0,double *max=0,int *option=0) const;
    virtual int effRange(double *min=0,double *max=0,int *option=0) const;
    virtual double setRange(double,double,int option=-1);
    virtual double stackRange(double,double,int);
    virtual int unstackRange(int);
    virtual int rangeOption (int *mod=0);
    virtual int tics(aux::TVList<Tic> *ticL=0) const;
    virtual aux::Rounded interval() const;
    virtual void clear();
    virtual int calcTics (int,aux::TVList<Tic> *ticL=0);
    virtual int doShift (double);

	protected:
		Scale(const Scale &) : aux::TypeId() {
		}
		Scale &operator=(const Scale &) {
		  return *this;
		}
    
};

} // namespace simpleplot

#endif //_scale_H_

