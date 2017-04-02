
#ifndef _axis_h_
#define _axis_h_

#include <auxx/auxx.h>
#include <auxx/vertex.h>
#include <osix/xxstyle.h>
#include <osix/xxtxt.h>

namespace simpleplot {

const char *const typeXaxis="xaxis";
const char *const typeYaxis="yaxis";

const int typeLeft=1;
const int typeRight=2;
const int typeTop=4;
const  int typeBottom=8;

const int typeShowLine=1;
const int typeShowNotch=2;
const int typeShowText=4;

class Tic;
class Scale;
class Graph;

extern Scale* oswinexp buildScale(const char *type);

class oswinexp Axis : public aux::TypeId {
	
  protected:
    Scale *m_scale;
    int m_pos,m_visible;
    double m_autobounds[2];
    osix::xxRectSize m_size;
    aux::TPList<TypeId> m_graphs;
      
  public:
    osix::xxStyle m_style;
    osix::xxFnt m_fnt[2];
    Axis(const char *type=0,unsigned int idd=0);
		virtual ~Axis();
		bool operator==(const Axis &cmp) const {
		  return ((const aux::TypeId*)this)->operator==((const aux::TypeId&)cmp);
		}
    bool operator<(const Axis &cmp) const {
      return ((const aux::TypeId*)this)->operator<((const aux::TypeId&)cmp);
    }
    const char *carttype() const {
      return type();
    }
    virtual aux::TypeId assignGraph(Graph *);
    virtual int unassignGraph(const aux::TypeId &);
    virtual int assignedGraphs(aux::TPList<Graph> *);
	  virtual Graph *findGraph(const aux::TypeId &);
	  virtual void setScale(Scale *);
    virtual Scale *scale() {
      return m_scale;
    }
    virtual int setPos(int pos) {
      m_pos=(abs(pos)&(typeLeft|typeRight|typeTop|typeBottom));
      return m_pos;
    }
    virtual int pos() const {
      return m_pos;
    }
    virtual int findAutoBounds(int,double *,double *);
    virtual int checkAutoBounds(double,double);
    virtual osix::xxRectSize size() const {
      return m_size;
    }
    virtual double len() const {
      return sqrt(m_size.width()*m_size.width()+m_size.height()*m_size.height());
    }
    virtual double needExcess(osix::xxRectSize) {
      return 0.;
    }
    virtual double needSize(osix::xxRectSize) {
      return 0.;
    }
    virtual int resize(osix::xxRectSize) {
      return 0;
    }
    virtual int sc2sz(aux::Vector3 *) const {
      return 0;
    }
    virtual int sz2sc(aux::Vector3 *) const {
      return 0;
    }
             
  protected:
    Axis(const Axis &) : aux::TypeId() {
    }
    Axis &operator=(const Axis &) {
      return *this; 
    }
    virtual int calcTics(double,double,aux::TVList<Tic> *);
    virtual double ticLen4Size(const Tic *) {
      return 0.;
    }
    

};

class oswinexp Xaxis : public Axis {

  public:
    Xaxis() : Axis(typeXaxis) {
      m_pos=typeBottom;
    }
    virtual ~Xaxis() {
    }
    bool operator==(const Xaxis &cmp) const {
      return ((const Axis*)this)->operator==((const Axis&)cmp);
    }
    bool operator<(const Xaxis &cmp) const {
      return ((const Axis*)this)->operator<((const Axis&)cmp);
    }
    aux::TypeId assignGraph(Graph *);
    int setPos(int);
    int findAutoBounds(int,double *min,double *max) {
      return Axis::findAutoBounds(aux::typeX,min,max);
    }
    double needExcess(osix::xxRectSize);
    double needSize(osix::xxRectSize);
    int resize(osix::xxRectSize);
    double len() const {
      return m_size.width();
    }
    int sc2sz(aux::Vector3 *) const;
    int sz2sc(aux::Vector3 *) const;
        
  protected:
    Xaxis(const Xaxis &) : Axis() {
    }
    Xaxis &operator=(const Xaxis &) {
      return *this;
    }
    double ticLen4Size(const Tic *);
      
};

class oswinexp Yaxis : public Axis {

  public:
    Yaxis() : Axis(typeYaxis) {
      m_pos=typeLeft;
    }
    virtual ~Yaxis() {
    }
    bool operator==(const Yaxis &cmp) const {
      return ((const Axis*)this)->operator==((const Axis&)cmp);
    }
    bool operator<(const Yaxis &cmp) const {
      return ((const Axis*)this)->operator<((const Axis&)cmp);
    }
    aux::TypeId assignGraph(Graph *);
    int setPos(int);
    int findAutoBounds(int,double *min,double *max) {
      return Axis::findAutoBounds(aux::typeY,min,max);
    }
    double needExcess(osix::xxRectSize);
    double needSize(osix::xxRectSize);
    int resize(osix::xxRectSize);
    double len() const {
      return m_size.height();
    }
    int sc2sz(aux::Vector3 *) const;
    int sz2sc(aux::Vector3 *) const;
        
  protected:
    Yaxis(const Yaxis &) : Axis() {
    }
    Yaxis &operator=(const Yaxis &) {
      return *this;
    }
    double ticLen4Size(const Tic *);
        
};

} // namespace simpleplot

#endif //_axis_h_

