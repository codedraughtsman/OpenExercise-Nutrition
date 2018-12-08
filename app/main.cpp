#include "mainwindow.h"
#include "storagemanager.h"
#include <QApplication>

int main( int argc, char *argv[] ) {
	QApplication a( argc, argv );
	StorageManager::init();
	MainWindow w;
	w.show();

	return a.exec();
}
