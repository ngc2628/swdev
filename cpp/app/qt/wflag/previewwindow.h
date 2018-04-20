#ifndef PREVIEWWINDOW_H
#define PREVIEWWINDOW_H

#include <QMainWindow>
#include <QPushButton>
#include <QTextEdit>
#include <QLayout>
#include <QFrame>
#include <QApplication>

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
