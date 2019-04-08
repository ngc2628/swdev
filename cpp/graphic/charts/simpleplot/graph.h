
#ifndef _graph_h_
#define _graph_h_
//..end "Ifdef"

#include <numeric/interpolation.h>
#include <osix/xxstyle.h>
#include <tools/typeid.h>
#include <graphic/shapes/polygon.h>

namespace simpleplot {

const char *const typeGraphXY="graphxy";
const int maxdatacnt=4096;
const int nummarkshapes2d=6;
const char * const markshapes2d[nummarkshapes2d]={
  "none","circle","square","triangle","diamond","unknown"
};

class oswinexp GraphData {

  public:
    GraphData() {
    }
    virtual ~GraphData() {
    }
    virtual int count() const=0;
    virtual int data(int,mk_vertex,int *avail=0)=0;
    virtual int data(mk_list *) {
      return 0;
    };
    virtual int setData(int,mk_vertex) {
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
    virtual int match(mk_vertex,mk_vertex) {
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
    mk_list m_data; 
    mk_list m_mark;
    int m_sortype;
    GraphData2(const GraphData2 &ass) : GraphData(ass) {
    }
    GraphData2 &operator=(const GraphData2 &){
      return *this;
    }

  public:
    GraphData2(int sz=0);
    virtual ~GraphData2();
    virtual int count() const {
      return m_data.count;
    }
    virtual int data(int,mk_vertex,int *avail=0);
    virtual int data(mk_list *);
    virtual int setData(int,mk_vertex);
    virtual int findBounds(int,double *,double *);
    virtual shapes::Shape2 *mark(int);
    virtual int setMark(int,shapes::Shape2 *);
    virtual int sortype() const {
      return m_sortype;
    }
    virtual int setSortype(int);
    virtual int match(mk_vertex,mk_vertex);
    
};

class oswinexp Graph : public mk::TypeId {
	
  public:
    GraphData *m_graphdata;
    osix::xxStyle m_linestyle;
    mk::TypeId *m_axis[3];
    mk_list m_scData;
    mk_list m_scDataI;                        
    mk_list m_scMark;
    Graph(const char *type=0,unsigned int idd=0);
		virtual ~Graph();
    bool operator==(const Graph &cmp) const {
      return ((const mk::TypeId*)this)->operator==((const mk::TypeId&)cmp);
    }
    bool operator<(const Graph &cmp) const {
      return ((const mk::TypeId*)this)->operator<((const mk::TypeId&)cmp);
    }

    virtual int findBounds(int,double *,double *) {
      return -1;
    }
    virtual int match(mk_vertex) const;
    virtual int value(int,mk_vertex vertex,int *avail=0) const {
      if (avail)
        *avail=0;
      return 1;
    }
    virtual int setValue(int,mk_vertex,int *modbounds=0) {
      if (modbounds)
        *modbounds=0;
      return -1;
    }
    virtual int rescale() {
      return 0;
    }
    virtual int sc2sz(mk_vertex) const;
    virtual int sz2sc(mk_vertex) const;
    
	protected:
    Graph(const Graph &) : mk::TypeId() {
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
    virtual int value(int,mk_vertex,int *avail=0) const;
    virtual int setValue(int,mk_vertex,int *modbounds=0);
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
