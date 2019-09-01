#include "towerwidget.h"
#include "portioncollection.h"
#include "storagemanager.h"

#include <QDate>
#include <QDebug>
#include <QPaintEvent>
#include <QPainter>
#include <QRandomGenerator>
#include <QSqlDriver>
#include <QSqlError>
#include <QSqlQuery>
#include <QSqlRecord>

#define DATE_FORMAT "yyyy-MM-dd"
#define DATE_FORMAT_INPUT "yyyy'-'MM'-'dd"

TowerWidget::TowerWidget( QWidget *parent )
	: QWidget( parent ), m_axisColor( Qt::blue ) {
	QSqlDatabase db = QSqlDatabase::database();
	db.driver()->subscribeToNotification( "someEventId" );
	connect( &StorageManager::instance(), &StorageManager::dataChanged, this,
			 &TowerWidget::reloadData );
	reloadData();
}

QSize TowerWidget::sizeHint() const { return QSize( 300, 600 ); }

QSize TowerWidget::minimumSizeHint() const { return QSize( 150, 300 ); }

QVector<QDate> TowerWidget::getLastNDates( uint n ) {
	QSqlQuery query;
	query.prepare( "select distinct portionsPerDayPerFood.date from "
				   "portionsPerDayPerFood limit ?" );
	query.addBindValue( n );
	query.exec();
	QVector<QDate> dates;
	while ( query.next() ) {
		QString dateString = query.value( 0 ).toString();
		dates.append( QDate::fromString( dateString, DATE_FORMAT_INPUT ) );
	}
	return dates;
}

PortionCollection TowerWidget::loadPortion( QDate date ) {
	QSqlQuery query;

	query.prepare( "select foodName, " //(select kjPer100g from food where
									   // foodName=foodName)"
				   "sum( grams ) total from  (SELECT * FROM "
				   "portions WHERE timestamp LIKE ?)  group by foodName order "
				   "by total DESC" );
	query.addBindValue( date.toString( DATE_FORMAT ) + "%" );

	bool sucessful = query.exec();

	if ( !sucessful ) {
		qDebug() << "failed to load portions";
	}

	PortionCollection portions;
	while ( query.next() ) {
		QString foodName = query.value( 0 ).toString();
		uint grams = query.value( 1 ).toUInt();
		uint kj = StorageManager::getKjPer100g( foodName ) * grams / 100;
		// qDebug() << "foodName" << foodName << " grams " << grams << ", kj "
		//		 << kj;
		portions.addPortion( foodName, grams );
	}
	return portions;
}
void TowerWidget::reloadData() {
	// load the last 7 days of portions.
	m_portions.clear();
	for ( QDate date : getLastNDates( 7 ) ) {
		qDebug() << "reloading date:" << date.toString();
		m_portions.append( loadPortion( date ) );
	}
	// redraw the screen.
	this->repaint();
}

QColor TowerWidget::getColor( QString id ) {
	if ( !m_portionColors.contains( id ) ) {
		m_portionColors[ id ] =
			QColor::fromRgb( QRandomGenerator::global()->generate() );
	}

	return m_portionColors[ id ];
}

void TowerWidget::paintTower( QRectF area, PortionCollection &portions ) {
	// for each portion draw a rectangle.
	// width is the kj per 100 grams.
	// height is total kj in portion.
	float startHeight = area.height();
	QPainter painter( this );

	// draws outline around the tower.
	painter.drawRect( area );

	painter.setRenderHint( QPainter::Antialiasing );
	float penWidth = 2;
	QPen pen( Qt::black, penWidth );
	painter.setPen( pen );
	for ( Portion portion : portions.getPortions() ) {
		QString foodName = portion.getFoodName();
		uint kj = portion.getKj();
		float drawHeight = convertKjToPixelsHeight( kj );
		float drawWidth =
			convertKjToPixelsWidth( StorageManager::getKjPer100g( foodName ) ) -
			penWidth;
		/*qDebug() << "\nfoodName" << foodName;
		qDebug() << "draw width" << drawWidth << "getKjPer100g( foodName )"
				 << StorageManager::getKjPer100g( foodName )
				 << "m_kjPerPixelYAxis" << m_kjPerPixelHeight;
		qDebug() << "draw height:" << drawHeight << "kj" << kj << "startHeight"
				 << startHeight;
		*/
		QPainterPath path;
		QRectF rect( area.left() + penWidth * 2, startHeight - drawHeight,
					 drawWidth, drawHeight );
		path.addRoundedRect( rect, 10, 10 );

		painter.fillPath( path, getColor( foodName ) );
		painter.drawPath( path );

		// painter.save();
		// painter.rotate( -90 );
		uint textWidth = painter.fontMetrics().width( foodName );
		if ( textWidth < rect.width() ) {
			// draw inside rect.
			painter.drawText( rect, Qt::AlignHCenter | Qt::AlignVCenter,
							  foodName );
		} else {
			// draw outside rect.
			QRect outsideRect( rect.right(), rect.top(),
							   area.width() - rect.width(), rect.height() );
			painter.drawText( outsideRect, Qt::AlignHCenter | Qt::AlignVCenter,
							  foodName );
		}
		// painter.restore();

		startHeight -= drawHeight;
	}
}

void TowerWidget::updateZoom( QRectF drawArea ) {
	uint maxKjTower = 0, totalKjWidth = 0;
	for ( PortionCollection portions : m_portions ) {
		// qDebug() << "portions.getTotalKj() " << portions.getTotalKj();
		if ( portions.getTotalKj() > maxKjTower ) {
			maxKjTower = portions.getTotalKj();
		}
		totalKjWidth += portions.getMaxKjPer100g();
	}
	/*qDebug() << "maxKjTower" << maxKjTower << "m_portions" <<
	m_portions.size();
	qDebug() << "drawArea.height()" << drawArea.height() << "drawArea.width()"
			 << drawArea.width();
	*/
	// m_kjPerPixelHeight = qMax( 4000u, maxKjTower ) / drawArea.height();
	m_kjPerPixelHeight = maxKjTower / drawArea.height();
	qDebug() << "m_kjPerPixelHeight" << m_kjPerPixelHeight;

	m_kjPerPixelWidth = totalKjWidth / ( drawArea.width() -
										 ( 10.0 * ( m_portions.size() ) - 1 ) );
	/*qDebug() << "m_kjPerPixelWidth" << m_kjPerPixelWidth << "totalKjWidth"
			 << totalKjWidth;
			 */
}

float TowerWidget::convertKjToPixelsWidth( uint kj ) {
	return kj / m_kjPerPixelWidth;
}

float TowerWidget::convertKjToPixelsHeight( uint kj ) {
	return kj / m_kjPerPixelHeight;
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
		  area.height() > convertKjToPixelsHeight( kjGrad * gradSize );
		  kjGrad++ ) {
		uint totalKj = kjGrad * gradSize;
		float lineY = area.height() - convertKjToPixelsHeight( totalKj );

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
	uint gradSize = 100;
	for ( uint kjGrad = 1;
		  area.width() > convertKjToPixelsWidth( kjGrad * gradSize );
		  kjGrad++ ) {
		uint totalKj = kjGrad * gradSize;
		float lineX = area.left() + convertKjToPixelsWidth( totalKj );

		uint lineLength = 4;
		if ( totalKj % 500 == 0 ) {
			lineLength = 10;
			// painter.drawText( lineX, area.top() + lineLength,
			//				  QString::number( totalKj ) );
		}

		painter.drawLine( lineX, area.top(), lineX, area.top() + lineLength );
	}
}
void TowerWidget::paintEvent( QPaintEvent *event ) {
	QRect area = event->rect();
	qDebug() << "called paint event";
	uint xAxisMargin = 40, yAxisMargin = 30;
	QPointF origin( area.top() - xAxisMargin, area.bottom() - yAxisMargin );
	QRectF towerRect( xAxisMargin, 5, area.width() - xAxisMargin - 5,
					  area.height() - yAxisMargin - 5 );

	updateZoom( towerRect );
	qreal towerXStart = xAxisMargin;
	for ( PortionCollection portion : m_portions ) {
		qreal towerWidth = convertKjToPixelsWidth( portion.getMaxKjPer100g() );
		QRectF towerRect( towerXStart, 0, towerWidth,
						  area.height() - yAxisMargin );
		paintTower( towerRect, portion );
		towerXStart += towerWidth + 10;
	}
	paintYAxis( QRectF( 0, 0, xAxisMargin, area.height() - yAxisMargin ) );
	paintXAxis( QRectF( xAxisMargin, area.height() - yAxisMargin,
						area.width() - xAxisMargin, area.height() ) );
}
