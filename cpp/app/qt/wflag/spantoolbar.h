//..begin "Ifdef"
#ifndef _spantoolbar_H_
#define _spantoolbar_H_
//..end "Ifdef"




//..begin "UTF:includes"
#include <QString>
#include <QObject>
#include <QToolBar>
#include <QToolButton>
#include <QVector>
//..end "UTF:includes"

class QMainWindow;
class CSpanToolBar;

class CSpanToolButton : public QToolButton
{
  Q_OBJECT

  friend class CSpanToolBar;

  public:
    /**
    \attention: for internal use only 
    */
    CSpanToolButton();
    CSpanToolButton( CSpanToolBar * parent, 
                    const int & screen );
    virtual ~CSpanToolButton();
    int getScreenNumber() const {return m_screen;}

  signals:
    void signalClicked(const CSpanToolButton *);
    void signalToggled(const CSpanToolButton *, bool);

  public slots: 
    void slotButtonClicked();
    void slotButtonToggled(bool bOn);


  private: 
    void init();
    int m_screen;
    CSpanToolBar* m_pSpanToolBar;
};

class CSpanToolBar : public QToolBar
{
  Q_OBJECT

  public:
    CSpanToolBar( QMainWindow* pMainWindow );
    virtual ~CSpanToolBar();
    int getHomeScreenNumber();

  public slots: 
    void slotButtonClicked(const CSpanToolButton * pSpanToolButton);
    void slotButtonToggled(const CSpanToolButton * pSpanToolButton, bool bOn);


  private:
    void createButtons();
    QVector<CSpanToolButton*> m_toolBtnVec;
    bool m_bAcceptToggleSignal;
};

//..begin "Endif"
#endif //_spantoolbar_H_
//..end "Endif"
