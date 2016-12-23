#include "MainWindow.h"
#include "systemInfo.h"
#include <string>
#include <sstream>
#include <list>
#include <vector>
#include "QString"

#include "QAction"
#include "QMenuBar"
#include "QStatusBar"
#include "QInputDialog"
#include "QDir"

using namespace std;

MainWindow::MainWindow(QLabel *l, Table *t, QWidget *parent) : label(l),table(t),QMainWindow(parent)
{
	//Init Windows Name
	setWindowTitle(tr("Process Supervisor"));

	//Init timer
	timer = new QTimer(this);
	QObject::connect(timer, &QTimer::timeout, this, &MainWindow::updateData);
	timer->start(2000);

	//Init Actions
	auto openAction = new QAction(tr("&Run..."), this);
	openAction->setShortcuts(QKeySequence::Open);
	openAction->setStatusTip(tr("Run an Existing Software"));
	connect(openAction, &QAction::triggered, this, &MainWindow::runExec);

	QMenu *file = menuBar()->addMenu(tr("&File"));
	file->addAction(openAction);

	statusBar();
}

MainWindow::~MainWindow(){}

void MainWindow::updateData()
{
	///Top From
	int nCpuRate = -1;
	int nMemTotal = -1;
	int nMemUsed = -1;
	int nDiskTotal = -1;
	int nDiskUsed = -1;
	int nProcessMemRate = -1;

	GetSysCpu(nCpuRate);
	string temp("CPU Usage Rate:");
	string stemp;
	stringstream ss; ss << nCpuRate; ss >> stemp; ss.clear();
	temp += stemp; stemp.clear(); temp += "%       ";

	GetSysMemory(nMemTotal, nMemUsed);
	temp += "Memory Total:";
	ss << nMemTotal; ss >> stemp; ss.clear();
	temp += stemp; stemp.clear(); temp += "MB       Memory Used:";
	ss << nMemUsed; ss >> stemp; ss.clear();
	temp += stemp; stemp.clear(); temp += "MB\n";

	GetSysDisk(nDiskTotal, nDiskUsed);
	temp += "Disk Total:";
	ss << (nDiskTotal / 1024.0); ss >> stemp; ss.clear();
	temp += stemp; stemp.clear(); temp += "GB                         Disk Used:";
	ss << (nDiskUsed / 1024.0); ss >> stemp; ss.clear();
	temp += stemp; stemp.clear(); temp += "GB";

	QString str = QString::fromStdString(temp);
	label->setText(str);

	//From Inside
	list<vector<string>> pidMap = GetAllProcess();
	
	table->clearContents();

	int i = 0;
	for (auto &v:pidMap)
	{
		int j = 0;
		for (auto &s:v)
		{
			table->setItem(i, j, new QTableWidgetItem(s.c_str()));
			++j;
		}
		++i;

		if (i>=300)
		{
			break;
		}
	}
}

void MainWindow::runExec()
{
	bool ok = false;
	QString text = QInputDialog::getText(this, tr("Run a New Software"),tr("Please Enter a Complete Absolute Path:"), QLineEdit::Normal,QDir::home().dirName(), &ok);
	if (ok && !text.isEmpty())
	{
		string cmd = text.toStdString();
		WinExec(cmd.c_str(), SW_SHOW);
	}
}