
#include <QtWidgets/QApplication>
#include <stdio.h>
#include <string.h>
//#include <pthread.h>
#include <osix/xxevent.h>
#include <osix/xxpaint.h>
#include <qt/util/qtutil.h>
#include <qt/util/qtpaint.h>
#include <qt/util/qtutil.h>
#include <app/qt/spreadsheetapp/appframe.h>

int main(int argc,char **argv) { 

  QApplication aa(argc,argv);
  //QApplication::setDesktopSettingsAware( FALSE );
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
    
  int ii=0;
  for (ii=0;ii<argc;ii++)
    printf ("arg#%d : %s\n",ii,argv[ii]);
  if (argc>1 && strcmp(argv[1],"-help")==0) 
    printf ("no help\n");
    
  qtspreadsheet::AppFrame *cc=new qtspreadsheet::AppFrame(0);
  cc->setMinimumSize(QSize(1000,600));
  //aa.setMainWidget(cc);
  cc->show();
  return aa.exec();

}

