
#ifndef _diagram_h_
#define _diagram_h_

#include <stdio.h>
#include <stdlib.h>
#include <auxx/auxx.h>
#include <numeric/vertex.h>
#include <graphic/shapes/sshape.h>
#include <graphic/charts/simpleplot/axis.h>
#include <graphic/charts/simpleplot/graph.h>

namespace simpleplot {

class DiagramSelection {

  public:
    aux::TypeId m_sel;
    int m_idx;
    num::Vector3 m_val;
    DiagramSelection(aux::TypeId sel=aux::TypeId(),int idx=-1,num::Vector3 val=num::Vector3()) :
      m_sel(sel),m_idx(idx),m_val(val) { }
    DiagramSelection(const DiagramSelection &ass) : m_sel(ass.m_sel),m_idx(ass.m_idx),m_val(ass.m_val) { }
    ~DiagramSelection() { }
    DiagramSelection &operator=(const DiagramSelection &ass) { 
      m_sel=ass.m_sel; m_idx=ass.m_idx; m_val=ass.m_val; return *this;
    }
    bool operator==(const DiagramSelection &cmp) const { return (m_sel==cmp.m_sel && m_idx==cmp.m_idx); }
    bool operator<(const DiagramSelection &cmp) const { return (m_sel<cmp.m_sel || (m_sel==cmp.m_sel && m_idx<cmp.m_idx)); }
    void set(aux::TypeId sel=aux::TypeId(),int idx=-1,num::Vector3 val=num::Vector3()) {
      m_sel=sel; m_idx=idx; m_val=val; 
    }

};

class Diagram : public aux::TypeId {
	
  protected:
    aux::TPList<aux::TypeId> m_corps;
    int m_mouseMode; // 0: none, 1:crosshair, 2:zoom, 4:modify
    aux::TVList<DiagramSelection> m_selected; 
            
	public:
    Diagram(const char *type,unsigned int idd=0);
		virtual ~Diagram();
    Graph *graph(const aux::TypeId &);
		Axis *axis(const aux::TypeId &);
    int typegraphs(const char *,aux::TPList<Graph> *);
    int typeaxes(const char *,aux::TPList<Axis> *);
		Axis *removeAxis(const aux::TypeId &);
		Graph *removeGraph(const aux::TypeId &);
    virtual aux::TypeId setGraph(Graph *);
    virtual aux::TypeId setAxis(Axis *);
    virtual shapes::RectSize setup(shapes::RectSize) { return shapes::RectSize(); }
    int mouseMode() const { return m_mouseMode; }
    virtual int setMouseMode(int,bool combine=false);
    int selected(aux::TVList<DiagramSelection> *) const; 
    virtual int setSelection(DiagramSelection,bool add=false) { return -1; } 
    virtual aux::TypeId selectGraph(num::Vector3,bool set=true,bool add=false) { return aux::TypeId(); } // scaled point
        
  protected:
    Diagram(const Diagram &ass) : aux::TypeId((const aux::TypeId&)ass) { }
    Diagram &operator=(const Diagram &ass) {
      if (this==&ass) return *this; 
      ((aux::TypeId *)this)->operator=((const aux::TypeId&)ass); 
      return *this; 
    }
    
};

class DiagramXY : public Diagram {

  public:
    DiagramXY(int sz=1023);
    virtual ~DiagramXY() { }
    virtual aux::TypeId setGraph(Graph *graph) { return Diagram::setGraph(graph); }
    virtual aux::TypeId setAxis(Axis *axis) { return Diagram::setAxis(axis); }
    int setSelection(DiagramSelection,bool add=false);
    virtual aux::TypeId selectGraph(num::Vector3,bool set=true,bool add=false); // scaled point
    virtual shapes::RectSize setup(shapes::RectSize,shapes::RectSize *excess=0);
    
  protected:
    DiagramXY(const DiagramXY &ass) : Diagram((const Diagram&)ass) { }
    DiagramXY &operator=(const DiagramXY &) { return *this; }
  
};

} // namespace simpleplot

#endif //_diagram_h_
