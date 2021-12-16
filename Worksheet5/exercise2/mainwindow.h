#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "stockitemlistmodel.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    StockItemListModel stockList;
// Example of slot definition in mainwindow.h---------------------------
public slots:
void handleAddButton();
void handleEditButton();
void handleRemoveButton();
void handleSaveAction();
// ---------------------------------------------------------------------

// Example of signal definition in mainwindow.h-------------------------
signals:
	void statusUpdateMessage( const QString & message, int timeout );
	void actionSave( bool checked = false);
// ---------------------------------------------------------------------

};

#endif // MAINWINDOW_H