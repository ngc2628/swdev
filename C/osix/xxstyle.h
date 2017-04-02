
#ifndef _xxstyle_h_
#define _xxstyle_h_

#include <auxx/auxx.h>

namespace osix {

const int xx_cntcolors=16;
const unsigned int xx_somecolors[xx_cntcolors]={
  4278190080UL,4294901760UL,4278255360UL,4278190335UL,4294967295UL,4294967040UL,
  4294902015UL,4278255615UL,4294927410UL,4278222912UL,4288708672UL,4284506208UL,
  4286611584UL,4288716960UL,4290822336UL,4292927712UL};
enum xx_esomecolors {
  black=0,red,green,blue,white,yellow,magenta,cyan,orange,
  darkgreen,brown,grey96,grey128,grey160,grey192,grey224};

class oswinexp xxStyle {

  public:
    unsigned int m_color;
    short m_style;
    float m_width;
    xxStyle(unsigned int color=0,short style=0,float width=.0) : m_color(color),m_style(style),m_width(width) {
    }
    xxStyle(const xxStyle &ass) : m_color(ass.m_color),m_style(ass.m_style),m_width(ass.m_width) {
    }
    ~xxStyle() {
    }
    xxStyle &operator=(const xxStyle &ass) {
      m_color=ass.m_color;
      m_style=ass.m_style;
      m_width=ass.m_width;
      return *this;
    }
    bool operator==(const xxStyle &cmp) const {
      return (m_color==cmp.m_color && m_style==cmp.m_style && m_width==cmp.m_width);  
    }
    bool operator<(const xxStyle &cmp) const {
      return (m_color<cmp.m_color || (m_color==cmp.m_color && (m_style<cmp.m_style || 
              (m_style==cmp.m_style && m_width<cmp.m_width)))
             ); 
    }
    void set(unsigned int color=0,short style=0,float width=.0) {
      m_color=color;
      m_style=style;
      m_width=width;
    }
    int cantPaint(unsigned int color,int usewidth) {
      return ((m_color==color || m_style<=0 || (usewidth>0 && m_width<=.0)) ? 1 : 0);
    }
    void toString(aux::Asciistr *) const;
    
};

extern unsigned int oswinexp xxcolor(unsigned short,unsigned short,unsigned short,unsigned short alpha=255);
extern void oswinexp xxrgba(unsigned int,unsigned short *r=0,unsigned short *g=0,unsigned short *b=0,unsigned short *a=0);
extern unsigned short oswinexp xxred(unsigned int);
extern unsigned short oswinexp xxgreen(unsigned int);
extern unsigned short oswinexp xxblue(unsigned int);
extern unsigned short oswinexp xxalpha(unsigned int);

} // namespace 

#endif
