#include "portion.h"
#include "storagemanager.h"

Portion::Portion() {}

Portion::Portion( QString foodName, double xValue, double yValue )
	: m_foodName( foodName ), m_xValue( xValue ), m_yValue( yValue ) {}
