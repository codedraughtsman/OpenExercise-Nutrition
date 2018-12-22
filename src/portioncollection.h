#ifndef PORTIONCOLLECTION_H
#define PORTIONCOLLECTION_H

#include "portion.h"

#include <QObject>
#include <QPair>
#include <QVector>

class PortionCollection {

	QVector<Portion> m_portions;
	uint m_maxKjPer100g, m_totalKj;

  public:
	explicit PortionCollection();
	// PortionCollection( PortionCollection &portion );
	void addPortion( QString foodName, uint grams );
	const QVector<Portion> &getPortions();
	uint getMaxKjPer100g();
	uint getTotalKj();
  signals:

  public slots:
};

#endif // PORTIONCOLLECTION_H
