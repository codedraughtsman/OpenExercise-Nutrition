#ifndef PORTIONCOLLECTION_H
#define PORTIONCOLLECTION_H

#include "portion.h"

#include <QObject>
#include <QPair>
#include <QVector>

class PortionCollection {

	QVector<Portion> m_portions;

  public:
	explicit PortionCollection();
	void addPortion( QString foodName, double xValue, double yValue );
	const QVector<Portion> &getPortions();
	double getMaxXValue();
	double getTotalYValue();

  signals:

  public slots:
};

#endif // PORTIONCOLLECTION_H
