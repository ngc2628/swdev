
#ifndef _diagram_h_
#define _diagram_h_

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
    mk::TypeId m_sel;
    int m_idx;
    mk_vertex m_val;
    DiagramSelection(mk::TypeId sel=mk::TypeId(),int idx=-1);
    DiagramSelection(const DiagramSelection &);
    ~DiagramSelection() {
    }
    DiagramSelection &operator=(const DiagramSelection &);
    void clear(mk::TypeId sel=mk::TypeId(),int idx=-1);

};

class oswinexp Diagram : public mk::TypeId {
	
  protected:
    osix::xxDrawable m_winscr,m_pixscr,m_pixplot;
    osix::xxFnt m_font;
    osix::xxStyle m_bg;
    mk_list m_entityL;
    int m_mouseMode;
    osix::xxEvent m_xxlastinputev;
    mk_list m_selected; 
            
  public:
    Diagram(const char *type,unsigned int idd=0);
	  virtual ~Diagram();
	  bool operator==(const Diagram &cmp) const {
	    return (*((mk::TypeId*)this)==((const mk::TypeId)cmp));
	  }
	  bool operator<(const Diagram &cmp) const {
	    return (*((mk::TypeId*)this)<((const mk::TypeId)cmp));
	  }
	  virtual Graph *graph(mk::TypeId);
	  virtual Axis *axis(mk::TypeId);
    virtual mk::TypeId setGraph(Graph *);
    virtual mk::TypeId setAxis(Axis *);
    virtual Axis *removeAxis(mk::TypeId);
    virtual Graph *removeGraph(mk::TypeId);
    virtual int entities(mk_list *,const char *);
    virtual int clear(bool destr=false);
    virtual int selected(mk_list *) const;
    virtual int setSelection(DiagramSelection,int add=0);
    virtual mk::TypeId selectGraph(mk_vertex,int set=1,int add=0); // scaled point
    virtual int mouseMode(int *mod=0);
    virtual int redraw(int flag=0);
        
  protected:
    Diagram(const Diagram &) : mk::TypeId() {
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
      return (*((mk::TypeId*)this)==((const mk::TypeId)cmp));
    }
    bool operator<(const DiagramXY &cmp) const {
      return (*((mk::TypeId*)this)<((const mk::TypeId)cmp));
    }
    virtual mk::TypeId setGraph(Graph *graph) {
      return Diagram::setGraph(graph);
    }
    virtual mk::TypeId setAxis(Axis *axis) {
      return Diagram::setAxis(axis);
    }
    virtual osix::xxRectSize setup(osix::xxRectSize) {
      return osix::xxRectSize();
    }
    virtual int setSelection(DiagramSelection,int add=0);
    virtual mk::TypeId selectGraph(mk_vertex,int set=1,int add=0); // scaled point
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
