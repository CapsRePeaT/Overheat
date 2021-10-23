#include <QApplication>
#include <QLabel>

#include "mainwindow.h"

int main(int argc, char** argv)
{
	QApplication a(argc, argv);
    QMainWindow& w = MainWindow::instance();
	w.show();
	return a.exec();
}
