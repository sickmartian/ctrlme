#include <QWidget>
#include <QVBoxLayout>
#include <QLineEdit>
#include <QListWidget>
#include <QPushButton>
#include <QString>
#include <QListWidgetItem>
#include <QMessageBox>
#include <QStringList>
#include <QAction>
#include <QMenu>
#include <QContextMenuEvent>
#include <QPoint>
#include <QPlastiqueStyle>
#include "taskliststyle.h"
#include "tareas.h"

using namespace std;

Tasks::Tasks(QWidget *parent)
  : QWidget(parent)
{

  QVBoxLayout *layout = new QVBoxLayout;
  
  text = new QLineEdit("");
  text->setObjectName("linea");
  layout->addWidget(text);

  list = new QListWidget;
  list->setStyle(new TaskListStyle(list));
  list->setContextMenuPolicy(Qt::CustomContextMenu);
  QObject::connect(list, SIGNAL(customContextMenuRequested(const QPoint &)), this, SLOT(listMenu(const QPoint &)));

  deleteAct = new QAction("Delete",this);
  QObject::connect(deleteAct, SIGNAL(triggered()), this, SLOT(borrarItem()));

  layout->addWidget(list);
  
  QObject::connect(text, SIGNAL(returnPressed()), this, SLOT(addTask()));
  
  parent->setLayout(layout); 
}

void Tasks::addATask(QString tarea)
{
  QListWidgetItem *task = new QListWidgetItem(tarea);
  list->addItem(task);
}

QString* Tasks::getItems( int *count )
{
  int size = list->count();
  QString *test = new QString[size];
  
  for (int i = 0; i < size; i++)
  {
    test[i] = list->item(i)->text();
  }
  
  *count = size;
  return test;
}

void Tasks::addTask()
{
  this->list->addItem(this->text->text());
  this->text->setText("");  
}

void Tasks::listMenu(const QPoint &point) {
  QMenu menu(this);
  menu.addAction(this->deleteAct);
  menu.exec(list->mapToGlobal(point));
}

void Tasks::borrarItem() {
  qDeleteAll(list->selectedItems());
}
