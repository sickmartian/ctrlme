#include <QString>

#include <QDomElement>
#include <QDomText>
#include <QDomDocument>

#include "task.h"
#include "taskid.h"

int Task::lastUseTime = 604800;

/*
    | CONSTRUCTORS |
*/
Task::Task(const Task &task)
: lastUse(task.getLastUse()),
  id(task.getId()),
  active(task.getActive()),
  selected(task.getSelected())
{
  description = new QString(task.getDescription());
}

Task::Task(QString description, task_id id, bool selected,bool active)
: id(id),
  active(active),
  selected(selected)
{
  lastUse = QDateTime::currentDateTime();
  this->description = new QString(description);
}

Task::Task()
: id(0),
  active(false),
  selected(false)
{
  lastUse = QDateTime::currentDateTime();
  this->description = new QString("Tarea borrada");
}

Task::Task(QDomElement element)
: active(true)
{
  QDomNode n = element.firstChild();
  while ( !n.isNull() ) {
    QDomElement childElement = n.toElement();
    if ( !childElement.isNull() ) {
      if (childElement.tagName() == "Description") {
        description = new QString(childElement.text());
      }
      else if (childElement.tagName() == "Id") {
        id = childElement.text().toUInt();
      }
      else if (childElement.tagName() == "Active") {
        active = childElement.text().toInt();
      }
      else if (childElement.tagName() == "lastUse") {
        lastUse = QDateTime::fromString( childElement.text(), "yyyyMMddhhmmss");
      }
    }
    n = n.nextSibling();
  }

  if (lastUse.secsTo(QDateTime::currentDateTime())>getLastUseTime())
    active=false;

  selected = false;

}

/*
    | DESTRUCTOR |
*/
Task::~Task() {
  delete description;
}

/*
    | OPERATORS |
*/
Task& Task::operator=(const Task &task)
{
  lastUse = task.getLastUse();
  id = task.getId();
  active = task.getActive();
  selected = task.getSelected();
  *description = task.getDescription();
  return *this;
}

/*
    | GETTERS |
*/
QString Task::getDescription() const {
  return *this->description;
}

task_id Task::getId() const {
  return this->id;
}

bool Task::getSelected() const {
  return this->selected;
}

QDomElement Task::getElement() const {
  QDomDocument dummy;
  QDomElement task = dummy.createElement("Task");

  QDomElement idElement = dummy.createElement("Id");
  QDomText idText = dummy.createTextNode(QString::number(id));
  idElement.appendChild(idText);
  task.appendChild(idElement);

  QDomElement activeElement = dummy.createElement("Active");
  QDomText activeText = dummy.createTextNode(QString::number(active));
  activeElement.appendChild(activeText);
  task.appendChild(activeElement);

  QDomElement lastUseElement = dummy.createElement("lastUse");
  QDomText lastUseText = dummy.createTextNode( lastUse.toString("yyyyMMddhhmmss") );
  lastUseElement.appendChild(lastUseText);
  task.appendChild(lastUseElement);

  QDomElement descriptionElement = dummy.createElement("Description");
  QDomText descriptionText = dummy.createTextNode(*this->description);
  descriptionElement.appendChild(descriptionText);
  task.appendChild(descriptionElement);

  return task;
}

bool Task::getActive() const {
  return this->active;
}

QDateTime Task::getLastUse() const {
  return this->lastUse;
}

int Task::getLastUseTime() {
  return lastUseTime;
}

/*
    | SETTERS |
*/
void Task::setDescription(QString description) {
  if ( this->description != 0 )
    delete this->description;
  this->description = new QString(description);
}

void Task::setSelected(bool selected) {
  this->selected = selected;
}

void Task::setActive(bool active) {
  this->active = active;
}

void Task::setLastUse(QDateTime lastUse) {
  this->lastUse = lastUse;
}

void Task::setLastUse() {
  this->lastUse = QDateTime::currentDateTime();
}

void Task::setLastUseTime(int lastUseTime) {
  Task::lastUseTime = lastUseTime;
}
