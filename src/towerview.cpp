#include "towerview.h"

TowerView::TowerView( QWidget *parent, QString query, bool showXAxis )
	: QWidget( parent ), m_query( query ), m_showXAxis( showXAxis ) {
	updateView();
}
void TowerView::updateView() {}
