/*
 * timeline.cpp
 *
 *  Created on: 26-jul-2008
 *      Author: leuu
 */

#include "timeline.h"

TimeLine::TimeLine(Log* log, TaskList *taskList, QWidget *parent)
: 	QWidget(parent),
	taskList(taskList),
	log(log)
{
	rubberBand = new QRubberBand(QRubberBand::Rectangle, this);

	//splitter = new QSplitter(this);
	GraphicTask *gt1 = new GraphicTask(this);//splitter);
    GraphicTask *gt2 = new GraphicTask(this);//splitter);
    GraphicTask *gt3 = new GraphicTask(this);//splitter);
    /*QWidget *filler = new QWidget();
    filler->setMinimumWidth(0);
    QWidget *filler2 = new QWidget();
    filler2->setMinimumWidth(0);
    QWidget *filler3 = new QWidget();
    filler3->setMinimumWidth(0);
    splitter->setOpaqueResize(false);
	splitter->addWidget(gt1);
	splitter->addWidget(filler);
	splitter->addWidget(gt2);
	splitter->addWidget(filler2);
	splitter->addWidget(gt3);
	splitter->addWidget(filler3);
	splitter->setHandleWidth(0);
	splitter->resize(400,400);*/
}

void TimeLine::closeEvent( QCloseEvent *event )
{
  event->ignore();
  hide();
}

void TimeLine::mousePressEvent(QMouseEvent *event)
{
    origin = event->pos();
    rubberBand->setGeometry(QRect(origin, QSize()));
    rubberBand->show();
}

void TimeLine::mouseMoveEvent(QMouseEvent *event)
{
    rubberBand->setGeometry(QRect(origin, event->pos()).normalized());
}

void TimeLine::mouseReleaseEvent(QMouseEvent *event)
{
    rubberBand->hide();
    // determine selection, for example using QRect::intersects()
    // and QRect::contains().
}
