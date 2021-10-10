#include <QApplication>
#include <QLabel>

#include "OverheatMainWidnow.h"

int main(int argc, char** argv)
{
	QApplication a(argc, argv);
	QMainWindow& w = OverheatMainWindow::instance();
	QLabel l("Hello Qt!", &w);
	w.show();
	return a.exec();
}
