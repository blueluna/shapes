#include <QApplication>
#include <QFile>
#include "mainwindow.hpp"

int main(int argc, char *argv[])
{
	QApplication app(argc, argv);

	// Load an application style
	QFile styleFile(":/stylesheets/stylesheet");
	styleFile.open(QFile::ReadOnly);
	// Apply the loaded stylesheet
	QString style(styleFile.readAll());
	app.setStyleSheet(style);

	MainWindow mainWindow;
	mainWindow.show();
	
	return app.exec();
}
