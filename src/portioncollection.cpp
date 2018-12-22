#include "portioncollection.h"
#include "storagemanager.h"

PortionCollection::PortionCollection() : m_maxKjPer100g( 0 ), m_totalKj( 0 ) {}
/*
PortionCollection::PortionCollection( PortionCollection &portion )
	: m_maxKjPer100g( portion.m_maxKjPer100g ), m_totalKj( portion.m_totalKj ),
	  m_portions( portion.m_portions ) {}
*/
void PortionCollection::addPortion( QString foodName, uint grams ) {
	m_portions.append( QPair<QString, uint>( foodName, grams ) );
	m_totalKj += StorageManager::getKjPer100g( foodName ) * grams;
	m_maxKjPer100g =
		qMax( m_maxKjPer100g, StorageManager::getKjPer100g( foodName ) );
}

const QVector<QPair<QString, uint>> &PortionCollection::getPortions() {
	return m_portions;
}

uint PortionCollection::getMaxKjPer100g() { return m_maxKjPer100g; }

uint PortionCollection::getTotalKj() { return m_totalKj; }
