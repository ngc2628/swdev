
#ifndef _XCBTST11DESKTOP_H_
#define _XCBTST11DESKTOP_H_

#include <xcb/xcb.h>

struct MotifHints {
  unsigned int flags;
  unsigned int  functions;
  unsigned int  decorations;
  int  input_mode;
  unsigned int  status;
};

const int maxscreens=16;
struct xcbdesktop_t {
  xcb_screen_t *xcbscreen;
  xcb_drawable_t xcbmainwindow;
  xcb_drawable_t xcbmainwindowbutton;
};
extern struct xcbdesktop_t xcbdesktop[maxscreens];

extern int setupGui(xcb_connection_t *);
extern int findScreens(xcb_connection_t *);
extern xcb_drawable_t spawnDesktop(xcb_connection_t *,struct xcbdesktop_t *);
extern xcb_drawable_t subWindow(xcb_connection_t *,struct xcbdesktop_t *);

#endif
