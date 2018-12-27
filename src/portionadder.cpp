#include "portionadder.h"
#include "ui_portionadder.h"

#include <QSqlQueryModel>

PortionAdder::PortionAdder( QWidget *parent )
	: QDialog( parent ), ui( new Ui::PortionAdder ) {
	ui->setupUi( this );
	QSqlQueryModel *model = new QSqlQueryModel;
	model->setQuery( "SELECT foodName, kjPer100g FROM food" );
	model->setHeaderData( 0, Qt::Horizontal, tr( "Name" ) );
	model->setHeaderData( 1, Qt::Horizontal, tr( "kj/100g" ) );
	ui->tableView->setModel( model );
	connect( ui->closeButton, &QPushButton::released, this, &QDialog::close );
	ui->tableView->setSelectionBehavior( QAbstractItemView::SelectRows );
	ui->tableView->verticalHeader()->hide();
}

PortionAdder::~PortionAdder() { delete ui; }
