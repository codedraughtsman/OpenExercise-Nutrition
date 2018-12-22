#include "portion.h"
#include "storagemanager.h"

Portion::Portion() {}

Portion::Portion( QString foodName, uint grams )
	: m_foodName( foodName ), m_grams( grams ) {

	m_kj = ( StorageManager::getKjPer100g( foodName ) * grams ) / 100;
}

QString Portion::getFoodName() { return m_foodName; }

float Portion::getKj() { return m_kj; }
