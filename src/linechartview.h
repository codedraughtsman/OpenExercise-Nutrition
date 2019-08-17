#ifndef LINECHARTVIEW_H
#define LINECHARTVIEW_H

#include <QtCharts>

#include <QChartView>

class LineChartView : public QChartView {
  public:
	LineChartView( QWidget *parent = nullptr );
  public slots:
	void loadLineData();
};

#endif // LINECHARTVIEW_H
