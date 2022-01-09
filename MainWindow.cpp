#include "MainWindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    resize(500, 500);
    setCentralWidget(&m_textWidget);
}

MainWindow::~MainWindow()
{
}

