
#ifndef _scale_H_
#define _scale_H_

#include <mkbase/mkutil.h>
#include <tools/char.h>
#include <tools/misc.h>
#include <tools/typeid.h>

namespace simpleplot {

const int typeBoundAutoMin=1;
const int typeBoundAutoMax=2;
const int typeBoundStaticMin=4;
const int typeBoundStaticMax=8;
const int numscaletypes=2;
const char *const scaletypes[numscaletypes]={"none","linearscale"};
const int maxRangeStack=127;

class oswinexp Tic {

  mk_string m_str; 
  public:
    double m_val;
    unsigned char m_size,m_drawable,m_ucs; 
    Tic(double val=mk_dnan,unsigned char sz=1,unsigned char drawable=1,unsigned char ucs=0);
    Tic(const Tic &);
    ~Tic() { }
    Tic &operator=(const Tic &);
    int text(mk_string) const;
    int ucsText(mk::Ucsstr *) const;
    int setText(mk_string);
    int setUcsText(const mk::Ucsstr &);
    
};

extern "C" int oswinexp cmpTic(const void *,const void *);

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
    
};

class oswinexp Scale : public mk::TypeId {	
	
  protected:
    mk_list m_range;
    mk_list m_tics;
    mk::Rounded m_interval;
    virtual double calcInterval(int,int *);
    virtual void setMajTics();
		virtual double growIntervalSize (double);
		virtual void rollInnerTics (double);
		virtual int adaptShiftedTicArray (double,bool,int);
		virtual int failsave(mk_list *ticL=0);
    virtual Range currRange() const;
           	
	public:
		Scale ();
		virtual ~Scale ();
    bool operator==(const Scale &cmp) const {
      return ((const mk::TypeId*)this)->operator==((const mk::TypeId&)cmp);
    }
    bool operator<(const Scale &cmp) const {
      return ((const mk::TypeId*)this)->operator<((const mk::TypeId&)cmp);
    }
    virtual int range(double *min=0,double *max=0,int *option=0) const;
    virtual int effRange(double *min=0,double *max=0,int *option=0) const;
    virtual double setRange(double,double,int option=-1);
    virtual double stackRange(double,double,int);
    virtual int unstackRange(int);
    virtual int rangeOption (int *mod=0);
    virtual int tics(mk_list *ticL=0) const;
    virtual mk::Rounded interval() const;
    virtual void clear();
    virtual int calcTics (int,mk_list *ticL=0);
    virtual int doShift (double);

	protected:
		Scale(const Scale &) : mk::TypeId() {
		}
		Scale &operator=(const Scale &) {
		  return *this;
		}
    
};

} // namespace simpleplot

#endif //_scale_H_

