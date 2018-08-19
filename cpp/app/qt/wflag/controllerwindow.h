#ifndef CONTROLLERWINDOW_H
#define CONTROLLERWINDOW_H

#include <QWidget>
#include <QPushButton>
#include <QRadioButton>
#include <QLabel>
#include <QGroupBox>
#include <QCheckBox>
#include <QLayout>
#include <QAbstractNativeEventFilter>

#include "previewwindow.h"

class CNativeEventFilter : public QAbstractNativeEventFilter {
  public:
    QObject *m_app;
    CNativeEventFilter(QObject *app=0) : m_app(app) {}
    virtual ~CNativeEventFilter() {}
    virtual bool  nativeEventFilter(const QByteArray &,void *,long *);
};

class ControllerWindow : public QWidget
{
    Q_OBJECT

public:
    ControllerWindow();

private slots:
    void updatePreview();

private:
    void createTypeGroupBox();
    void createHintsGroupBox();
    QCheckBox *createCheckBox(const QString &text);
    QRadioButton *createRadioButton(const QString &text);

    QList<PreviewWindow*> previewWindowList;
    //PreviewWindow *previewWindow;

    QGroupBox *typeGroupBox;
    QGroupBox *hintsGroupBox;
    QPushButton *quitButton;

    QRadioButton *windowRadioButton;
    QRadioButton *dialogRadioButton;
    QRadioButton *sheetRadioButton;
    QRadioButton *drawerRadioButton;
    QRadioButton *popupRadioButton;
    QRadioButton *toolRadioButton;
    QRadioButton *toolTipRadioButton;
    QRadioButton *splashScreenRadioButton;

    QCheckBox *msWindowsFixedSizeDialogCheckBox;
    QCheckBox *x11BypassWindowManagerCheckBox;
    QCheckBox *framelessWindowCheckBox;
    QCheckBox *windowNoShadowCheckBox;
    QCheckBox *windowTitleCheckBox;
    QCheckBox *windowSystemMenuCheckBox;
    QCheckBox *windowMinimizeButtonCheckBox;
    QCheckBox *windowMaximizeButtonCheckBox;
    QCheckBox *windowCloseButtonCheckBox;
    QCheckBox *windowContextHelpButtonCheckBox;
    QCheckBox *windowShadeButtonCheckBox;
    QCheckBox *windowStaysOnTopCheckBox;
    QCheckBox *windowStaysOnBottomCheckBox;
    QCheckBox *customizeWindowHintCheckBox;
};

#endif
