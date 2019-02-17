
#ifndef _xxevent_h_
#define _xxevent_h_

#include <mkbase/defs.h>
#include <osix/xxkey.h>
#include <osix/xxmouse.h>
#include <osix/xxshape.h>

namespace osix {

const int xx_keyPressed=2;
const int xx_keyReleased=3;
const int xx_mousePressed=4;
const int xx_mouseReleased=5;
const int xx_mouseMove=6;
const int xx_enter=7;
const int xx_leave=8;
const int xx_focusIn=9;
const int xx_focusOut=10;
const int xx_paint=12;
const int xx_show=15;
const int xx_resize=25;
const int xx_mouseDblClicked=38;
const int xx_unknownEvent=1073741824;

const int xx_modCtrl=1048576; //#21
const int xx_modShift=2097152; //#22
const int xx_modAlt=4194304; //#23
const int xx_modMeta=8388608; //#24
const int xx_modGroupSwitch=16777216; //#25
const int xx_modKeypad=33554432; //#26

const int xx_ignored=0;
const int xx_insignificant=1;
const int xx_processed=2;
const int xx_consumed=4;

class oswinexp xxEvent {

  public:
    int m_type,m_mods,m_consumer,m_xxk,m_buttons;
    unsigned short m_xxsym;
    num::Vector3 m_pos,m_globalpos,m_lastpos,m_downpos;
    xxRect m_r;
    mk_string m_info;
    xxEvent() : 
      m_type(0),m_mods(0),m_consumer(0),m_xxk(0),m_buttons(0),m_xxsym(0) {
      mk_stringset(m_info,0);
    }
    xxEvent(const xxEvent &ass) :
      m_type(ass.m_type),m_mods(ass.m_mods),m_consumer(ass.m_consumer),
      m_xxk(ass.m_xxk),m_buttons(ass.m_buttons),m_xxsym(ass.m_xxsym),
      m_pos(ass.m_pos),m_globalpos(ass.m_globalpos),m_lastpos(ass.m_lastpos),
      m_downpos(ass.m_downpos),m_r(ass.m_r) {
      if (&ass!=this)
        mk_stringset(m_info,&ass.m_info[0]);
    }
    ~xxEvent() {
    }
    xxEvent &operator=(const xxEvent &);
    bool operator==(const xxEvent &cmp) const;
    bool operator<(const xxEvent &cmp) const;
    void clear();
    int setInfo(const char *);
    int info(mk_string) const;
    int type2txt(mk_string) const;

};

extern int oswinexp xxTypeMouse(xxEvent *);
extern int oswinexp xxTypeKey(xxEvent *);
inline int oswinexp xxTypeInput(xxEvent *xxev) {

  return (xxTypeMouse(xxev)|xxTypeKey(xxev));

}

typedef int (*xxtranslateEventF)(void *,xxEvent *);
extern xxtranslateEventF oswinexp xxtranslateEventExt;
extern int oswinexp xxtranslateEvent(void *,xxEvent *);

} // namespace

#endif
