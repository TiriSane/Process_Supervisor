#ifndef _TABEL_H_
#define _TABEL_H_

#include "QTableWidget"
#include "QMessageBox"

class Table : public QTableWidget
{
Q_OBJECT
public:
	Table();
	void NewDialog(int row);
};

#endif // !_TABEL_H_