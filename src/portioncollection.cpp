#include "portioncollection.h"
#include "storagemanager.h"

PortionCollection::PortionCollection() {}

void PortionCollection::addPortion( QString foodName, double xValue,
									double yValue ) {

	m_portions.append( Portion( foodName, xValue, yValue ) );
}

const QVector<Portion> &PortionCollection::getPortions() { return m_portions; }

double PortionCollection::getMaxXValue() {
	double maxValue = 0;
	for ( Portion p : m_portions ) {
		maxValue = qMax( maxValue, p.m_xValue );
	}
	return maxValue;
}

double PortionCollection::getTotalYValue() {
	double totalValue = 0;
	for ( Portion p : m_portions ) {
		totalValue += p.m_yValue;
	}
	return totalValue;
}
