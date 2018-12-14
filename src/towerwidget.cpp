#include "towerwidget.h"

#include <QDate>
#include <QDebug>
#include <QPaintEvent>
#include <QPainter>
#include <QSqlError>
#include <QSqlQuery>
#include <QSqlRecord>

#define DATE_FORMAT "yyyy-MM-dd"
#define DATE_FORMAT_INPUT "yyyy'-'MM'-'dd"

TowerWidget::TowerWidget( QWidget *parent )
	: QWidget( parent ), m_totalKj( 0 ), m_axisColor( Qt::blue ) {
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
// select foodName,sum( grams ) total from  (SELECT * FROM portions WHERE
// timestamp LIKE '2018-12-12%')  group by foodName order by total DESC

void TowerWidget::reloadData() {

	// get the latest data from the database.
	QDate lastDate = getLastDate();

	// get the data for the last date.
	QSqlQuery query;

	query.prepare( "select foodName,sum( grams ) total from  (SELECT * FROM "
				   "portions WHERE timestamp LIKE ?)  group by foodName order "
				   "by total DESC" );
	query.addBindValue( lastDate.toString( DATE_FORMAT ) + "%" );

	bool sucessful = query.exec();
	qDebug() << "last date is: " << lastDate.toString( DATE_FORMAT )
			 << " sucessful " << sucessful;
	qDebug() << "running:" << query.lastQuery() << query.executedQuery() << ", "
			 << query.lastError();

	qDebug() << "nunber of rows: " << query.size(); // does not support size.
	m_totalKj = 0;
	while ( query.next() ) {
		qDebug() << query.value( 0 ) << query.value( 1 )
				 << "query.value( 1 ).toUInt()" << query.value( 1 ).toUInt();
		m_aggeratedPortions.append( QPair<QString, uint>(
			query.value( 0 ).toString(), query.value( 1 ).toUInt() ) );
		m_totalKj += query.value( 1 ).toUInt();
	}
}

void TowerWidget::updateTower() {}

QColor TowerWidget::getColor( QString id ) {
	if ( id == "apple" ) {
		return Qt::blue;
	} else if ( id == "tomato" ) {
		return Qt::green;
	} else if ( id == "banna" ) {
		return Qt::yellow;
	}
	return Qt::red;
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
	for ( auto portion : m_aggeratedPortions ) {
		uint kj = portion.second;
		float drawHeight = kj / m_kjPerPixel;
		float drawWidth = 80 - penWidth;
		qDebug() << "draw height:" << drawHeight << "kj" << kj << "startHeight"
				 << startHeight;
		QPainterPath path;
		QRectF rect( area.left() + penWidth * 2, startHeight - drawHeight,
					 drawWidth, drawHeight );
		path.addRoundedRect( rect, 10, 10 );

		painter.fillPath( path, getColor( portion.first ) );
		painter.drawPath( path );

		// painter.save();
		// painter.rotate( -90 );
		painter.drawText( rect, Qt::AlignHCenter | Qt::AlignVCenter,
						  portion.first );
		// painter.restore();

		startHeight -= drawHeight;
	}
}

void TowerWidget::updateZoom( QRectF drawArea ) {
	m_kjPerPixel = qMax( 4000u, m_totalKj ) / drawArea.height();
}

void TowerWidget::paintYAxis( QRectF area ) {
	QPainter painter( this );
	painter.setRenderHint( QPainter::Antialiasing );
	// draw vertical line.
	QPen pen( m_axisColor, 1 );
	painter.setPen( pen );
	painter.drawLine( area.topRight(), area.bottomRight() );
	uint gradSize = 100;
	for ( uint kjGrad = 1; area.height() > ( kjGrad * gradSize ) / m_kjPerPixel;
		  kjGrad++ ) {
		uint totalKj = kjGrad * gradSize;
		float lineY = area.height() - totalKj / m_kjPerPixel;

		uint lineLength = 4;
		if ( totalKj % 1000 == 0 ) {
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
