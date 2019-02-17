
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
    float m_size;
    int m_style;
    xxRectSize m_metric;
    mk_string m_fam;
    xxFnt(const char *fam="Arial",float size=10.,
          int style=xx_fntReg,xxRectSize metric=xxRectSize());
    xxFnt(const xxFnt &);
    ~xxFnt() { }
    xxFnt &operator=(const xxFnt &);
    bool operator==(const xxFnt &) const;
    bool operator<(const xxFnt &) const;
    int toString(mk_string) const;
    
};

typedef int (*xxtoClipboardF)(aux::Ucsstr *);
extern xxtoClipboardF oswinexp xxtoClipboardExt;
extern int oswinexp xxtoClipboard(aux::Ucsstr *);

typedef int (*xxfromClipboardF)(aux::Ucsstr *);
extern xxfromClipboardF oswinexp xxfromClipboardExt;
extern int oswinexp xxfromClipboard(aux::Ucsstr *);

} // namespace 

#endif

