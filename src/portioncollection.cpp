#include "portioncollection.h"
#include "storagemanager.h"

PortionCollection::PortionCollection() {}

void PortionCollection::addPortion( QString foodName, uint xValue,
									uint yValue ) {

	m_portions.append( Portion( foodName, xValue, yValue ) );
}

const QVector<Portion> &PortionCollection::getPortions() { return m_portions; }

uint PortionCollection::getMaxXValue() {
	uint maxValue = 0;
	for ( Portion p : m_portions ) {
		maxValue = qMax( maxValue, p.m_xValue );
	}
	return maxValue;
}

uint PortionCollection::getTotalYValue() {
	uint totalValue = 0;
	for ( Portion p : m_portions ) {
		totalValue += p.m_yValue;
	}
	return totalValue;
}
