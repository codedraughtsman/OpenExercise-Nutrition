#ifndef ADDFOODDIALOG_H
#define ADDFOODDIALOG_H

#include <QDialog>

namespace Ui {
class addFoodDialog;
}

class addFoodDialog : public QDialog {
	Q_OBJECT

  public:
	explicit addFoodDialog( QWidget *parent = 0 );
	~addFoodDialog();

  private:
	Ui::addFoodDialog *ui;

	// QDialog interface
  public slots:
	virtual void accept() override;
	virtual void reject() override;
  private slots:
	void on_buttonBox_accepted();
};

#endif // ADDFOODDIALOG_H
