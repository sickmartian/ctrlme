/*
 * timeline.h
 *
 *  Created on: 26-jul-2008
 *      Author: leuu
 */

#ifndef TIMELINE_H_
#define TIMELINE_H_

#include <QWidget>
#include <QCloseEvent>
#include <QSplitter>
#include <QLabel>
#include <QListView>
#include <QTreeView>
#include <QTextEdit>
#include <QRubberBand>
#include <QPoint>
#include <QMouseEvent>

#include "log.h"
#include "tasklist.h"
#include "graphic_task.h"

class TimeLine : public QWidget
{
	Q_OBJECT;
public:
	TimeLine(Log*,TaskList*,QWidget* = 0);

private:
	void mousePressEvent(QMouseEvent*);
	void mouseMoveEvent(QMouseEvent*);
	void mouseReleaseEvent(QMouseEvent*);
	void closeEvent(QCloseEvent*);

	TaskList *taskList;
	Log *log;
	QSplitter *splitter;

	QPoint origin;
	QRubberBand *rubberBand;

};

#endif /* TIMELINE_H_ */
