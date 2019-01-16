
#ifndef _graph_h_
#define _graph_h_
//..end "Ifdef"

#include <auxx/auxx.h>
#include <numeric/vertex.h>
#include <numeric/interpolation.h>
#include <osix/xxstyle.h>
#include <graphic/shapes/polygon.h>

namespace simpleplot {

const char *const typeGraphXY="graphxy";
const int maxdatacnt=4096;
const int nummarkshapes2d=6;
const char * const markshapes2d[nummarkshapes2d]={"none","circle","square","triangle","diamond","unknown"};

class oswinexp GraphData {

  public:
    GraphData() {
    }
    virtual ~GraphData() {
    }
    virtual int count() const=0;
    virtual num::Vector3 data(int,int *avail=0)=0;
    virtual int data(num::VertexList *) {
      return 0;
    };
    virtual int setData(int,num::Vector3*) {
      return -1;
    }
    virtual int findBounds(int,double *,double *)=0;
    virtual shapes::Shape2 *mark(int)=0;
    virtual int setMark(int,shapes::Shape2 *) {
      return -1;
    }
    virtual void setCmp(int (*)(const void*,const void*)) {
    }
    virtual int sortype() const {
      return -1;
    }
    virtual int setSortype(int) {
      return -1;
    }
    virtual int match(num::Vector3,num::Vector3) {
      return -1;
    }
    
  protected:
    GraphData(const GraphData &) {
    }
    GraphData &operator=(const GraphData &) {
      return *this;
    }

};

extern shapes::Shape2* oswinexp buildMarkshape2(const char *,double);
extern int oswinexp markshape2idx(const char *);
extern const char* oswinexp idx2markshape2(int);

class oswinexp GraphData2 : public GraphData {

  protected:
    num::VertexList m_data; 
    aux::TPArr<shapes::Shape2> m_mark;
    int m_sortype;

  public:
    GraphData2(int sz=0);
    GraphData2(const GraphData2 &ass);
    virtual ~GraphData2() {
      m_mark.resize(0,true);
    }
    GraphData2 &operator=(const GraphData2 &);
    virtual int count() const {
      return m_data.count();
    }
    virtual num::Vector3 data(int idx,int *avail=0);
    virtual int data(num::VertexList *);
    virtual int setData(int,num::Vector3*);
    virtual int findBounds(int,double *,double *);
    virtual shapes::Shape2 *mark(int);
    virtual  int setMark(int,shapes::Shape2 *);
    virtual int sortype() const {
      return m_sortype;
    }
    virtual int setSortype(int);
    virtual int match(num::Vector3,num::Vector3);
    
};

class oswinexp Graph : public aux::TypeId {
	
  public:
    GraphData *m_graphdata;
    osix::xxStyle m_linestyle;
    aux::TypeId *m_axis[3];
    num::VertexList m_scData;
    aux::TVList<num::VertexList> m_scDataI;
    aux::TPArr<shapes::Shape2> m_scMark;
    Graph(const char *type=0,unsigned int idd=0) : aux::TypeId(type,idd),m_graphdata(0) {
      m_axis[0]=m_axis[1]=m_axis[2]=0;
    }
		virtual ~Graph() {
		  if (m_graphdata)
		    delete m_graphdata;
		}
    bool operator==(const Graph &cmp) const {
      return ((const aux::TypeId*)this)->operator==((const aux::TypeId&)cmp);
    }
    bool operator<(const Graph &cmp) const {
      return ((const aux::TypeId*)this)->operator<((const aux::TypeId&)cmp);
    }

    virtual int findBounds(int,double *,double *) {
      return -1;
    }
    virtual int match(num::Vector3) const;
    virtual num::Vector3 value(int,int *avail=0) const {
      if (avail)
        *avail=0;
      return num::Vector3();
    }
    virtual int setValue(int,num::Vector3*,int *modbounds=0) {
      if (modbounds)
        *modbounds=0;
      return -1;
    }
    virtual int rescale() {
      return 0;
    }
    virtual int sc2sz(num::Vector3 *) const;
    virtual int sz2sc(num::Vector3 *) const;
    
	protected:
    Graph(const Graph &) : aux::TypeId() {
    }
    Graph &operator=(const Graph &) {
      return *this; 
    }
    
};

class oswinexp GraphXY : public Graph {
    
  public:
    num::Interpolation *m_interpolation;
    GraphXY();
    virtual ~GraphXY();
    bool operator==(const GraphXY &cmp) const {
      return ((const Graph*)this)->operator==((const Graph&)cmp);
    }
    bool operator<(const GraphXY &cmp) const {
      return ((const Graph*)this)->operator<((const Graph&)cmp);
    }
    virtual int findBounds(int,double *,double *);
    virtual num::Vector3 value(int,int *avail=0) const;
    virtual int setValue(int,num::Vector3 *,int *modbounds=0);
    int rescale();
    
  protected:
    GraphXY(const GraphXY &) : Graph() {
    }
    GraphXY &operator=(const GraphXY &) {
      return *this;
    }

};

} // namespace 

#endif //_graph_h_
