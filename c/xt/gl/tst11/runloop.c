
#include <xt/gl/tst11/globals.h>

/*
KeyPress                2
KeyRelease              3
ButtonPress             4
ButtonRelease           5
MotionNotify            6
EnterNotify             7
LeaveNotify             8
FocusIn                 9
FocusOut                10
KeymapNotify            11
Expose                  12
GraphicsExpose          13
NoExpose                14
VisibilityNotify        15
CreateNotify            16
DestroyNotify           17
UnmapNotify             18
MapNotify               19
MapRequest              20
ReparentNotify          21
ConfigureNotify         22
ConfigureRequest        23
GravityNotify           24
ResizeRequest           25
CirculateNotify         26
CirculateRequest        27
PropertyNotify          28
SelectionClear          29
SelectionRequest        30
SelectionNotify         31
ColormapNotify          32
ClientMessage           33
MappingNotify           34
GenericEvent            35
*/

int runloop() {

  int res=0;
  XEvent event;

  while (run!=0) {
    XtAppNextEvent(applcontext,&event);
    XtDispatchEvent(&event);
  }

  return res;


}
