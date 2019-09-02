#ifndef TOWERWIDGET_H
#define TOWERWIDGET_H

#include "aggreator.h"
#include "portioncollection.h"

#include <QMap>
#include <QWidget>

class TowerWidget : public QWidget {
	Q_OBJECT
	QString m_xAxis, m_yAxis;

  public:
	TowerWidget( QWidget *parent = nullptr, QString xAxis = "kjPer100Grams",
				 QString yAxis = "totalKj" );

  private:
	QVector<PortionCollection> m_portions;
	QMap<QString, QColor> m_portionColors;

	double m_kjPerPixelWidth, m_kjPerPixelHeight;
	QColor m_axisColor;
	// uint getWidth();

	QColor getColor( QString id );
	void paintTower( QRectF area, PortionCollection &portions );
	void updateZoom( QRectF drawArea );
	void paintYAxis( QRectF area );
	void paintXAxis( QRectF area );
  signals:

  public slots:
	void reloadData();
	// QWidget interface
  public:
	virtual QSize sizeHint() const override;
	virtual QSize minimumSizeHint() const override;

	QVector<QDate> getLastNDates( uint n );

	PortionCollection loadPortion( QDate date );

	double convertKjToPixelsWidth( double kj );

	double convertKjToPixelsHeight( double kj );

  protected:
	virtual void paintEvent( QPaintEvent *event ) override;
};

#endif // TOWERWIDGET_H
