#include <string>
#include "table.h"
#include "cstdlib"

using namespace std;

Table::Table()
{
	//Init Table
	setRowCount(300);
	setColumnCount(4);

	setColumnWidth(0, 270);
	setColumnWidth(1, 210);
	setColumnWidth(2, 210);
	setColumnWidth(3, 210);

	//Init Header
	QStringList header;
	header << "Name" << "PID"<< "Memory Used"<<"Father ID";
	setHorizontalHeaderLabels(header);

	//Set No Edit Triggers
	setEditTriggers(QAbstractItemView::NoEditTriggers);
	setSelectionBehavior(QAbstractItemView::SelectRows);

	//Connect if Row Was Select
	connect(this,&Table::cellClicked,this,&Table::NewDialog);
}

void Table::NewDialog(int row)
{
	if (this->item(row, 0) != nullptr)
	{
		string questionString("Do You Want to Kill this Process?\n");
		questionString += item(row, 0)->text().toStdString();

		QMessageBox message(QMessageBox::NoIcon, "question", questionString.c_str(), QMessageBox::Yes | QMessageBox::No, nullptr);
		if (message.exec() == QMessageBox::Yes)
		{
			string pid = item(row, 1)->text().toStdString();

			string cmd("taskkill /F /PID ");
			cmd += pid;

			system(cmd.c_str());
		}
	}
}
