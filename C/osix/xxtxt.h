
#ifndef _xxtxt_h_
#define _xxtxt_h_

#include <auxx/auxx.h>
#include <osix/xxshape.h>

namespace osix {

const int xx_fntReg=1;
const int xx_fntBold=2;
const int xx_fntItalic=4;

const int xx_alignleft=1;
const int xx_alignright=2;
const int xx_alignhcenter=4;
const int xx_aligntop=8;
const int xx_alignbottom=16;
const int xx_alignvcenter=32;

class oswinexp xxFnt {

  public:
    aux::Asciistr m_fam;
    float m_size;
    int m_style;
    xxRectSize m_metric;
    xxFnt(const aux::Asciistr &fam="Arial",float size=10.,
          int style=xx_fntReg,xxRectSize metric=xxRectSize()) :
      m_fam(fam),m_size(size),m_style(style),m_metric(metric) {
    }
    xxFnt(const xxFnt &ass) : m_fam(ass.m_fam),m_size(ass.m_size),m_style(ass.m_style) {
    }
    ~xxFnt() { }
    xxFnt &operator=(const xxFnt &ass);
    bool operator==(const xxFnt &cmp) const {
      return (m_fam==cmp.m_fam && m_size==cmp.m_size && m_style==cmp.m_style);
    }
    bool operator<(const xxFnt &cmp) const {
      return (m_fam<cmp.m_fam || 
               (m_fam==cmp.m_fam && (m_size<cmp.m_size || 
                 (m_size==cmp.m_size && m_style<cmp.m_style))));
    }
    void toString(aux::Asciistr *) const;
    
};

typedef int (*xxtoClipboardF)(aux::Ucsstr *);
extern xxtoClipboardF oswinexp xxtoClipboardExt;
extern int oswinexp xxtoClipboard(aux::Ucsstr *);

typedef int (*xxfromClipboardF)(aux::Ucsstr *);
extern xxfromClipboardF oswinexp xxfromClipboardExt;
extern int oswinexp xxfromClipboard(aux::Ucsstr *);

} // namespace 

#endif

