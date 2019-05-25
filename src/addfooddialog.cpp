#include "addfooddialog.h"
#include "storagemanager.h"
#include "ui_addfooddialog.h"

addFoodDialog::addFoodDialog( QWidget *parent )
	: QDialog( parent ), ui( new Ui::addFoodDialog ) {
	ui->setupUi( this );
	connect( ui->buttonBox, &QDialogButtonBox::accepted, this,
			 &QDialog::close );
}

addFoodDialog::~addFoodDialog() { delete ui; }

void addFoodDialog::accept() {
	StorageManager::instance().addFood( ui->foodNameEntry->text(),
										ui->kjPer100gEntry->text().toInt(),
										ui->gramsPerCupEntry->text().toInt() );
	this->close();
}

void addFoodDialog::reject() { addFoodDialog::close(); }

void addFoodDialog::on_buttonBox_accepted() { close(); }
