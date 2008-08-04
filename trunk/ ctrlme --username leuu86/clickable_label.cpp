/*
 * sort_label.cpp
 *
 *  Created on: 03-ago-2008
 *      Author: leuu
 */

#include "clickable_label.h"

SortLabel::SortLabel(QWidget *parent)
: QLabel(parent)
{
qDebug("Empty");
}

void SortLabel::mousePressEvent( QMouseEvent * e ) {
	if (e->button() == Qt::LeftButton) {
		emit clicked();
	}
}
