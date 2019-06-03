#include "portionadder.h"
#include "ui_portionadder.h"

#include "storagemanager.h"
#include <QDebug>
#include <QInputDialog>
#include <QSortFilterProxyModel>
#include <QSqlQueryModel>

PortionAdder::PortionAdder( QWidget *parent )
	: QDialog( parent ), ui( new Ui::PortionAdder ) {
	ui->setupUi( this );

	ui->dateTimeEdit->setDateTime( QDateTime::currentDateTime() );

	QSqlQueryModel *model = new QSqlQueryModel;
	model->setQuery( "SELECT foodName, kjPer100g FROM food" );
	model->setHeaderData( 0, Qt::Horizontal, tr( "Name" ) );
	model->setHeaderData( 1, Qt::Horizontal, tr( "kj/100g" ) );
	// connect(StorageManager::instance(),
	// &StorageManager::dataChanged,model,&QSqlQueryModel::);
	QSortFilterProxyModel *proxyModel = new QSortFilterProxyModel( this );
	proxyModel->setSourceModel( model );
	proxyModel->setFilterKeyColumn( 0 );
	proxyModel->setFilterCaseSensitivity(
		Qt::CaseSensitivity::CaseInsensitive );

	ui->tableView->setModel( proxyModel );
	ui->tableView->setSortingEnabled( true );

	connect( ui->filterLineEdit, &QLineEdit::textChanged, proxyModel,
			 &QSortFilterProxyModel::setFilterWildcard );

	connect( ui->closeButton, &QPushButton::released, this, &QDialog::close );
	ui->tableView->setSelectionBehavior( QAbstractItemView::SelectRows );
	ui->tableView->verticalHeader()->hide();
	ui->tableView->horizontalHeader()->setSectionResizeMode(
		0, QHeaderView::ResizeToContents );
	connect( ui->tableView, &QTableView::doubleClicked, this,
			 &PortionAdder::onTableClicked );
}
void PortionAdder::onTableClicked( const QModelIndex &index ) {
	qDebug() << "double clicked:" << index;
	if ( index.isValid() ) {
		const QAbstractItemModel *model = index.model();
		QVariant firstCell =
			model->data( model->index( index.row(), 0 ), Qt::DisplayRole );
		QString foodName = firstCell.toString();
		qDebug() << "food name is " << foodName;
		bool ok;
		int grams = QInputDialog::getInt( this, tr( "quantity of food" ),
										  "grams of " + foodName, 100, 0,
										  2147483647, 1, &ok );
		if ( ok ) {
			StorageManager::instance().addPortion(
				foodName, grams, ui->dateTimeEdit->dateTime() );
			ui->statusMessageLabel->setText( QString( "added %1 grams of %2" )
												 .arg( grams )
												 .arg( foodName ) );
		}
	}
}
PortionAdder::~PortionAdder() { delete ui; }
