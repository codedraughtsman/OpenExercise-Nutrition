#ifndef TOWERVIEW_H
#define TOWERVIEW_H

#include <QSqlQuery>
#include <QWidget>

class TowerView : public QWidget {
	Q_OBJECT
	QString m_query;
	bool m_showXAxis;

	void updateView();

	QSqlQuery loadData();

  public:
	explicit TowerView( QWidget *parent = nullptr, QString query = "",
						bool showXAxis = true );

  signals:

  public slots:

	// QWidget interface
  protected:
	virtual void paintEvent( QPaintEvent *event ) override;

	// QWidget interface
  public:
	virtual QSize sizeHint() const override;

	// QWidget interface
  public:
	virtual QSize minimumSizeHint() const override;
};

#endif // TOWERVIEW_H
