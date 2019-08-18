#include "linechartview.h"

#include <QDebug>
#include <QLineSeries>

LineChartView::LineChartView( QWidget *parent ) : QChartView( parent ) {
	QLineSeries *series = loadLineData();
	chart()->addSeries( series );

	QDateTimeAxis *axisX = new QDateTimeAxis;
	axisX->setTickCount( 10 );
	axisX->setFormat( "dd MMM" );
	axisX->setTitleText( "Date" );
	chart()->addAxis( axisX, Qt::AlignBottom );
	series->attachAxis( axisX );

	QValueAxis *axisY = new QValueAxis;
	axisY->setLabelFormat( "%i" );
	axisY->setTitleText( "Kj" );
	chart()->addAxis( axisY, Qt::AlignLeft );
	series->attachAxis( axisY );
}

QSqlQuery LineChartView::loadSQL() {
	QSqlQuery query;
	query.prepare( "select date, totalGrams, totalKj from portionsTotalPerDay "
				   "ORDER BY date DESC LIMIT 10" );
	query.exec();
	return query;
}

QLineSeries *LineChartView::loadLineData() {
	QLineSeries *series = new QLineSeries();
	QSqlQuery results = loadSQL();
	while ( results.next() ) {
		QString dateString = results.value( 0 ).toString();
		QDateTime date = QDateTime::fromString( dateString, "yyyy'-'MM'-'dd" );
		series->append( date.toMSecsSinceEpoch(), results.value( 2 ).toInt() );
		qDebug() << "dateString " << dateString << "date " << date

				 << " results.value( 2 ).toInt() "
				 << results.value( 2 ).toInt();
	}
	return series;
}
