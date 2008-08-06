#include "tray_menu.h"

#include <QDir>

TrayMenu::TrayMenu() :
	QSystemTrayIcon(0) {

	/*---------------
	 MENU
	 ---------------*/
	forceLogAct = new QAction("Log activity", this);
	autoLogAct = new QAction("&Automatic log", this);
	showAct = new QAction("Show Tasklist", this);
	timeLineAct = new QAction("Timeline", this);
	reportAct = new QAction("Report", this);
	saveAct = new QAction("Save",this);
	showSettingsAct = new QAction("Settings", this);
	showSettingsAct->setIcon(QIcon(":/icons/puzzle.png"));
	quitAct = new QAction("&Quit", this);
	quitAct->setIcon(QIcon(":/icons/stop.png"));

	autoLogAct->setCheckable(true);
	QFont italic;
	italic.setStyle(QFont::StyleItalic);
	quitAct->setFont(italic);

	trayMenu = new QMenu(0);
	trayMenu->addAction(forceLogAct);
	trayMenu->addAction(autoLogAct);
	trayMenu->addSeparator();
	trayMenu->addAction(showAct);
	trayMenu->addAction(timeLineAct);
	trayMenu->addAction(reportAct);
	trayMenu->addAction(saveAct);
	trayMenu->addAction(showSettingsAct);
	trayMenu->addSeparator();
	trayMenu->addAction(quitAct);
	trayMenu->setDefaultAction(forceLogAct);
	setContextMenu(trayMenu);

	/*-----------------
	 Internal objects
	 -----------------*/
	settings = new QSettings("settings.ini", QSettings::IniFormat);

	askReportDate = new AskDateInterval();
	taskList = new TaskList(settings);
	log = new Log(taskList, settings);
	taskWdg = new TasksWdg(taskList, log, settings, 0);
	itw = new InputTimeWnd(taskList, log, settings, 0);
	settingsDialog = new Settings(settings, log, itw, taskList, taskWdg, this);
	timeLine = new TimeLine(log, taskList);

	/*-----------------
	 Configuration
	 -----------------*/
	readSettings();

	QObject::connect(this, SIGNAL(activated(QSystemTrayIcon::ActivationReason)), this, SLOT(handleActivated(QSystemTrayIcon::ActivationReason)));
	QObject::connect(reportAct, SIGNAL(triggered()), askReportDate, SLOT(show()));
	QObject::connect(askReportDate, SIGNAL(datesSetted(QDate,QDate,bool)), this, SLOT(writeReport(QDate,QDate)));
	QObject::connect(quitAct, SIGNAL(triggered()), this, SIGNAL(quit()));
	QObject::connect(saveAct, SIGNAL(triggered()), this, SLOT(saveLog()));
	QObject::connect(showAct, SIGNAL(triggered()), taskWdg, SLOT(show()));
	QObject::connect(showSettingsAct, SIGNAL(triggered()), settingsDialog, SLOT(show()));
	QObject::connect(forceLogAct, SIGNAL(triggered()), this, SLOT(showItw()));
	QObject::connect(autoLogAct, SIGNAL(toggled(bool)), itw, SLOT(setAskForInput(bool)));
	QObject::connect(autoLogAct, SIGNAL(toggled(bool)), this, SLOT(setEnabledIcon(bool)));
	QObject::connect(timeLineAct, SIGNAL(triggered()), timeLine, SLOT(show()));

}

void TrayMenu::readSettings() {
	bool autoStart = settings->value("itw/autoLogOnStart", true).toBool();
	setEnabledIcon(autoStart);
	autoLogAct->setChecked(autoStart);
}

void TrayMenu::writeSettings() {

}

TrayMenu::~TrayMenu() {
	delete trayMenu;
	delete log;
	delete taskList;
	delete taskWdg;
	delete itw;
	delete askReportDate;
	writeSettings();
	delete settings;
}

void TrayMenu::setEnabledIcon(bool green) {

	const QString enabledIcon(":/icons/iconOn.png");
	const QString disabledIcon(":/icons/icon.png");

	QString iconPath;
	if (green) {
		iconPath = enabledIcon;
	} else {
		iconPath = disabledIcon;
	}
	QIcon icon(iconPath);
	setIcon(icon);

}

void TrayMenu::writeReport(QDate beginDate, QDate endDate) {

	QList<QDate> dates;
	QDate lastDate;

	QList<QString> data;
	QString dataLine;

	task_id lastTask = 0;
	int lastDateIndex = 0;
	int dateIndex;

	//Get the report's data between the specified fields
	Report reporte = log->getReport(&beginDate, &endDate);
	if (beginDate.isNull() || endDate.isNull()) {
		QMessageBox::information(itw, tr("Report"), tr(
				"There's nothing to report on these dates"));
		return;
	}

	/*---------------
	 Create datalines
	 ---------------*/
	for (Report::const_iterator i = reporte.begin(); i != reporte.end(); ++i) {
		//On task change append last line.
		if (lastTask != (*i).first.task) {
			if (dataLine != "") {
				data << dataLine;
			}
			dataLine = taskList->getTask((*i).first.task).getDescription()
					+ ";";
			lastDateIndex = 0;
		}

		dateIndex = beginDate.daysTo((*i).first.date);
		//We need to calculate the position of the new entry
		int gap = dateIndex - lastDateIndex;
		for (; gap != 0; --gap) {
			dataLine = dataLine + ";";
		}
		//New entry
		dataLine = dataLine + QString::number((*i).second, 'f', 2);

		lastDateIndex = dateIndex;
		lastTask = (*i).first.task;

	}
	//We print the last line
	data << dataLine;

	/*---------------
	 We print all the dates
	 ---------------*/
	QString header;
	int gap = beginDate.daysTo(endDate) + 1;
	for (int i = 0; i < gap; ++i) {
		header = header + ";" + beginDate.addDays(i).toString("dd/MM/yyyy");
	}

	/*---------------
	 Write the file
	 ---------------*/
	//Select file
	QFileDialog fd;
	fd.setFileMode(QFileDialog::AnyFile);
	fd.setFilter("CSV Files (*.csv)");
	fd.setDirectory(QCoreApplication::applicationDirPath());
	fd.setAcceptMode(QFileDialog::AcceptSave);
	QStringList filenames;
	if (fd.exec()) {
		filenames = fd.selectedFiles();
		QStringList::iterator i = filenames.begin();

		//Add file extension if necesary
		QRegExp csvExt("*.csv");
		csvExt.setPatternSyntax(QRegExp::Wildcard);
		if (!csvExt.exactMatch((*i)))
			(*i) = (*i) + ".csv";

		//Write file
		QFile file((*i));
		if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
			QTextStream out(&file);
			out << header << endl;
			for (QList<QString>::const_iterator i = data.begin(); i
					!= data.end(); ++i) {
				out << (*i) << endl;
			}
		}
	}

}

void TrayMenu::showItw() {
	itw->show();
	itw->activateWindow();
}

void TrayMenu::handleActivated(QSystemTrayIcon::ActivationReason reason) {
	if (reason == QSystemTrayIcon::DoubleClick) {
		if (itw->isVisible()) {
			itw->hide();
		} else {
			showItw();
		}
	}
}

void TrayMenu::saveLog() {
	log->saveLog();
}
