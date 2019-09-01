#ifndef LINECHARTVIEW_H
#define LINECHARTVIEW_H

#include <QtCharts>

#include <QChartView>
#include <QSqlQuery>

class LineChartView : public QChartView {
  public:
	LineChartView( QWidget *parent = nullptr );
  public slots:
	QLineSeries *loadLineData( QSqlQuery results );

  private:
	QVector<QSqlQuery> loadSQL();

	QVector<QLineSeries *> loadData();
	QSqlQuery runQuery( QString str );
};

#endif // LINECHARTVIEW_H
