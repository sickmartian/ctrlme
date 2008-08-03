#ifndef REPORTING
  #define REPORTING

#include <QDateTime>
#include <QDomElement>
#include <map>
#include "task.h"

typedef struct taskDate {
  task_id task;
  QDate date;
  bool operator<(const taskDate& two) const {
    if (task < two.task) return true;
    if (task > two.task) return false;
    if (date < two.date) return true;
    return false;
  }
};
typedef std::map<taskDate,float> Report;

typedef struct taskControl {
  QDateTime begin;
  QDateTime end;
  bool inUse;
  bool operator<(const taskControl& two) const {
    if (begin < two.begin) return true;
    if (begin > two.begin) return false;
    return false;
  }
  taskControl() {
    inUse = false;
  }
};
typedef std::map<int, taskControl> tasksMap;

float obtenerCompartido(tasksMap *tasksTime,QDateTime taskBeginDate,QDateTime taskEndDate,int entryId,int contador);
float obtenerTarea(tasksMap *tasksTime,QDateTime taskBeginDate,QDateTime taskEndDate,int entryId);
float calculateTask(QDomElement task,QDomElement entry);

#endif
