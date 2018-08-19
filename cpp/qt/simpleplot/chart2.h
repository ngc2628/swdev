
#ifndef _chart2_h_
#define _chart2_h_

#include <QtCore/QObject>
#include <QtCore/QEvent>
#include <QtCore/QRect>
#include <QtWidgets/QApplication>
#include <QtWidgets/QFrame>
#include <QtWidgets/QLayout>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QScrollBar>
#include <QtGui/QPainter>
#include <QtGui/QResizeEvent>
#include <stdio.h>
#include <stdlib.h>
#include <auxx/auxx.h>
#include <osix/xxshape.h>
#include <graphic/charts/simpleplot/diagram.h>
#include <qt/util/qtutil.h>
#include <qt/util/qtwidgets.h>

class QFont;

namespace simpleplot {
  class Axis;
}

namespace qtsimpleplot {

class Controls;
class QtDiagramXY;

class oswinexp PlotView : public QWidget {

  Q_OBJECT

  friend class QtDiagramXY;

  public:
    QtDiagramXY *m_diagram;
    PlotView(QtDiagramXY *, const char *);
    virtual ~PlotView();
    bool operator==(const PlotView &cmp) const {
      return (m_diagram==cmp.m_diagram);
    }
    bool operator<(const PlotView &cmp) const {
      return (m_diagram<cmp.m_diagram);
    }
    bool event (QEvent *);
    bool eventFilter(QObject*,QEvent*);

  protected:
    PlotView(const PlotView &) : QWidget() {
    }
    PlotView &operator=(const PlotView &) {
      return *this;
    }

};

class oswinexp QtDiagramXY : public QFrame,public simpleplot::DiagramXY {

  Q_OBJECT

  friend class PlotView;

  public: 
    QtDiagramXY(QWidget *,int sz=1023);
    virtual ~QtDiagramXY();
    void *findDisplay();
    int osUpdate(osix::xxRect);
    TypeId setAxis(simpleplot::Axis *);
            
  protected:
    QGridLayout *m_layout;
    PlotView *m_view;
    QCheckBox *m_modX;
    QCheckBox *m_modZ;
    QCheckBox *m_modM;
    QCheckBox *m_showcontrols;
    Controls *m_controlsDialog;
    QtDiagramXY(const QtDiagramXY &) : QFrame(),simpleplot::DiagramXY() {
    }
    QtDiagramXY &operator=(const QtDiagramXY &) {
      return *this;
    }
  
  protected slots:
    void slotCheckX(int);
    void slotCheckZ(int);
    void slotCheckM(int);
    void slotControls(int);
    void slotControlsDismiss(); 
          
};

} // namespace

#endif //_chart2_h_
