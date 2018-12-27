#include "mainwindow.h"

#include <QToolBar>
#include <portionadder.h>
#include <towerwidget.h>

MainWindow::MainWindow( QWidget *parent ) : QMainWindow( parent ) {
	QWidget *tower = new TowerWidget( this );
	setCentralWidget( tower );
	QToolBar *tools = new QToolBar();
	QAction *addPortionAction = new QAction( "add portion", this );
	connect( addPortionAction, &QAction::triggered, this,
			 &MainWindow::createAddPortion );
	tools->addAction( addPortionAction );
	addToolBar( tools );
}
void MainWindow::createAddPortion() {
	PortionAdder *p = new PortionAdder();
	p->show();
}

MainWindow::~MainWindow() {}
