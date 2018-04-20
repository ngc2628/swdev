
#include <osix/xxevent.h>
#include <osix/xxpaint.h>
#include <qt/util/qtpaint.h>
#include <app/qt/simpleplotapp/appframe.h>
#include <stdio.h>
#include <string.h>
//#include <pthread.h>
#include <QtWidgets/QApplication>

int main(int argc,char **argv) { 

  aux::Asciistr fname;
  if (argc>1)
    fname=argv[1];

  QApplication a(argc,argv);
  //QApplication::setDesktopSettingsAware( FALSE );
  osix::xxtranslateEventExt=qtutil::xxtranslateQevent;
  osix::xxpredrawExt=qtutil::xxpredrawQt;
  osix::xxpostdrawExt=qtutil::xxpostdrawQt;
  osix::xxdrawLineExt=qtutil::xxdrawLineQt;
  osix::xxdrawLinesExt=qtutil::xxdrawLinesQt;
  osix::xxdrawRectExt=qtutil::xxdrawRectQt;
  osix::xxfillRectExt=qtutil::xxfillRectQt;
  osix::xxfillPolygonExt=qtutil::xxfillPolygonQt;
  osix::xxdrawTextExt=qtutil::xxdrawTextQt;
  osix::xxdrawCtrlExt=qtutil::xxdrawCtrlQt;
  osix::xxresizeDrawableExt=qtutil::xxresizeDrawableQt;
  osix::xxcopyAreaExt=qtutil::xxcopyAreaQt;
    
  int ii=0;
  for (ii=0;ii<argc;ii++)
    printf ("arg#%d : %s\n",ii,argv[ii]);
  if (argc>1 && strcmp(argv[1],"-help")==0) 
    printf ("no help\n");
    
  qtsimpleplot::AppFrame *c=new qtsimpleplot::AppFrame(0,fname);
  c->setMinimumSize(QSize(1000,600));
  //a.setMainWidget(c);
  c->show();
  return a.exec();

}

