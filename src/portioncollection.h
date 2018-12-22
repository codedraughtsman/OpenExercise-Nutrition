#ifndef PORTIONCOLLECTION_H
#define PORTIONCOLLECTION_H

#include <QObject>
#include <QPair>
#include <QVector>

class PortionCollection {

	QVector<QPair<QString, uint>> m_portions;
	uint m_maxKjPer100g, m_totalKj;

  public:
	explicit PortionCollection();
	// PortionCollection( PortionCollection &portion );
	void addPortion( QString foodName, uint grams );
	const QVector<QPair<QString, uint>> &getPortions();
	uint getMaxKjPer100g();
	uint getTotalKj();
  signals:

  public slots:
};

#endif // PORTIONCOLLECTION_H
