
#include <QtGui/QMouseEvent>
#include <osix/xxevent.h>
#include <qt/util/qtutil.h>

namespace qtutil {

int translateQmouse(QMouseEvent *qm,osix::xxEvent *xm) {

  mk_vertex xmpos={(double)qm->pos().x(),(double)qm->pos().y(),.0,1.};
  mk_vertex xmglpos={(double)qm->globalX(),(double)qm->globalY(),.0,1.};
  mk_vertexcopy(xm->m_pos,xmpos);
  mk_vertexcopy(xm->m_globalpos,xmglpos);
  
  if (((int)qm->buttons()&(int)Qt::LeftButton)>0)
    xm->m_buttons|=osix::xxm_leftButton;
  if (((int)qm->buttons()&(int)Qt::MidButton)>0)
    xm->m_buttons|=osix::xxm_midButton;
  if (((int)qm->buttons()&(int)Qt::RightButton)>0)
    xm->m_buttons|=osix::xxm_rightButton;

  if (xm->m_type==osix::xx_mousePressed)
    mk_vertexcopy(xm->m_downpos,xm->m_pos);
  else if (xm->m_type==osix::xx_mouseReleased) {
    if (qm->button()==Qt::LeftButton)
      xm->m_buttons|=osix::xxm_leftButton;
    else if (qm->button()==Qt::MidButton)
      xm->m_buttons|=osix::xxm_midButton;
    else if (qm->button()==Qt::RightButton)
      xm->m_buttons|=osix::xxm_rightButton;
  }

  int qmods=(int)qm->modifiers();
  if ((qmods&(int)Qt::ControlModifier)>0)
    xm->m_mods|=osix::xx_modCtrl;
  if ((qmods&(int)Qt::ShiftModifier)>0)
    xm->m_mods|=osix::xx_modShift;
  if ((qmods&(int)Qt::AltModifier)>0)
    xm->m_mods|=osix::xx_modAlt;
  if ((qmods&(int)Qt::MetaModifier)>0)
    xm->m_mods|=osix::xx_modMeta;
  if ((qmods&(int)Qt::KeypadModifier)>0)
    xm->m_mods|=osix::xx_modKeypad;

  return 0;

}

}
