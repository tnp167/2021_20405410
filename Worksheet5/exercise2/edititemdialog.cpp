#include "edititemdialog.h"
#include "ui_edititemdialog.h"

EditItemDialog::EditItemDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::EditItemDialog)
{
    ui->setupUi(this);
    connect( ui->buttonBox, &QDialogButtonBox::accepted, this, &QDialog::accept );
    connect( ui->buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject );
}
bool EditItemDialog::runDialog( StockItem & item ) {
	ui->name->setText( item.getName() );
	ui->unitCost->setValue( item.getUnitCost() );
	ui->stockLevel->setValue( item.getStockLevel() );
	ui->reorder->setChecked( item.getReorder() );
	if( this->exec() == QDialog::Accepted ) {
		item.setName( ui->name->text() );
		item.setUnitCost( ui->unitCost->value() );
		item.setStockLevel( ui->stockLevel->value() );
		item.setReorder( ui->reorder->isChecked() );
		return true;
	}
	return false;
}

EditItemDialog::~EditItemDialog()
{
    delete ui;
}
