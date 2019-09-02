#include "portion.h"
#include "storagemanager.h"

Portion::Portion() {}

Portion::Portion( QString foodName, uint xValue, uint yValue )
	: m_foodName( foodName ), m_xValue( xValue ), m_yValue( yValue ) {}
