#include "linechartview.h"

#include <QLineSeries>

LineChartView::LineChartView( QWidget *parent ) : QChartView( parent ) {
	QLineSeries *series = new QLineSeries();
	series->append( 0, 6 );
	series->append( 2, 4 );

	chart()->addSeries( series );
	chart()->createDefaultAxes();
}
