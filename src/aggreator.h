#ifndef AGGREATOR_H
#define AGGREATOR_H

#include <QObject>

class Aggreator : public QObject {
	Q_OBJECT
  public:
	explicit Aggreator( QObject *parent = nullptr );
	void add( QString key, uint value );
  signals:

  public slots:
};

#endif // AGGREATOR_H
