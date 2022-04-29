#include <qapplication.h>
#include <spdlog/spdlog.h>

#include <QApplication>
#include <QLabel>

#include "log.h"
#include "config.h"
#include "mainwindow.h"

int main(int argc, char** argv) {
	spdlog::set_level(spdlog::level::debug);
	QApplication a(argc, argv);
	Log::Init();
	Config::Init(QApplication::applicationFilePath().toStdString());
	MainWindow w;
	w.show();
	return a.exec();
}
