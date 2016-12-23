#ifndef _MAINWINDOW_H_
#define _MAINWINDOW_H_

#include <QMainWindow>
#include "table.h"
#include "QTimer"
#include "QLabel"

class MainWindow : public QMainWindow
{
Q_OBJECT
public:
	MainWindow(QLabel *l, Table *t, QWidget *parent = 0);
    ~MainWindow();

	void updateData();
private:
	QTimer *timer;
	QLabel *label;
	Table *table;
};

#endif // _MAINWINDOW_H_