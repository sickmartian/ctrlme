#ifndef TRAYMENU
#define TRAYMENU

#include <QSystemTrayIcon>
#include <QAction>
#include <QIcon>
#include <QMenu>
#include <QApplication>
#include <QDesktopWidget>
#include <QInputDialog>
#include <QTextStream>
#include <QFile>
#include <QFileDialog>

#include "tasks_wdg.h"
#include "inputtimewnd.h"
#include "log.h"
#include "ask_date_interval.h"
#include "settings.h"
#include "timeline.h"

class TrayMenu: public QSystemTrayIcon {
Q_OBJECT

public:
TrayMenu();
~TrayMenu();
void readSettings();

public slots:
	void writeReport(QDate,QDate);

	signals:
	void quit();

private slots:
	void handleActivated(QSystemTrayIcon::ActivationReason);
	void setEnabledIcon(bool);
	void showItw();

private:
	void writeSettings();

	QMenu *trayMenu;
	QAction *reportAct;
	QAction *showAct;
	QAction *forceLogAct;
	QAction *autoLogAct;
	QAction *quitAct;
	QAction *timeLineAct;
	QAction *showSettingsAct;

	Settings *settingsDialog;
	QSettings *settings;
	TaskList *taskList;
	TasksWdg *taskWdg;
	InputTimeWnd *itw;
	AskDateInterval *askReportDate;
	Log *log;
	TimeLine *timeLine;

};

#endif
