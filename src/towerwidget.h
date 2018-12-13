#ifndef TOWERWIDGET_H
#define TOWERWIDGET_H

#include "aggreator.h"

#include <QWidget>

class TowerWidget : public QWidget {
	Q_OBJECT
  public:
	TowerWidget( QWidget *parent = nullptr );

  private:
	QVector<QPair<QString, uint>> m_aggeratedPortions;
	uint m_totalKj;
	float m_kjPerPixel;
	QColor m_axisColor;
	// uint getWidth();

	QDate getLastDate();
	void reloadData();
	QColor getColor( QString id );
	void paintTower( QRectF area );
	void updateZoom( QRectF drawArea );
	void paintYAxis( QRectF area );
	void paintXAxis( QRectF area );
  signals:

  public slots:

	// QWidget interface
  public:
	virtual QSize sizeHint() const override;
	virtual QSize minimumSizeHint() const override;

	void updateTower();

  protected:
	virtual void paintEvent( QPaintEvent *event ) override;
};

#endif // TOWERWIDGET_H
