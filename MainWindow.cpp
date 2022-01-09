#include "MainWindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setCentralWidget(&m_textWidget);
}

MainWindow::~MainWindow()
{
}

