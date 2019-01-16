
#ifndef _tgraph_h_
#define _tgraph_h_
//..end "Ifdef"

#include <auxx/auxx.h>
#include <numeric/vertex.h>
#include <numeric/interpolation.h>
#include <graphic/shapes/style.h>
#include <graphic/shapes/polygon.h>

namespace tableplot {

static const int nummarkshapes2d=7;
static const char * const markshapes2d[nummarkshapes2d]=
  {"none","circle","square","triangle","diamond","pixmap","unknown"};

class GraphData {

  public:
    GraphData() { }
    virtual ~GraphData() { }
    virtual int count() const=0;
    virtual num::Vector3 data(int,int *avail=0)=0;
    virtual int data(aux::TVList<num::Vector3> *) { return 0; };
    virtual int setData(int,num::Vector3*) { return -1; }
    virtual int findBounds(aux::MinMax *,aux::MinMax *)=0;
    virtual shapes::Shape2 *mark(int)=0;
    virtual int setMark(int,shapes::Shape2 *) { return -1; }
    virtual void setCmp(int (*fcmp)(const void*,const void*)) { }
    virtual int sortype() const { return -1; }
    virtual int setSortype(int) { return -1; }
    virtual int match(num::Vector3,num::Vector3) { return -1; }
    
  protected:
    GraphData(const GraphData &) { }
    GraphData &operator=(const GraphData &) { return *this; } 

};

extern shapes::Shape2 *buildMarkshape2(const char *,double);
extern int markshape2idx(const char *);
extern const char *idx2markshape2(int);

class GraphData2 : public GraphData {

  protected:
    aux::TVList<num::Vector3> m_data; 
    aux::TPArr<shapes::Shape2> m_mark;
    int m_sortype;

  public:
    GraphData2(int sz=0) : m_data(sz),m_mark(sz),m_sortype(-1) { }
    GraphData2(const GraphData2 &ass);
    virtual ~GraphData2() { m_mark.resize(0,true); }
    GraphData2 &operator=(const GraphData2 &ass);
    virtual int count() const { return m_data.count(); }
    virtual num::Vector3 data(int idx,int *avail=0);
    virtual int data(aux::TVList<num::Vector3> *);
    virtual int setData(int,num::Vector3*);
    virtual int findBounds(aux::MinMax *,aux::MinMax *);
    virtual shapes::Shape2 *mark(int);
    virtual  int setMark(int,shapes::Shape2 *);
    virtual int sortype() const { return m_sortype; }
    virtual int setSortype(int);
    virtual int match(num::Vector3,num::Vector3);
    
};

class Graph : public aux::TypeId {
	
  public:
    Graph(const char *type=0,unsigned int idd=0) : aux::TypeId(type,idd),m_graphdata(0),m_destr(0) { m_axis[0]=m_axis[1]=m_axis[2]=0; }
		virtual ~Graph() { if (m_graphdata) delete m_graphdata; }
    bool operator==(const Graph &cmp) const { return ((const aux::TypeId*)this)->operator==((const aux::TypeId&)cmp); }
    bool operator<(const Graph &cmp) const { return ((const aux::TypeId*)this)->operator<((const aux::TypeId&)cmp); }
    GraphData *m_graphdata;
    int m_destr;
    shapes::Style m_linestyle;
    TypeId *m_axis[3];
    virtual aux::MinMax findBounds(int) { return aux::MinMax(); }
    virtual int match(num::Vector3 p) const;
    virtual num::Vector3 value(int,int *avail=0) const { if (avail) *avail=0; return num::Vector3(); }
    virtual int setValue(int,num::Vector3*,int *modbounds=0) { return -1; }
    virtual int rescale() { return 0; }
    aux::TVList<num::Vector3> m_scData;
    aux::TPList<aux::TVList<num::Vector3> > m_scDataI;
    aux::TPArr<shapes::Shape2> m_scMark;
    virtual void sc2sz(num::Vector3 *) const;
    virtual void sz2sc(num::Vector3 *) const;
    
	protected:
    Graph(const Graph &ass) : aux::TypeId((const aux::TypeId&)ass),m_graphdata(0) { m_axis[0]=m_axis[1]=m_axis[2]=0; }
    Graph &operator=(const Graph &ass) { 
      if (this==&ass) return *this;
      ((aux::TypeId *)this)->operator=((const aux::TypeId&)ass); 
      return *this; 
    }
    
};

class GraphXY : public Graph {
    
  public:
    GraphXY(int m_destr=0);
    virtual ~GraphXY();
    bool operator==(const GraphXY &cmp) const { return ((const Graph*)this)->operator==((const Graph&)cmp); }
    bool operator<(const GraphXY &cmp) const { return ((const Graph*)this)->operator<((const Graph&)cmp); }
    num::Interpolation *m_interpolation;
    virtual aux::MinMax findBounds(int type);
    virtual num::Vector3 value(int,int *avail=0) const;
    virtual int setValue(int,num::Vector3 *,int *modbounds=0);
    int rescale();
    
  protected:
    GraphXY(const GraphXY &ass) : Graph((const Graph &)ass),m_interpolation(0) { }
    GraphXY &operator=(const GraphXY &) { return *this; } 

};

} // namespace 

#endif //_graph_h_
