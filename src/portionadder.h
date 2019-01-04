#ifndef PORTIONADDER_H
#define PORTIONADDER_H

#include <QDialog>

namespace Ui {
class PortionAdder;
}

class PortionAdder : public QDialog {
	Q_OBJECT

  public:
	explicit PortionAdder( QWidget *parent = 0 );
	~PortionAdder();

  protected slots:
	void onTableClicked( const QModelIndex &index );

  private:
	Ui::PortionAdder *ui;
};

#endif // PORTIONADDER_H
