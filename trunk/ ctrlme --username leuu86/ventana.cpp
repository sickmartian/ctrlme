#include <QWidget>
#include <QMessageBox>
#include <QCloseEvent>
#include "ventana.h"
#include <QString>
#include <QDomDocument>
#include <QFile>
#include <QTextStream>
#include <QStringList>
#include <QDomNodeList>

MainWnd::MainWnd( QWidget *parent )
: QDialog(parent)
{ 
  tskmng = new Tasks(this);
  QDomDocument doc("Tasks");
  QFile file("out.xml");
  if (file.open(QIODevice::ReadOnly)) {
    if (doc.setContent(&file)) {
      file.close();
      QDomNodeList tareas = doc.elementsByTagName("Task");

      for (int i=0; i < tareas.size();++i) {
        tskmng->addATask(tareas.item(i).toElement().text());
      }
    }
  }
}

void MainWnd::closeEvent( QCloseEvent *event )
{
  int count;
  QString *items = this->tskmng->getItems(&count);
  QDomDocument doc("Tasks");
  QDomElement root = doc.createElement("Tasks");
  doc.appendChild(root);
  for (int i=0; i!=count; i++) {
    QDomElement task = doc.createElement("Task");
    root.appendChild(task);
    QDomText val = doc.createTextNode(*items);
    task.appendChild(val);
    *items++;
  }

  QFile file("out.xml");
  if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
    QTextStream out(&file);
    out << doc.toString();
  }
  
  event->ignore();
  hide();
}
