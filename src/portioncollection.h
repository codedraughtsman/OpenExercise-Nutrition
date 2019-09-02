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
	void addPortion( QString foodName, uint xValue, uint yValue );
	const QVector<Portion> &getPortions();
	uint getMaxXValue();
	uint getTotalYValue();

  signals:

  public slots:
};

#endif // PORTIONCOLLECTION_H
