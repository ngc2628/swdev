
#include "controllerwindow.h"
#include <QtWidgets/QApplication>
#include <QtWidgets/QDesktopWidget>
#include <QtWidgets/QHBoxLayout>
#include <QtCore/QString>

#if defined (__linux__)
#include <QX11Info>
#include <xcb/xcb.h>
#include <errno.h>
#endif
#include <stdlib.h>
#include <unistd.h>

bool CNativeEventFilter::nativeEventFilter(const QByteArray &type,void *msg,long *res) {

  if (strlen(type.constData())==19 && 
      strncmp(type.constData(),"xcb_generic_event_t",19)==0) {
//printf ("%d [%d]\n",__LINE__,((xcb_generic_event_t*)msg)->response_type);
  }
  return false;
}

ControllerWindow::ControllerWindow()
{
    //previewWindow = new PreviewWindow(this);
    int ii=0,noScreens = qApp->desktop()->screenCount();
fprintf(stderr,"%d [%d] screens\n",__LINE__,noScreens);
    for (ii = 0; ii < noScreens; ii++)
    {
      previewWindowList.append(new PreviewWindow(qApp->desktop()->screen(ii)));
    }

    createTypeGroupBox();
    createHintsGroupBox();

    quitButton = new QPushButton(tr("&Quit"));
    connect(quitButton, SIGNAL(clicked()), qApp, SLOT(quit()));

    QHBoxLayout *bottomLayout = new QHBoxLayout;
    bottomLayout->addStretch();
    bottomLayout->addWidget(quitButton);

    QHBoxLayout *mainLayout = new QHBoxLayout;
    mainLayout->addWidget(typeGroupBox);
    mainLayout->addWidget(hintsGroupBox);
    mainLayout->addLayout(bottomLayout);
    setLayout(mainLayout);

    setWindowTitle(tr("Window Flags"));
    updatePreview();
    setWindowFlags(Qt::WindowStaysOnTopHint);

    qApp->installNativeEventFilter(new CNativeEventFilter(this));

}

void ControllerWindow::updatePreview()
{
    Qt::WindowFlags flags = 0;

    if (windowRadioButton->isChecked()) {
        flags = Qt::Window;
    } else if (dialogRadioButton->isChecked()) {
        flags = Qt::Dialog;
    } else if (sheetRadioButton->isChecked()) {
        flags = Qt::Sheet;
    } else if (drawerRadioButton->isChecked()) {
        flags = Qt::Drawer;
    } else if (popupRadioButton->isChecked()) {
        flags = Qt::Popup;
    } else if (toolRadioButton->isChecked()) {
        flags = Qt::Tool;
    } else if (toolTipRadioButton->isChecked()) {
        flags = Qt::ToolTip;
    } else if (splashScreenRadioButton->isChecked()) {
        flags = Qt::SplashScreen;
    }

    if (msWindowsFixedSizeDialogCheckBox->isChecked())
        flags |= Qt::MSWindowsFixedSizeDialogHint;
    if (x11BypassWindowManagerCheckBox->isChecked())
        flags |= Qt::X11BypassWindowManagerHint;
    if (framelessWindowCheckBox->isChecked())
        flags |= Qt::FramelessWindowHint;
    if (windowNoShadowCheckBox->isChecked())
        flags |= Qt::NoDropShadowWindowHint;
    if (windowTitleCheckBox->isChecked())
        flags |= Qt::WindowTitleHint;
    if (windowSystemMenuCheckBox->isChecked())
        flags |= Qt::WindowSystemMenuHint;
    if (windowMinimizeButtonCheckBox->isChecked())
        flags |= Qt::WindowMinimizeButtonHint;
    if (windowMaximizeButtonCheckBox->isChecked())
        flags |= Qt::WindowMaximizeButtonHint;
    if (windowCloseButtonCheckBox->isChecked())
        flags |= Qt::WindowCloseButtonHint;
    if (windowContextHelpButtonCheckBox->isChecked())
        flags |= Qt::WindowContextHelpButtonHint;
    if (windowShadeButtonCheckBox->isChecked())
        flags |= Qt::WindowShadeButtonHint;
    if (windowStaysOnTopCheckBox->isChecked())
        flags |= Qt::WindowStaysOnTopHint;
    if (windowStaysOnBottomCheckBox->isChecked())
        flags |= Qt::WindowStaysOnBottomHint;
    if (customizeWindowHintCheckBox->isChecked())
        flags |= Qt::CustomizeWindowHint;

    int noScreens = qApp->desktop()->screenCount();
    for (int i = 0; i < noScreens; i++)
    {
      PreviewWindow *previewWindow = previewWindowList[i];
      previewWindow->setWindowFlags(flags);
      QPoint pos = previewWindow->pos();
      if (pos.x() < 0)
        pos.setX(0);
      if (pos.y() < 0)
        pos.setY(0);
      previewWindow->move(pos);
      previewWindow->show();
    }
}

void ControllerWindow::createTypeGroupBox()
{
    typeGroupBox = new QGroupBox(tr("Type"));

    windowRadioButton = createRadioButton(tr("Window"));
    dialogRadioButton = createRadioButton(tr("Dialog"));
    sheetRadioButton = createRadioButton(tr("Sheet"));
    drawerRadioButton = createRadioButton(tr("Drawer"));
    popupRadioButton = createRadioButton(tr("Popup"));
    toolRadioButton = createRadioButton(tr("Tool"));
    toolTipRadioButton = createRadioButton(tr("Tooltip"));
    splashScreenRadioButton = createRadioButton(tr("Splash screen"));
    windowRadioButton->setChecked(true);

    QGridLayout *layout = new QGridLayout;
    layout->addWidget(windowRadioButton, 0, 0);
    layout->addWidget(dialogRadioButton, 1, 0);
    layout->addWidget(sheetRadioButton, 2, 0);
    layout->addWidget(drawerRadioButton, 3, 0);
    layout->addWidget(popupRadioButton, 0, 1);
    layout->addWidget(toolRadioButton, 1, 1);
    layout->addWidget(toolTipRadioButton, 2, 1);
    layout->addWidget(splashScreenRadioButton, 3, 1);
    typeGroupBox->setLayout(layout);
}

void ControllerWindow::createHintsGroupBox()
{
    hintsGroupBox = new QGroupBox(tr("Hints"));

    msWindowsFixedSizeDialogCheckBox =
            createCheckBox(tr("MS Windows fixed size dialog"));
    x11BypassWindowManagerCheckBox =
            createCheckBox(tr("X11 bypass window manager"));
    framelessWindowCheckBox = createCheckBox(tr("Frameless window"));
    windowNoShadowCheckBox = createCheckBox(tr("No drop shadow"));
    windowTitleCheckBox = createCheckBox(tr("Window title"));
    windowSystemMenuCheckBox = createCheckBox(tr("Window system menu"));
    windowMinimizeButtonCheckBox = createCheckBox(tr("Window minimize button"));
    windowMaximizeButtonCheckBox = createCheckBox(tr("Window maximize button"));
    windowCloseButtonCheckBox = createCheckBox(tr("Window close button"));
    windowContextHelpButtonCheckBox =
            createCheckBox(tr("Window context help button"));
    windowShadeButtonCheckBox = createCheckBox(tr("Window shade button"));
    windowStaysOnTopCheckBox = createCheckBox(tr("Window stays on top"));
    windowStaysOnBottomCheckBox = createCheckBox(tr("Window stays on bottom"));
    customizeWindowHintCheckBox= createCheckBox(tr("Customize window"));

    QGridLayout *layout = new QGridLayout;
    layout->addWidget(msWindowsFixedSizeDialogCheckBox, 0, 0);
    layout->addWidget(x11BypassWindowManagerCheckBox, 1, 0);
    layout->addWidget(framelessWindowCheckBox, 2, 0);
    layout->addWidget(windowNoShadowCheckBox, 3, 0);
    layout->addWidget(windowTitleCheckBox, 4, 0);
    layout->addWidget(windowSystemMenuCheckBox, 5, 0);
    layout->addWidget(customizeWindowHintCheckBox, 6, 0);
    layout->addWidget(windowMinimizeButtonCheckBox, 0, 1);
    layout->addWidget(windowMaximizeButtonCheckBox, 1, 1);
    layout->addWidget(windowCloseButtonCheckBox, 2, 1);
    layout->addWidget(windowContextHelpButtonCheckBox, 3, 1);
    layout->addWidget(windowShadeButtonCheckBox, 4, 1);
    layout->addWidget(windowStaysOnTopCheckBox, 5, 1);
    layout->addWidget(windowStaysOnBottomCheckBox, 6, 1);
    hintsGroupBox->setLayout(layout);
}

QCheckBox *ControllerWindow::createCheckBox(const QString &text)
{
    QCheckBox *checkBox = new QCheckBox(text);
    connect(checkBox, SIGNAL(clicked()), this, SLOT(updatePreview()));
    return checkBox;
}

QRadioButton *ControllerWindow::createRadioButton(const QString &text)
{
    QRadioButton *button = new QRadioButton(text);
    connect(button, SIGNAL(clicked()), this, SLOT(updatePreview()));
    return button;
}
