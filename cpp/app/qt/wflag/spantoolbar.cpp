#include <QMainWindow>
#include <QApplication>
#include <QDesktopWidget>
#include <QX11Info>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <xcb/xcb.h>

#include "spantoolbar.h"


/*-----------------------------------------------------------------------------
Standardkonstruktor
-----------------------------------------------------------------------------*/
CSpanToolButton :: CSpanToolButton () : QToolButton( )
{
  setObjectName("unknown spantoolbutton");
  m_pSpanToolBar = 0; 
  m_screen = 0; 
  setMinimumSize(QSize(24,24));
}


/*-----------------------------------------------------------------------------
Konstruktor
-----------------------------------------------------------------------------*/
CSpanToolButton :: CSpanToolButton( CSpanToolBar * parent, 
                                    const int & screen  ) 
                 : QToolButton( parent)
{
  setObjectName(QString::number(screen));
  m_pSpanToolBar = parent;
  m_screen = screen;
  setMinimumSize(QSize(24,24));
  init();
}


/*-----------------------------------------------------------------------------
Destruktor
-----------------------------------------------------------------------------*/
CSpanToolButton :: ~CSpanToolButton()
{
}

/*-----------------------------------------------------------------------------
init
-----------------------------------------------------------------------------*/
void CSpanToolButton :: init() 
{
  setText(QString::number(m_screen+1));
  setToolButtonStyle(Qt::ToolButtonTextBesideIcon);

  QWidget* pAppWin = window();
  int appOnScreen = qApp->desktop()->screenNumber(pAppWin);
  if (appOnScreen == m_screen)
  {
    setToolTip("Heimatmonitor");
    setStyleSheet("background-color:blue;");
    //QPalette p = palette();
    //p.setColor(backgroundRole(), QColor(0,0,255));
    //setPalette(p);
    connect(this, SIGNAL(clicked()), this, SLOT(slotButtonClicked()));
  }
  else
  {
    setToolTip(QString("Monitor %1").arg(m_screen+1));
    setCheckable(true);
    //setStyleSheet("QAbstractButton::checked {background-color:red;}"); //fkt nicht mit PSIlaf
    connect(this, SIGNAL(toggled(bool)), this, SLOT(slotButtonToggled(bool)));
  }
}

/*-----------------------------------------------------------------------------
slotButtonClicked
-----------------------------------------------------------------------------*/
void CSpanToolButton :: slotButtonClicked()
{
    emit signalClicked(this);
    return;
}

/*-----------------------------------------------------------------------------
slotButtonToggled
-----------------------------------------------------------------------------*/
void CSpanToolButton :: slotButtonToggled(bool bOn)
{
  emit signalToggled(this, bOn);
  return;
}


//============================================================================


/*-----------------------------------------------------------------------------
Konstruktor
-----------------------------------------------------------------------------*/
CSpanToolBar::CSpanToolBar(QMainWindow* pMainWindow)
                   : QToolBar("Arbeitsfläche anpassen", pMainWindow)
{
  //Top + false
  setMovable(false);
  createButtons();
  m_bAcceptToggleSignal = true;
}


/*-----------------------------------------------------------------------------
Destruktor
-----------------------------------------------------------------------------*/
CSpanToolBar::~CSpanToolBar()
{
    blockSignals(true);
    disconnect(this);
}

/*-----------------------------------------------------------------------------
createButtons
-----------------------------------------------------------------------------*/
void CSpanToolBar::createButtons()
{
  int nScreens = qApp->desktop()->screenCount();
  for (int i=0; i < nScreens; i++)
  {
    CSpanToolButton* pSpanToolButton = new CSpanToolButton(this, i);
    addWidget(pSpanToolButton);
    m_toolBtnVec.append(pSpanToolButton);
    connect( pSpanToolButton, SIGNAL(signalClicked(const CSpanToolButton*)), 
             this, SLOT(slotButtonClicked(const CSpanToolButton*)) );
    connect( pSpanToolButton, SIGNAL(signalToggled(const CSpanToolButton*, bool)), 
             this, SLOT(slotButtonToggled(const CSpanToolButton*, bool)) );
  }
}

/*-----------------------------------------------------------------------------
createButtons
-----------------------------------------------------------------------------*/
int CSpanToolBar::getHomeScreenNumber()
{
  QList<CSpanToolButton*> tbList = findChildren<CSpanToolButton*>();
  foreach(CSpanToolButton* pSpanToolBtn, tbList)
  {
    if (!pSpanToolBtn->isCheckable())
    {
      return pSpanToolBtn->getScreenNumber();
    }
  }
}

/*-----------------------------------------------------------------------------

-----------------------------------------------------------------------------*/
void CSpanToolBar::slotButtonClicked( const CSpanToolButton * pSpanToolButton )
{
  if (!pSpanToolButton)
  {
    return;
  }
  QWidget* pScreenWidget = qApp->desktop()->screen(pSpanToolButton->getScreenNumber());
  QRect myScreenRect = pScreenWidget->rect();
  QList<CSpanToolButton*> tbList = findChildren<CSpanToolButton*>();
  foreach(CSpanToolButton* pSpanToolBtn, tbList)
  {
    if (pSpanToolBtn && pSpanToolBtn->isCheckable() && pSpanToolBtn->isChecked())
    {
      pSpanToolBtn->setChecked(false);
    }
  }

  QWidget* previewWindow = parentWidget();
  QSize frame = previewWindow->frameSize() - previewWindow->size();
  previewWindow->resize(myScreenRect.size()- frame);
  previewWindow->move(pScreenWidget->pos());
}

/*-----------------------------------------------------------------------------

-----------------------------------------------------------------------------*/
void CSpanToolBar::slotButtonToggled( const CSpanToolButton * pSpanToolButton,
                                      bool bOn )
{
  if (!pSpanToolButton || !m_bAcceptToggleSignal)
  {
    return;
  }
  int myScreen = getHomeScreenNumber();
  QWidget* pScreenWidget = qApp->desktop()->screen(myScreen);
  //TRACE(gT_LVIS_APP, T_TRC, QString("myScreenRect: %1x%2+%3+%4").arg(myScreenRect.width()).arg(myScreenRect.height()).arg(myScreenRect.x()).arg(myScreenRect.y()));
  QRect spanRect = pScreenWidget->geometry();
  QList<CSpanToolButton*> tbList = findChildren<CSpanToolButton*>();
  foreach(CSpanToolButton* pSpanToolBtn, tbList)
  {
    if (pSpanToolBtn && pSpanToolBtn->isCheckable() && pSpanToolBtn->isChecked())
    {
      int screenno = pSpanToolBtn->getScreenNumber();
      QRect tbScreenRect = qApp->desktop()->screen(screenno)->geometry();
      //tbScreenRect.move(qApp->desktop()->screen(screenno)->pos());
      //TRACE(gT_LVIS_APP, T_TRC, QString("tbScreenRect: %1x%2+%3+%4").arg(tbScreenRect.width()).arg(tbScreenRect.height()).arg(tbScreenRect.x()).arg(tbScreenRect.y()));
      spanRect = spanRect.united(tbScreenRect);
      //TRACE(gT_LVIS_APP, T_TRC, QString("spanRect: %1x%2+%3+%4").arg(spanRect.width()).arg(spanRect.height()).arg(spanRect.x()).arg(spanRect.y()));
    }
  }
  QWidget* pAppWin = parentWidget();
  QSize frame = pAppWin->frameSize() - pAppWin->size();
  pAppWin->resize(spanRect.size()- frame);
  pAppWin->move(spanRect.topLeft());
  qApp->processEvents();
  unsigned int values[]={XCB_STACK_MODE_ABOVE};
  xcb_configure_window(QX11Info::connection(),pAppWin->window()->winId(),XCB_CONFIG_WINDOW_STACK_MODE,values);
  xcb_flush(QX11Info::connection());

  //Togglestatus angleichen
  m_bAcceptToggleSignal = false;
  int tScreen = pSpanToolButton->getScreenNumber();
  if (bOn)
  {
    if (tScreen > myScreen)
    {
      for (int i = myScreen+1; i<tScreen; i++)
      {
        m_toolBtnVec[i]->setChecked(true);
      }
    }
    else
    {
      for (int i = tScreen+1; i < myScreen; i++)
      {
        m_toolBtnVec[i]->setChecked(true);
      }
    }
  }
  else
  {
    if (tScreen > myScreen)
    {
      for (int i = tScreen+1; i< (int)m_toolBtnVec.count(); i++)
      {
        m_toolBtnVec[i]->setChecked(false);
      }
    }
    else
    {
      for (int i = 0; i < tScreen; i++)
      {
        m_toolBtnVec[i]->setChecked(false);
      }
    }
  }
  m_bAcceptToggleSignal = true;
}


