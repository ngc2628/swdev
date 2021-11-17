#ifndef PREVIEWWINDOW_H
#define PREVIEWWINDOW_H

#include <QtWidgets/QMainWindow>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QLayout>
#include <QtWidgets/QFrame>
#include <QtWidgets/QApplication>

class CSpanToolBar;

class PreviewWindow : public QMainWindow
{
    Q_OBJECT

public:
    PreviewWindow(QWidget *parent = 0);

    void setWindowFlags(Qt::WindowFlags flags);

private:
    QTextEdit *textEdit;
    QPushButton *closeButton;
    CSpanToolBar* pToolBar;
};

#endif
