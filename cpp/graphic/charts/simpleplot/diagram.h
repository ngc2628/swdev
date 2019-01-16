
#ifndef _diagram_h_
#define _diagram_h_

#include <auxx/auxx.h>
#include <numeric/vertex.h>
#include <osix/xxevent.h>
#include <osix/xxshape.h>
#include <osix/xxpaint.h>
#include <graphic/charts/simpleplot/axis.h>
#include <graphic/charts/simpleplot/graph.h>
#include <stdio.h>
#include <stdlib.h>

namespace simpleplot {

const int mouseCoorTracking=1;
const int mouseCrosshair=2;
const int mouseZoom=4;
const int mouseModify=8;

const int redoScaleRect=1;
const int redoScaleGraph=2;
const int redoDrawAxes=4;
const int redoDrawGraph=8;
const int redoScr=16;

class oswinexp DiagramSelection {

  public:
    aux::TypeId m_sel;
    int m_idx;
    num::Vector3 m_val;
    DiagramSelection(aux::TypeId sel=aux::TypeId(),int idx=-1,num::Vector3 val=num::Vector3()) :
      m_sel(sel),m_idx(idx),m_val(val) {
    }
    DiagramSelection(const DiagramSelection &ass) : 
      m_sel(ass.m_sel),m_idx(ass.m_idx),m_val(ass.m_val) {
    }
    ~DiagramSelection() {
    }
    DiagramSelection &operator=(const DiagramSelection &ass) { 
      m_sel=ass.m_sel;
      m_idx=ass.m_idx;
      m_val=ass.m_val;
      return *this;
    }
    bool operator==(const DiagramSelection &cmp) const {
      return (m_sel==cmp.m_sel && m_idx==cmp.m_idx);
    }
    bool operator<(const DiagramSelection &cmp) const {
      return (m_sel<cmp.m_sel || (m_sel==cmp.m_sel && m_idx<cmp.m_idx));
    }
    void clear(aux::TypeId sel=aux::TypeId(),int idx=-1,num::Vector3 val=num::Vector3()) {
      m_sel=sel;
      m_idx=idx;
      m_val=val;
    }

};

class oswinexp Diagram : public aux::TypeId {
	
  protected:
    osix::xxDrawable m_winscr,m_pixscr,m_pixplot;
    osix::xxFnt m_font;
    osix::xxStyle m_bg;
    aux::TPList<aux::TypeId> m_corps;
    int m_mouseMode;
    osix::xxEvent m_xxlastinputev;
    aux::TVList<DiagramSelection> m_selected; 
            
  public:
    Diagram(const char *type,unsigned int idd=0);
	  virtual ~Diagram();
	  bool operator==(const Diagram &cmp) const {
	    return (*((aux::TypeId*)this)==((const aux::TypeId)cmp));
	  }
	  bool operator<(const Diagram &cmp) const {
	    return (*((aux::TypeId*)this)<((const aux::TypeId)cmp));
	  }
	  virtual Graph *graph(const aux::TypeId &);
	  virtual Axis *axis(const aux::TypeId &);
    virtual aux::TypeId setGraph(Graph *);
    virtual aux::TypeId setAxis(Axis *);
    virtual Axis *removeAxis(const aux::TypeId &);
    virtual Graph *removeGraph(const aux::TypeId &);
    virtual int clear(bool destr=false);
    virtual int typegraphs(const char *,aux::TPList<Graph> *);
    virtual int typeaxes(const char *,aux::TPList<Axis> *);
    virtual int selected(aux::TVList<DiagramSelection> *) const;
    virtual int setSelection(DiagramSelection,int add=0);
    virtual aux::TypeId selectGraph(num::Vector3,int set=1,int add=0); // scaled point
    virtual int mouseMode(int *mod=0);
    virtual int redraw(int flag=0);
        
  protected:
    Diagram(const Diagram &) : aux::TypeId() {
    }
    Diagram &operator=(const Diagram &) {
      return *this; 
    }
    virtual int inputEvent(osix::xxEvent *);
    virtual int mousePressed (osix::xxEvent *) {
      return 0;
    }
    virtual int mouseMoved (osix::xxEvent *) {
      return 0;
    }
    virtual int mouseReleased (osix::xxEvent *) {
      return 0;
    }
    virtual int mouseEnter(osix::xxEvent *) {
      return 0;
    }
    virtual int mouseLeave(osix::xxEvent *) {
      return 0;
    }
    virtual int resetRect() {
      return 0;
    }
    virtual int rescaleGraph() {
      return 0;
    }
    virtual int drawAxes() {
      return 0;
    }
    virtual int drawGraphs() {
      return 0;
    }
    virtual void *findDisplay() {
      return 0;
    }
    virtual int osEvent(void *,void *);
    virtual int osUpdate(osix::xxRect=osix::xxRect()) {
      return 0;
    }
    
};

class oswinexp DiagramXY : public Diagram {

  public:
    DiagramXY(int sz=1023);
    virtual ~DiagramXY() {
    }
    bool operator==(const DiagramXY &cmp) const {
      return (*((aux::TypeId*)this)==((const aux::TypeId)cmp));
    }
    bool operator<(const DiagramXY &cmp) const {
      return (*((aux::TypeId*)this)<((const aux::TypeId)cmp));
    }
    virtual aux::TypeId setGraph(Graph *graph) {
      return Diagram::setGraph(graph);
    }
    virtual aux::TypeId setAxis(Axis *axis) {
      return Diagram::setAxis(axis);
    }
    virtual osix::xxRectSize setup(osix::xxRectSize) {
      return osix::xxRectSize();
    }
    virtual int setSelection(DiagramSelection,int add=0);
    virtual aux::TypeId selectGraph(num::Vector3,int set=1,int add=0); // scaled point
    virtual osix::xxRectSize setup(osix::xxRectSize,osix::xxRectSize *excess=0);
    
  protected:
    DiagramXY(const DiagramXY &) : Diagram(0) {
    }
    DiagramXY &operator=(const DiagramXY &) {
      return *this;
    }
    virtual int resetRect();
    virtual int rescaleGraph();
    virtual int drawAxes();
    virtual int drawGraphs();
    virtual int redrawGraphs(void *);
    virtual int mousePressed (osix::xxEvent *);
    virtual int mouseMoved (osix::xxEvent *);
    virtual int mouseReleased (osix::xxEvent *);
    virtual int mouseEnter(osix::xxEvent *);
    virtual int mouseLeave(osix::xxEvent *);
    virtual int drawCrosshair(osix::xxEvent *);
    virtual int drawZoomRect(osix::xxEvent *);
  
};

} // namespace simpleplot

#endif //_diagram_h_
