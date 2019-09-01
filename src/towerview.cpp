#include "towerview.h"
#include <QDebug>
#include <QPaintEvent>
#include <QPainter>

TowerView::TowerView( QWidget *parent, QString query, bool showXAxis )
	: QWidget( parent ), m_query( query ), m_showXAxis( showXAxis ) {
	updateView();
}
void TowerView::updateView() { QSqlQuery data = loadData(); }

QSqlQuery TowerView::loadData() { return QSqlQuery(); }

void TowerView::paintEvent( QPaintEvent *event ) {
	QPainter painter( this );
	painter.fillRect( event->rect(), QColor( 255, 0, 0 ) );
}

QSize TowerView::sizeHint() const {
	return QSize( parentWidget()->size().width(), 400 );
}

QSize TowerView::minimumSizeHint() const {
	return QSize( parentWidget()->size().width(), 400 );
}
