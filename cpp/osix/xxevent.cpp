
#include <osix/xxevent.h>
#include <mkbase/mkconv.h>

namespace osix {

xxtranslateEventF xxtranslateEventExt=0;

xxEvent &xxEvent::operator=(const xxEvent &ass) {
      
  m_type=ass.m_type;
  m_mods=ass.m_mods;
  m_consumer=ass.m_consumer;
  m_xxk=ass.m_xxk;
  m_buttons=ass.m_buttons;
  m_xxsym=ass.m_xxsym;
  if (&ass!=this) {
    m_pos=ass.m_pos;
    m_globalpos=ass.m_globalpos;
    m_lastpos=ass.m_lastpos;
    m_downpos=ass.m_downpos;
    m_r=ass.m_r;
    mk_stringset(m_info,&ass.m_info[0]);
  }
  return *this;
  
}
    
bool xxEvent::operator==(const xxEvent &cmp) const {

  return (m_type==cmp.m_type);

}

bool xxEvent::operator<(const xxEvent &cmp) const {

  return (m_type<cmp.m_type);

}

void xxEvent::clear() {
      
  m_type=m_mods=m_consumer=m_xxk=m_buttons=0;
	m_xxsym=0;
  m_pos=m_globalpos=m_lastpos=m_downpos=num::Vector3(mk_dnan,mk_dnan,mk_dnan);
  m_r=xxRect();
  mk_stringsetlength(m_info,0);
    
}

int xxEvent::setInfo(const char *info) {

  mk_stringset(m_info,info);
  return mk_stringlength(m_info);   

}

int xxEvent::info(mk_string info) const {

  mk_stringset(info,&m_info[0]);
  return mk_stringlength(info);

}
    
int xxEvent::type2txt(mk_string txt) const {

  if (m_type==xx_keyPressed)
    mk_stringset(txt,"key pressed");
  else if (m_type==xx_keyReleased)
    mk_stringset(txt,"key released");
  else if (m_type==xx_mousePressed)
    mk_stringset(txt,"mouse pressed");
  else if (m_type==xx_mouseReleased)
    mk_stringset(txt,"mouse released");
  else if (m_type==xx_mouseMove)
    mk_stringset(txt,"mouse moved");
  else if (m_type==xx_enter)
    mk_stringset(txt,"enter");
  else if (m_type==xx_leave)
    mk_stringset(txt,"leave");
  else if (m_type==xx_focusIn)
    mk_stringset(txt,"focus in");
  else if (m_type==xx_focusOut)
    mk_stringset(txt,"focus out");
  else if (m_type==xx_paint)
    mk_stringset(txt,"paint");
  else if (m_type==xx_show)
    mk_stringset(txt,"show");
  else if (m_type==xx_resize)
    mk_stringset(txt,"resize");
  else if (m_type==xx_mouseDblClicked)
    mk_stringset(txt,"mouse double clicked");
  else {
    mk_string numstr;
    mk_stringset(numstr,0);
    if ((m_type&osix::xx_unknownEvent)>0)
      mk_i2a(m_type&~osix::xx_unknownEvent,numstr);
    else
      mk_i2a(m_type,numstr);
    mk_stringset(txt,numstr);
  }  
  return mk_stringlength(txt);

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
