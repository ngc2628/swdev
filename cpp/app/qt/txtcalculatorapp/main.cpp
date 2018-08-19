
#include <QtWidgets/QApplication>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
//#include <pthread.h>
#include <osix/xxkey.h>
#include <osix/xxmouse.h>
#include <osix/xxpaint.h>
#include <osix/xxevent.h>
#include <qt/util/qtutil.h>
#include <qt/util/qtpaint.h>
#include <app/qt/txtcalculatorapp/appframe.h>

int main(int argc,char **argv) { 

  //QApplication::setDesktopSettingsAware( FALSE );
  QApplication a(argc,argv); 

  osix::xxtranslateEventExt=qtutil::xxtranslateQevent;
  osix::xxpredrawExt=qtutil::xxpredrawQt;
  osix::xxpostdrawExt=qtutil::xxpostdrawQt;
  osix::xxdrawLineExt=qtutil::xxdrawLineQt;
  osix::xxdrawRectExt=qtutil::xxdrawRectQt;
  osix::xxfillRectExt=qtutil::xxfillRectQt;
  osix::xxdrawTextExt=qtutil::xxdrawTextQt;
  osix::xxdrawCtrlExt=qtutil::xxdrawCtrlQt;
  osix::xxresizeDrawableExt=qtutil::xxresizeDrawableQt;
  osix::xxcopyAreaExt=qtutil::xxcopyAreaQt;
  osix::xxtoClipboardExt=qtutil::toQClipboard;
  osix::xxfromClipboardExt=qtutil::fromQClipboard;

  int ii=0;
  for (ii=0;ii<argc;ii++)
    printf ("arg#%d : %s\n",ii,argv[ii]);
  if (argc>1 && strcmp(argv[1],"-help")==0)
    printf ("no help\n");
    
  qtcalculator::AppFrame *c=new qtcalculator::AppFrame(0);
  c->setFixedSize(c->minimumSize());
  c->show();
  return a.exec();

}

