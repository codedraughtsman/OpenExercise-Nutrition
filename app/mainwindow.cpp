#include "mainwindow.h"

#include <towerwidget.h>

MainWindow::MainWindow( QWidget *parent ) : QMainWindow( parent ) {
	QWidget *tower = new TowerWidget( this );
	setCentralWidget( tower );
}

MainWindow::~MainWindow() {}
