
#ifndef _axis_h_
#define _axis_h_

#include <auxx/auxx.h>
#include <graphic/shapes/style.h>
#include <graphic/shapes/rect.h>
#include <graphic/shapes/fnt.h>

namespace simpleplot {

class Tic;
class Scale;
class Graph;

extern Scale *buildScale(const char *type);

class Axis : public aux::TypeId {
	
  protected:
    Scale *m_scale;
    int m_pos; // 1:bottom, 2:top, 4:left, 8:right
    aux::MinMax m_autoBounds;
    shapes::RectSize m_size;
    aux::TPList<TypeId> m_graphs;
      
  public:
    int m_destr;
    int m_hasAutoBounds;
    int m_visible;
    shapes::Style m_style;
    shapes::Fnt m_fnt[2];
    Axis(const char *type=0,unsigned int idd=0);
		virtual ~Axis();
		bool operator==(const Axis &cmp) const { return ((const aux::TypeId*)this)->operator==((const aux::TypeId&)cmp); }
    bool operator<(const Axis &cmp) const { return ((const aux::TypeId*)this)->operator<((const aux::TypeId&)cmp); }
    const char *carttype() const { return type(); }
    virtual aux::TypeId assignGraph(Graph *);
    int unassignGraph(const aux::TypeId &);
    int assignedGraphs(aux::TPList<Graph> *);
		Graph *findGraph(const aux::TypeId &);
		void setScale(Scale *);
    Scale *scale() { return m_scale; }
    virtual int setPos(int) { return m_pos; }
    int pos() const { return m_pos; }
    aux::MinMax findAutoBounds(int); 
    virtual aux::MinMax findAutoBounds() { return aux::MinMax(0.,0.); }
    int checkAutoBounds(aux::MinMax);
    aux::MinMax autoBounds(int * hasAutoBounds=0) { if (hasAutoBounds) *hasAutoBounds=m_hasAutoBounds; return m_autoBounds; }
    shapes::RectSize size() const { return m_size; }
    virtual double len() const { return sqrt(m_size.width()*m_size.width()+m_size.height()*m_size.height()); }
    virtual double needExcess(shapes::RectSize) { return 0.; }
    virtual double needSize(shapes::RectSize) { return 0.; }
    virtual int resize(shapes::RectSize) { return 0; }   
    virtual void sc2sz(num::Vector3 *) const { }
    virtual void sz2sc(num::Vector3 *) const { }
             
  protected:
    Axis(const Axis &ass) : aux::TypeId((const aux::TypeId&)ass),
                            m_scale(0),m_pos(0),m_hasAutoBounds(1),m_visible(15),m_style(shapes::somecolors[shapes::black],1,1) { }
    Axis &operator=(const Axis &ass) {
      if (this==&ass) return *this; 
      ((aux::TypeId *)this)->operator=((const aux::TypeId&)ass); 
      return *this; 
    }
    virtual double ticLen4Size(const Tic *) { return 0.; }
    virtual int calcTics(double,double,aux::TPList<Tic> *);
    
};

class Xaxis : public Axis {

  public:
    Xaxis(int destr=0) : Axis("xaxis") { m_destr=destr; m_pos=1; m_graphs.resize(1023); }
    virtual ~Xaxis() { }
    bool operator==(const Xaxis &cmp) const { return ((const Axis*)this)->operator==((const Axis&)cmp); }
    bool operator<(const Xaxis &cmp) const { return ((const Axis*)this)->operator<((const Axis&)cmp); }
    aux::TypeId assignGraph(Graph *);
    int setPos(int pos) { if ((pos&1)>0 || (pos&2)>0) m_pos=pos; return m_pos; }
    aux::MinMax findAutoBounds() { return Axis::findAutoBounds(0); }
    double needExcess(shapes::RectSize);
    double needSize(shapes::RectSize);
    int resize(shapes::RectSize); 
    double len() const { return m_size.width(); }
    void sc2sz(num::Vector3 *) const;
    void sz2sc(num::Vector3 *) const;
        
  protected:
    Xaxis(const Xaxis &ass) : Axis((const Axis&)ass) { }
    Xaxis &operator=(const Xaxis &) { return *this; }
    double ticLen4Size(const Tic *);
      
};

class Yaxis : public Axis {

  public:
    Yaxis(int destr=0) : Axis("yaxis") { m_destr=destr; m_pos=4; m_graphs.resize(1023); }
    virtual ~Yaxis() { }
    bool operator==(const Yaxis &cmp) const { return ((const Axis*)this)->operator==((const Axis&)cmp); }
    bool operator<(const Yaxis &cmp) const { return ((const Axis*)this)->operator<((const Axis&)cmp); }
    aux::TypeId assignGraph(Graph *);
    int setPos(int pos) { if ((pos&4)>0 || (pos&8)>0) m_pos=pos; return m_pos; }
    aux::MinMax findAutoBounds() { return Axis::findAutoBounds(1); }
    double needExcess(shapes::RectSize);
    double needSize(shapes::RectSize);
    int resize(shapes::RectSize);
    double len() const { return m_size.height(); }
    void sc2sz(num::Vector3 *) const;
    void sz2sc(num::Vector3 *) const;
        
  protected:
    Yaxis(const Yaxis &ass) : Axis((const Axis&)ass) { }
    Yaxis &operator=(const Yaxis &) { return *this; } 
    double ticLen4Size(const Tic *);
        
};

} // namespace simpleplot

#endif //_axis_h_

