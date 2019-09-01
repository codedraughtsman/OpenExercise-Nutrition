#ifndef TOWERVIEW_H
#define TOWERVIEW_H

#include <QWidget>

class TowerView : public QWidget {
	Q_OBJECT
	QString m_query;
	bool m_showXAxis;

	void updateView();

  public:
	explicit TowerView( QWidget *parent = nullptr, QString query = "",
						bool showXAxis = true );

  signals:

  public slots:
};

#endif // TOWERVIEW_H
