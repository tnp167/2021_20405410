#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "edititemdialog.h"
#include "ui_edititemdialog.h"
#include "stockitem.h"
#include "stockitemlistmodel.h"
#include <QMessageBox>
#include <QFileDialog>
#include <QFile>
/*MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}*/

/*MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent),ui(new Ui::MainWindow) 
{
	ui->setupUi(this);
	connect( ui->addButton, &QPushButton::released, this, &MainWindow::handleAddButton );
}*/

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent),ui(new Ui::MainWindow)
 {
	ui->setupUi(this);
	// Link the ListModel to the ListView
	ui->listView->setModel( &stockList );
	// Tell this list view to only accept single selections
	ui->listView->setSelectionBehavior( QAbstractItemView::SelectItems );
	connect( ui->addButton, &QPushButton::released, this, &MainWindow::handleAddButton );
	connect( ui->editButton, &QPushButton::released, this, &MainWindow::handleEditButton );
	connect( ui->removeButton, &QPushButton::released, this, &MainWindow::handleRemoveButton );

	connect( this, &MainWindow::statusUpdateMessage, ui->statusBar, &QStatusBar::showMessage );
	
	connect( ui->actionSave, &QAction::triggered, this, &MainWindow::handleSaveAction);
	connect( ui->actionFileSave, &QAction::triggered, this, &MainWindow::handleSaveAction);
}


MainWindow::~MainWindow()
{
    delete ui;
}

/*
// Example of slot implementation in mainwindow.cpp---------------------
// (You'll also need to include <QMessageBox>)
void MainWindow::handleAddButton() 
{
	QMessageBox msgBox;
	msgBox.setText("Add button was clicked");
	msgBox.exec();
}
// ---------------------------------------------------------------------*/

// Example of slot implementation in mainwindow.cpp---------------------
void MainWindow::handleAddButton() 
{
	StockItem item;
	QModelIndexList selectedList;
	selectedList = ui->listView->selectionModel()->selectedIndexes();
	
	if( selectedList.length()==1){	
		stockList.insertItem( item, selectedList[0] );	
	}
	else{
		stockList.addItem( item );
	}	
	emit statusUpdateMessage( QString("Add button was clicked"), 0 );
}
// --------------------------------------------------------------------
void MainWindow::handleEditButton() {
	EditItemDialog dialog( this );
	QModelIndexList selectedList;
	selectedList = ui->listView->selectionModel()->selectedIndexes();
	if( selectedList.length() == 1 ) {
	// selectedList is a list of all selected items in the listView. Since we set its
	// behaviour to single selection, were only interested in the first selecteded item.
		StockItem item = stockList.getItem( selectedList[0] );
		if( dialog.runDialog( item ) ) {
	// user clicked ok, need to update item in list...
			stockList.setItem( item, selectedList[0] );
		}
		emit statusUpdateMessage( QString("Edit button was clicked"), 0 );
	} else {
		emit statusUpdateMessage( QString("No item selected to edit!"), 0 );
	}
}

void MainWindow::handleRemoveButton() {
	QModelIndexList selectedList;
	selectedList = ui->listView->selectionModel()->selectedIndexes();
	if( selectedList.length() == 1 ) {
	stockList.removeItem( selectedList[0] );}
	emit statusUpdateMessage( QString("Remove button was clicked"), 0 );
}

void MainWindow::handleSaveAction(){
	QString fileName = QFileDialog::getSaveFileName(this, tr("Save File"),
                           "C:/Users/iambitiouz/2021_20405410/Worksheet5/exercise2/Untitled.txt",
                           tr("Text files (*.txt)"));
    QFile file(fileName); 
	if (file.open(QFile::WriteOnly | QFile::Text)) {
    QTextStream s(&file);
    s<< "----------------------------List of items----------------------------\n";
    for (int i =0;i< stockList.rowCount();i++)
    {
    	s << "Item No. " << i+1 << " : Name: " << stockList.getItemName(i) << "     Unit cost: " << stockList.getItemUnitCost(i) << "     Stock level: "<< stockList.getItemStockLevel(i) <<"     Reorder: " ;
		if(stockList.getItemReorder(i))
		{
			s << "Yes\n";
		}
		else
		{
			s << "No\n";
		}
	}
  } 
	                   
}


