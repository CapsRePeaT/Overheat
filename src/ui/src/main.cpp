#include <spdlog/spdlog.h>

#include <QApplication>
#include <QLabel>

#include "log.h"
#include "mainwindow.h"

int main(int argc, char** argv) {
	spdlog::set_level(spdlog::level::debug);
	Log::Init();
	QApplication a(argc, argv);
	MainWindow w;
	w.show();
	return a.exec();
}
