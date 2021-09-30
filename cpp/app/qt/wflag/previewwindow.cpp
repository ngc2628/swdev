
#include "previewwindow.h"
#include "spantoolbar.h"

PreviewWindow::PreviewWindow(QWidget *parent)
    : QMainWindow(parent)
{
    textEdit = new QTextEdit;
    textEdit->setReadOnly(true);
    textEdit->setLineWrapMode(QTextEdit::NoWrap);

    closeButton = new QPushButton(tr("&Close"));
    connect(closeButton, SIGNAL(clicked()), this, SLOT(close()));

    QWidget* pCentralWidget = new QWidget(this);
    setCentralWidget(pCentralWidget);
    QVBoxLayout *layout = new QVBoxLayout;
    layout->addWidget(textEdit);
    layout->addWidget(closeButton);
    pCentralWidget->setLayout(layout);
    
    setWindowTitle(tr("Preview"));
    if (parent)
    {
      QPoint point = parent->mapToGlobal(parent->rect().topLeft());
      move(point);

      //I need a proxy widget to calculate the width of the border and the height
      //of the titlebar.
      //see also: http://stackoverflow.com/questions/7408082
      //|- How to get the width of a window frame, before creating any windows?
      //and see: doc.qt.io/archives/qt-4.8/application-windows.html
      QFrame aFrame(parentWidget(), windowFlags());
      aFrame.setWindowOpacity(0.0);
      aFrame.show();
      QApplication::processEvents();
      QSize frame = aFrame.frameSize() - aFrame.size();
      QSize size = parent->size();
      resize(parent->size() -frame);
    }

    pToolBar = new CSpanToolBar(this);
    addToolBar(pToolBar);
}

void PreviewWindow::setWindowFlags(Qt::WindowFlags flags)
{
    QWidget::setWindowFlags(flags);

    int homeScreenNo = pToolBar->getHomeScreenNumber();
    switch (homeScreenNo)
    {
    case 0:
      textEdit->setStyleSheet("background-color:red;");
      break;
    case 1:
      textEdit->setStyleSheet("background-color:yellow;");
      break;
    case 2:
      textEdit->setStyleSheet("background-color:green;");
      break;
    case 3:
      textEdit->setStyleSheet("background-color:blue;");
      break;
    default:
      break;
    }



    QString text;
    text += "Screen: ";
    text += QString::number(pToolBar->getHomeScreenNumber());

    text += "\n---------------\n\n";

    Qt::WindowFlags type = (flags & Qt::WindowType_Mask);
    if (type == Qt::Window) {
        text += "Qt::Window";
    } else if (type == Qt::Dialog) {
        text = "Qt::Dialog";
    } else if (type == Qt::Sheet) {
        text = "Qt::Sheet";
    } else if (type == Qt::Drawer) {
        text = "Qt::Drawer";
    } else if (type == Qt::Popup) {
        text = "Qt::Popup";
    } else if (type == Qt::Tool) {
        text = "Qt::Tool";
    } else if (type == Qt::ToolTip) {
        text = "Qt::ToolTip";
    } else if (type == Qt::SplashScreen) {
        text = "Qt::SplashScreen";
    }

    if (flags & Qt::MSWindowsFixedSizeDialogHint)
        text += "\n| Qt::MSWindowsFixedSizeDialogHint";
    if (flags & Qt::X11BypassWindowManagerHint)
        text += "\n| Qt::X11BypassWindowManagerHint";
    if (flags & Qt::FramelessWindowHint)
        text += "\n| Qt::FramelessWindowHint";
    if (flags & Qt::NoDropShadowWindowHint)
        text += "\n| Qt::NoDropShadowWindowHint";
    if (flags & Qt::WindowTitleHint)
        text += "\n| Qt::WindowTitleHint";
    if (flags & Qt::WindowSystemMenuHint)
        text += "\n| Qt::WindowSystemMenuHint";
    if (flags & Qt::WindowMinimizeButtonHint)
        text += "\n| Qt::WindowMinimizeButtonHint";
    if (flags & Qt::WindowMaximizeButtonHint)
        text += "\n| Qt::WindowMaximizeButtonHint";
    if (flags & Qt::WindowCloseButtonHint)
        text += "\n| Qt::WindowCloseButtonHint";
    if (flags & Qt::WindowContextHelpButtonHint)
        text += "\n| Qt::WindowContextHelpButtonHint";
    if (flags & Qt::WindowShadeButtonHint)
        text += "\n| Qt::WindowShadeButtonHint";
    if (flags & Qt::WindowStaysOnTopHint)
        text += "\n| Qt::WindowStaysOnTopHint";
    if (flags & Qt::CustomizeWindowHint)
        text += "\n| Qt::CustomizeWindowHint";


    textEdit->setPlainText(text);
}
