/*
 * graphic_task.cpp
 *
 *  Created on: 26-jul-2008
 *      Author: leuu
 */

#include "graphic_task.h"

GraphicTask::GraphicTask(QWidget *parent)
: 	QLabel(parent)
{
	show();
	resize(100,100);
	setFrameShape(QFrame::Box);
	setFrameShadow(QFrame::Plain);
	setLineWidth(1);
	setMidLineWidth(1);
	setText("Nombre de la tarea");
	setMinimumWidth(1);

}
