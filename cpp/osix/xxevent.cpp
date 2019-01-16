
#include <osix/xxevent.h>

namespace osix {

xxtranslateEventF xxtranslateEventExt=0;

void xxEvent::clear() {
      
  m_type=m_mods=m_consumer=m_xxk=m_buttons=0;
	m_xxsym=0;
  m_pos=m_globalpos=m_lastpos=m_downpos=num::Vector3(mk_dnan,mk_dnan,mk_dnan);
  m_r=xxRect();
  m_info.clear();
  
}

int xxEvent::setInfo(const char *info,int idx) {

  if (idx<0)
    return -1;
  aux::Asciistr strinfo(info);
  m_info.set(idx,strinfo);
  return idx;   

}

int xxEvent::info(aux::Asciistr *str,int idx) const {

  if (idx<0 || idx>m_info.last())
    idx=m_info.last();
  *str=m_info[idx];
  return idx;

}
    
void xxEvent::type2txt(aux::Asciistr *as) const {

  if (m_type==xx_keyPressed)
    *as="key pressed";
  else if (m_type==xx_keyReleased)
    *as="key released";
  else if (m_type==xx_mousePressed)
    *as="mouse pressed";
  else if (m_type==xx_mouseReleased)
    *as="mouse released";
  else if (m_type==xx_mouseMove)
    *as="mouse moved";
  else if (m_type==xx_enter)
    *as="enter";
  else if (m_type==xx_leave)
    *as="leave";
  else if (m_type==xx_focusIn)
    *as="focus in";
  else if (m_type==xx_focusOut)
    *as="focus out";
  else if (m_type==xx_paint)
    *as="paint";
  else if (m_type==xx_show)
    *as="show";
  else if (m_type==xx_resize)
    *as="resize";
  else if (m_type==xx_mouseDblClicked)
    *as="mouse double clicked";
  else if ((m_type&osix::xx_unknownEvent)>0)
    aux::i2a(m_type&~osix::xx_unknownEvent,as);
  else
    aux::i2a(m_type,as);  

}

int typewriterkey(xxEvent *xxev) {

  if (!xxev)
    return 0;
  if (xxev->m_xxk<xxk_space || xxev->m_xxk>xxk_ydiaeresis)
    return 0;
  if (xxev->m_xxk<=xxk_asciitilde) // ascii
    return xxk_ascii;
  if (xxev->m_xxk>=xxk_exclamdown) // latin1
    return xxk_latin1;
  return 0;

}

int navkey(xxEvent *xxev) {

  if (!xxev)
    return 0;
  if (xxev->m_xxk>=xxk_Left && xxev->m_xxk<=xxk_Down)
    return 1;
  if (xxev->m_xxk==xxk_Tab || xxev->m_xxk==xxk_BackTab)
    return 2;
  return 0;

}

int lowerkey(xxEvent *xxev) {

  if (!xxev)
    return 0;
  if (xxev->m_xxk>=xxk_A && xxev->m_xxk<=xxk_Z)
    return xxev->m_xxk+32;
  return xxev->m_xxk;

}

int xxtranslateEvent(void *dev,xxEvent *xxev) {

  if (xxtranslateEventExt)
    return xxtranslateEventExt(dev,xxev);
  if (xxev)
    xxev->clear();
  return 0;

}

int xxTypeMouse(xxEvent *xxev) {

  if (!xxev)
    return 0;
  return ((xxev->m_type>=xx_mousePressed && xxev->m_type<=xx_mouseMove) ||
           xxev->m_type==xx_mouseDblClicked ? 1 : 0);

}

int xxTypeKey(xxEvent *xxev) {

  if (!xxev)
    return 0;
  return (xxev->m_type>=xx_keyPressed && xxev->m_type<=xx_keyReleased ? 1 : 0);

}

}
