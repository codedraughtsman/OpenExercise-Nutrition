#include "towerwidget.h"
#include "portioncollection.h"
#include "storagemanager.h"

#include <QDate>
#include <QDebug>
#include <QPaintEvent>
#include <QPainter>
#include <QRandomGenerator>
#include <QSqlError>
#include <QSqlQuery>
#include <QSqlRecord>

#define DATE_FORMAT "yyyy-MM-dd"
#define DATE_FORMAT_INPUT "yyyy'-'MM'-'dd"

TowerWidget::TowerWidget( QWidget *parent )
	: QWidget( parent ), m_axisColor( Qt::blue ) {
	reloadData();
}

QSize TowerWidget::sizeHint() const { return QSize( 300, 600 ); }

QSize TowerWidget::minimumSizeHint() const { return QSize( 150, 300 ); }

QDate TowerWidget::getLastDate() {
	// latest day that contains an entry.
	QSqlQuery query;
	query.exec( "SELECT MAX(timestamp) FROM portions;" );
	query.first(); // select the first valid record.

	QString dateString = query.value( 0 ).toString().section( " ", 0, 0 );
	QDate lastDate = QDate::fromString( dateString, DATE_FORMAT_INPUT );
	qDebug() << "last date is: " << lastDate.toString( DATE_FORMAT );
	return lastDate;
}
QVector<QDate> TowerWidget::getLastNDates( uint n ) {
	QSqlQuery query;
	query.prepare( "SELECT DISTINCT strftime('%Y-%m-%d',timestamp) FROM "
				   "portions ORDER BY timestamp DESC LIMIT ?" );
	query.addBindValue( n );
	query.exec();
	QVector<QDate> dates;
	while ( query.next() ) {
		QString dateString = query.value( 0 ).toString();
		dates.append( QDate::fromString( dateString, DATE_FORMAT_INPUT ) );
	}
	return dates;
}
// select foodName,sum( grams ) total from  (SELECT * FROM portions WHERE
// timestamp LIKE '2018-12-12%')  group by foodName order by total DESC

PortionCollection TowerWidget::loadPortion( QDate date ) {
	QSqlQuery query;

	query.prepare( "select foodName,sum( grams ) total from  (SELECT * FROM "
				   "portions WHERE timestamp LIKE ?)  group by foodName order "
				   "by total DESC" );
	query.addBindValue( date.toString( DATE_FORMAT ) + "%" );

	bool sucessful = query.exec();

	if ( !sucessful ) {
		qDebug() << "failed to load portions";
	}

	PortionCollection portions;
	while ( query.next() ) {
		qDebug() << query.value( 0 ) << query.value( 1 )
				 << "query.value( 1 ).toUInt()" << query.value( 1 ).toUInt();
		portions.addPortion( query.value( 0 ).toString(),
							 query.value( 1 ).toUInt() );
	}
	return portions;
}
void TowerWidget::reloadData() {
	// load the last 7 days of portions.
	m_portions.clear();
	for ( QDate date : getLastNDates( 7 ) ) {

		m_portions.append( loadPortion( date ) );
	}
}

void TowerWidget::updateTower() {}

QColor TowerWidget::getColor( QString id ) {
	if ( !m_portionColors.contains( id ) ) {
		m_portionColors[ id ] =
			QColor::fromRgb( QRandomGenerator::global()->generate() );
	}

	return m_portionColors[ id ];
}

void TowerWidget::paintTower( QRectF area ) {
	// for each portion draw a rectangle.
	// width is the kj per 100 grams.
	// height is total kj in portion.
	float startHeight = area.height();
	QPainter painter( this );
	painter.setRenderHint( QPainter::Antialiasing );
	float penWidth = 2;
	QPen pen( Qt::black, penWidth );
	painter.setPen( pen );
	for ( auto portion : m_portions[ 0 ].getPortions() ) {
		QString foodName = portion.first;
		uint kj = portion.second;
		float drawHeight = kj / m_kjPerPixelXAxis;
		float drawWidth = ( StorageManager::getKjPer100g( foodName ) /
							( m_kjPerPixelYAxis * 100 ) ) -
						  penWidth;
		qDebug() << "draw width" << drawWidth << "getKjPer100g( foodName )"
				 << StorageManager::getKjPer100g( foodName )
				 << "m_kjPerPixelYAxis" << m_kjPerPixelYAxis;
		qDebug() << "draw height:" << drawHeight << "kj" << kj << "startHeight"
				 << startHeight;
		QPainterPath path;
		QRectF rect( area.left() + penWidth * 2, startHeight - drawHeight,
					 drawWidth, drawHeight );
		path.addRoundedRect( rect, 10, 10 );

		painter.fillPath( path, getColor( foodName ) );
		painter.drawPath( path );

		// painter.save();
		// painter.rotate( -90 );
		painter.drawText( rect, Qt::AlignHCenter | Qt::AlignVCenter, foodName );
		// painter.restore();

		startHeight -= drawHeight;
	}
}

void TowerWidget::updateZoom( QRectF drawArea ) {
	m_kjPerPixelXAxis = qMax( 4000u, 0u ) / drawArea.height();
	m_kjPerPixelYAxis = 0.01; // 1000 / drawArea.width();
}

void TowerWidget::paintYAxis( QRectF area ) {
	QPainter painter( this );
	painter.setRenderHint( QPainter::Antialiasing );
	// draw vertical line.
	QPen pen( m_axisColor, 1 );
	painter.setPen( pen );
	painter.drawLine( area.topRight(), area.bottomRight() );
	uint gradSize = 100;
	for ( uint kjGrad = 1;
		  area.height() > ( kjGrad * gradSize ) / m_kjPerPixelXAxis;
		  kjGrad++ ) {
		uint totalKj = kjGrad * gradSize;
		float lineY = area.height() - totalKj / m_kjPerPixelXAxis;

		uint lineLength = 4;
		if ( totalKj % 500 == 0 ) {
			lineLength = 10;
			painter.drawText( area.left(), lineY, QString::number( totalKj ) );
		}

		painter.drawLine( area.right(), lineY, area.right() - lineLength,
						  lineY );
	}
}
void TowerWidget::paintXAxis( QRectF area ) {
	QPainter painter( this );
	painter.setRenderHint( QPainter::Antialiasing );

	QPen pen( m_axisColor, 1 );
	painter.setPen( pen );
	painter.drawLine( area.topLeft(), area.topRight() );
}
void TowerWidget::paintEvent( QPaintEvent *event ) {
	qDebug() << "called paint event";
	uint xAxisMargin = 40, yAxisMargin = 30;
	updateTower();
	updateZoom( event->rect() );
	QRectF towerRect( xAxisMargin, 0, event->rect().width() - xAxisMargin,
					  event->rect().height() - yAxisMargin );
	QPainter painter( this );
	painter.drawRect( towerRect );
	paintTower( towerRect );
	paintYAxis(
		QRectF( 0, 0, xAxisMargin, event->rect().height() - yAxisMargin ) );
	paintXAxis( QRectF( xAxisMargin, event->rect().height() - yAxisMargin,
						event->rect().width() - xAxisMargin,
						event->rect().height() ) );
}
