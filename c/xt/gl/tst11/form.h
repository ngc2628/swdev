
#ifndef FORM_H
#define FORM_H

#include <X11/Xm/Form.h>
#include <X11/Xm/Scale.h>
#include <X11/Xm/PushB.h>
#include <X11/Xm/ToggleB.h>
#include <X11/Xm/Frame.h>
#include <X11/Xm/RowColumn.h>

extern Widget mainframe_w,drawframe_w,lightbutton_w,xmod_w,ymod_w,zmod_w;
extern Widget createMainFrame(Widget);
extern void cbModSel(Widget,XtPointer,XtPointer);
extern void cbModView(Widget,XtPointer,XtPointer);

#endif
