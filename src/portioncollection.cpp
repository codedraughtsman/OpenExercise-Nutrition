#include "portioncollection.h"
#include "storagemanager.h"

PortionCollection::PortionCollection() : m_maxKjPer100g( 0 ), m_totalKj( 0 ) {}

void PortionCollection::addPortion( QString foodName, uint grams ) {

	m_portions.append( Portion( foodName, grams ) );
	m_totalKj += ( StorageManager::getKjPer100g( foodName ) * grams ) / 100;
	m_maxKjPer100g =
		qMax( m_maxKjPer100g, StorageManager::getKjPer100g( foodName ) );
}

const QVector<Portion> &PortionCollection::getPortions() { return m_portions; }

uint PortionCollection::getMaxKjPer100g() { return m_maxKjPer100g; }

uint PortionCollection::getTotalKj() { return m_totalKj; }
