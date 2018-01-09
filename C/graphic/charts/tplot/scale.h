
#ifndef _scale_H_
#define _scale_H_

#include <auxx/auxx.h>
#include <auxx/dbl.h>
#include <auxx/vertex.h>

namespace tableplot {

static const int numscaletypes=2;
static const char *const scaletypes[numscaletypes]={"none","linearscale"};

class Tic {

  public:
    aux::Dbl m_val;
    int m_size;
    int m_drawable;
    Tic(aux::Dbl val=.0) : m_val(val),m_size(0),m_drawable(1) { }
    Tic(const Tic &ass) : m_val(ass.m_val),m_size(0),m_drawable(1) { }
    ~Tic() { }
    Tic &operator=(const Tic &ass) { m_val=ass.m_val; m_size=0; m_drawable=1; return *this; }
    Tic &operator=(aux::Dbl val) { m_val=val; m_size=0; m_drawable=1; return *this; }
    bool operator==(const Tic &cmp) const { return (m_val==cmp.m_val); }
    bool operator<(const Tic &cmp) const { return (m_val<cmp.m_val); }
      
};

class Scale : public aux::TypeId {	
	
  protected:
		aux::Dbl m_interval;
    int m_staticBounds;
    aux::MinMax m_bounds;
    aux::MinMax m_calcedBounds;
    aux::TPList<Tic> m_tics;
		virtual double growIntervalSize (double);
    int failsave(aux::TPList<Tic> *ticL=0);
		void rollInnerTics (double);
		int adaptShiftedTicArray (double dir,bool,int);
		virtual void setMajTics();
		Scale(const Scale &ass) : aux::TypeId((const aux::TypeId&)ass) { }
    Scale &operator=(const Scale &ass) { ((aux::TypeId*)this)->operator=((const aux::TypeId &)ass); return *this; } 
           	
	public:
		Scale ();
		virtual ~Scale ();
    bool operator==(const Scale &cmp) const { return ((const aux::TypeId*)this)->operator==((const aux::TypeId&)cmp); }
    bool operator<(const Scale &cmp) const { return ((const aux::TypeId*)this)->operator<((const aux::TypeId&)cmp); }
    int tics(aux::TPList<Tic> *ticL=0);
    void clear();
    aux::MinMax bounds() { return m_bounds; }
    aux::MinMax calcedBounds() { return m_calcedBounds; }
    aux::MinMax effBounds() {
      if (m_tics.count()>1) return aux::MinMax((double)m_tics[0]->m_val,(double)m_tics[m_tics.count()-1]->m_val);
      return m_bounds;
    }
    aux::MinMax setBounds(aux::MinMax bounds);
    int doShift (double);
    virtual double calcInterval(int,int *);
		virtual int calcTics (int,aux::TPList<Tic> *ticL=0);
		virtual int keepBounds (int set) {
      if (set>=0) m_staticBounds=set&3;
      return m_staticBounds;
    }
    
};



} // namespace simpleplot

#endif //_scale_H_

