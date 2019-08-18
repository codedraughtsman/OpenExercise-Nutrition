#ifndef LINECHARTVIEW_H
#define LINECHARTVIEW_H

#include <QtCharts>

#include <QChartView>
#include <QSqlQuery>

class LineChartView : public QChartView {
  public:
	LineChartView( QWidget *parent = nullptr );
  public slots:
	QLineSeries *loadLineData();

  private:
	QSqlQuery loadSQL();
};

#endif // LINECHARTVIEW_H
