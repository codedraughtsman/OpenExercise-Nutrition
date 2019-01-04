#include "portionadder.h"
#include "ui_portionadder.h"

#include "storagemanager.h"
#include <QDebug>
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
	connect( ui->tableView, &QTableView::doubleClicked, this,
			 &PortionAdder::onTableClicked );
}
void PortionAdder::onTableClicked( const QModelIndex &index ) {
	qDebug() << "double clicked:" << index;
	if ( index.isValid() ) {
		QString foodName = index.data().toString();
		StorageManager::instance().addPortion( foodName, 100 );
	}
}
PortionAdder::~PortionAdder() { delete ui; }
