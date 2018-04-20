
#ifndef DIALOGS_H
#define DIALOGS_H

#include <X11/Xm/Form.h>
#include <X11/Xm/Scale.h>
#include <X11/Xm/PushB.h>
#include <X11/Xm/ToggleB.h>
#include <X11/Xm/Frame.h>
#include <X11/Xm/RowColumn.h>
#include <X11/Xm/MessageB.h>
#include <X11/Xm/ComboBox.h>
#include <X11/Xm/SSpinB.h>
#include <X11/Xm/SpinB.h>
#include <X11/Xm/TextF.h>
#include <X11/Xm/Text.h>
#include <X11/Xm/Label.h>
#include <xt/gl/tst11/lights.h>

extern Widget lightdlg_w[light_n],lighton_w[light_n],lightnature_w[light_n],
  naturecolor_w[light_n],material_w[light_n],materialcolor_w[light_n],
  naturealpha_w[light_n],naturered_w[light_n],naturegreen_w[light_n],
  natureblue_w[light_n],materialalpha_w[light_n],materialred_w[light_n],
  materialgreen_w[light_n],materialblue_w[light_n];


extern void createLightDialog(Widget,int);

#endif
