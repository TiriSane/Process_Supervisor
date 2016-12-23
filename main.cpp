#include "QApplication"
#include "QLabel"

#include "MainWindow.h"
#include "table.h"

int main(int argc, char *argv[])
{
	//Init App and App Window
    QApplication qtApp(argc, argv);

	//Show System Info Label
	QLabel infoLabel;
	infoLabel.setAlignment(Qt::AlignCenter);
	infoLabel.setFixedSize(1000,60);
	infoLabel.setText("CPU Usage Rate:0%           Memory Total:0MB           Memory Used:0MB\nDisk Total:0GB                             Disk Used:0GB");
	
	//Show Process Tabel
	Table mainTable;
	mainTable.setGeometry(0, 60, 1000, 660);

	MainWindow myWin(&infoLabel,&mainTable);
	mainTable.setParent(&myWin);
	infoLabel.setParent(&myWin);

	//Set Window Size
	myWin.setFixedSize(1000, 720);

	//Run
    myWin.show();
    return qtApp.exec();
}
