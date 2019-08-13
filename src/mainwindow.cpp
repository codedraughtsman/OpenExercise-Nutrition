#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "addfooddialog.h"
#include "portionadder.h"
#include <QToolBar>

MainWindow::MainWindow( QWidget *parent )
	: QMainWindow( parent ), ui( new Ui::MainWindow ) {
	ui->setupUi( this );
	QToolBar *tools = new QToolBar();
	QAction *addPortionAction = new QAction( "add portion", this );
	connect( addPortionAction, &QAction::triggered, this,
			 &MainWindow::createAddPortion );
	tools->addAction( addPortionAction );
	QAction *addFoodAction = new QAction( "add food", this );
	connect( addFoodAction, &QAction::triggered, this,
			 &MainWindow::createAddFood );
	tools->addAction( addFoodAction );
	addToolBar( tools );
}
void MainWindow::createAddPortion() {
	PortionAdder *p = new PortionAdder();
	p->show();
}
void MainWindow::createAddFood() {
	addFoodDialog *p = new addFoodDialog();
	p->show();
}

MainWindow::~MainWindow() { delete ui; }
