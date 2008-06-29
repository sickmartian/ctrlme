#include <QtGui>
#include <QStyle>
#include <QPlastiqueStyle>
#include <QListWidgetItem>
#include "taskliststyle.h"

TaskListStyle::TaskListStyle(QWidget *parent)
  : QPlastiqueStyle()
{
  parent->setStyleSheet("QListWidget { color: black; background-color: white; selection-color: red; selection-background-color: white; font: 14px; }");
}

void TaskListStyle::drawPrimitive(QStyle::PrimitiveElement element,
                                        const QStyleOption *option,
                                        QPainter *painter,
                                        const QWidget *widget) const
{
  if (element != QStyle::PE_FrameFocusRect && qobject_cast<const QAbstractItemDelegate*>(widget) == 0)
  {  QPlastiqueStyle::drawPrimitive(element, option, painter, widget); }
}
