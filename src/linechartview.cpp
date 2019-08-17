#include "linechartview.h"

#include <QLineSeries>

LineChartView::LineChartView( QWidget *parent ) : QChartView( parent ) {
	loadLineData();
	chart()->createDefaultAxes();
}

QSqlQuery LineChartView::loadSQL() {
	QSqlQuery query;
	return query;
}

void LineChartView::loadLineData() {
	QLineSeries *series = new QLineSeries();
	QSqlQuery results = loadSQL();
	series->append( 0, 6 );
	series->append( 2, 4 );

	chart()->addSeries( series );
}
