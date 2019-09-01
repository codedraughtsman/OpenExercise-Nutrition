#include "linechartview.h"

#include <QDebug>
#include <QLineSeries>

LineChartView::LineChartView( QWidget *parent ) : QChartView( parent ) {
	QVector<QLineSeries *> seriesData = loadData();
	for ( QLineSeries *series : seriesData ) {
		chart()->addSeries( series );

		QDateTimeAxis *axisX = new QDateTimeAxis;
		axisX->setTickCount( 10 );
		axisX->setFormat( "dd MMM" );
		axisX->setTitleText( "Date" );
		chart()->addAxis( axisX, Qt::AlignBottom );
		series->attachAxis( axisX );

		QValueAxis *axisY = new QValueAxis;
		axisY->setLabelFormat( "%f" );
		axisY->setTitleText( "Kj" );
		chart()->addAxis( axisY, Qt::AlignLeft );
		series->attachAxis( axisY );
	}
}

QVector<QLineSeries *> LineChartView::loadData() {
	QVector<QSqlQuery> queryResults = loadSQL();
	QVector<QLineSeries *> series;
	for ( QSqlQuery queryResult : queryResults ) {
		series.push_back( loadLineData( queryResult ) );
	}
	return series;
}

QVector<QSqlQuery> LineChartView::loadSQL() {
	QVector<QSqlQuery> queryResults;
	queryResults.append(
		QSqlQuery( "select date, totalKj from portionsTotalPerDay "
				   "ORDER BY date DESC LIMIT 10" ) );
	queryResults.append(
		QSqlQuery( "select date, totalGrams from portionsTotalPerDay "
				   "ORDER BY date DESC LIMIT 10" ) );
	queryResults.append( runQuery( "select date, weight from weight "
								   "ORDER BY date DESC LIMIT 10" ) );

	return queryResults;
}
QSqlQuery LineChartView::runQuery( QString str ) {
	QSqlQuery query;
	query.prepare( str );

	bool sucessful = query.exec();

	if ( !sucessful ) {
		qDebug() << "query failed " << query.lastQuery();
	}
	return query;
}

QLineSeries *LineChartView::loadLineData( QSqlQuery results ) {
	QLineSeries *series = new QLineSeries();
	while ( results.next() ) {
		QString dateString = results.value( 0 ).toString();
		QDateTime date = QDateTime::fromString( dateString, "yyyy'-'MM'-'dd" );
		if ( results.value( 1 ).toString().contains( '.' ) ) {
			// this is a float
			series->append( date.toMSecsSinceEpoch(),
							results.value( 1 ).toDouble() );
			qDebug() << "dateString " << dateString << "date " << date
					 << " y value: " << results.value( 1 ).toDouble();
		} else {
			series->append( date.toMSecsSinceEpoch(),
							results.value( 1 ).toInt() );
			qDebug() << "dateString " << dateString << "date " << date
					 << " y value: " << results.value( 1 ).toInt();
		}
	}
	return series;
}
