#include "reporting.h"

float obtenerCompartido(tasksMap *tasksTime,QDateTime taskBeginDate,QDateTime taskEndDate,int entryId,int contador) {

  float w = 0;
  float resultado = 0;
  QDateTime b,e;
  contador++;
  int tiempo;
  tiempo = taskBeginDate.secsTo(taskEndDate);

  for (tasksMap::iterator i=tasksTime->begin(); i!=tasksTime->end(); ++i) {
    if ( ((*i).second.begin != (*i).second.end)
    && ( (*i).first != entryId )
    && ( ( (taskBeginDate<=(*i).second.begin) && !(taskEndDate<=(*i).second.begin) ) || ( (taskEndDate>=(*i).second.end) && !(taskBeginDate>=(*i).second.end) ) )
    && ( (*i).second.inUse == false ) ) {

      // --- CALCULO COMPETENCIA ---
      // El periodo esta incluido entero
      if ( ((*i).second.begin > taskBeginDate) && ((*i).second.end < taskEndDate) ) {
        b = (*i).second.begin;
        e = (*i).second.end;
      }
      // El periodo esta incluido por derecha
      else if ((*i).second.begin > taskBeginDate) {
        if ( (*i).second.end != taskEndDate ) {
          b = taskEndDate;
        }
        else {
          b = (*i).second.begin;
        }
        e = (*i).second.end;
      }
      // El periodo esta incluido por izquierda
      else if  ((*i).second.end < taskEndDate) {
        b = (*i).second.begin;
        if ( (*i).second.begin != taskBeginDate ) {
          e = taskBeginDate;
        }
        else {
          e = (*i).second.end;
        }
      }
      else if ( ((*i).second.begin <= taskBeginDate) && ((*i).second.end >= taskEndDate) ){
      // Yo estoy incluido en el
        b = taskBeginDate;
        e = taskEndDate;
      }

      (*i).second.inUse = true;
      w = obtenerCompartido(tasksTime,b,e,(*i).first,contador);
      (*i).second.inUse = false;
      tiempo = tiempo - b.secsTo(e);
      resultado = w + resultado;

      // --- CIERRO ---
      if ( ((*i).second.begin > taskBeginDate) && ((*i).second.end < taskEndDate) ) {
        (*i).second.end = (*i).second.begin;
      }
      // El periodo esta incluido por derecha
      else if ((*i).second.begin > taskBeginDate) {
        (*i).second.begin = taskEndDate;
      }
      // El periodo esta incluido por izquierda
      else if  ((*i).second.end < taskEndDate) {
        (*i).second.end = taskBeginDate;
      }
      else if ( ((*i).second.begin <= taskBeginDate) && ((*i).second.end >= taskEndDate) ) {
        // Estoy incluido (se supone que no debería estar incluido por derecha)
        (*i).second.begin = taskEndDate;
      }

    }
  }
  return ((tiempo)/contador + resultado);
}

float obtenerTarea(tasksMap *tasksTime,QDateTime taskBeginDate,QDateTime taskEndDate,int entryId) {

  float w = 0;
  float resultado = 0;
  int tiempo;
  tiempo = taskBeginDate.secsTo(taskEndDate);

  for (tasksMap::iterator i=tasksTime->begin(); i!=tasksTime->end(); ++i) {
    if ( ((*i).second.begin != (*i).second.end)
    && ( (*i).first != entryId )
    && ( (taskBeginDate<=(*i).second.begin) || (taskEndDate>=(*i).second.end) )
    && ( (*i).second.inUse == false ) ) {

      (*i).second.inUse = true;
      w = obtenerCompartido(tasksTime,(*i).second.begin,(*i).second.end,(*i).first,1);
      (*i).second.inUse = false;

      tiempo = tiempo - (*i).second.begin.secsTo((*i).second.end);

      (*i).second.begin = (*i).second.end;
      resultado = w + resultado;
    }
  }

  return (resultado + tiempo);
}

float calculateTask(QDomElement task,QDomElement entry) {

  taskControl tasksDates;
  tasksMap tasksTime;

  QDomElement currentTask;
  QDateTime currentBeginDate,currentEndDate,taskBeginDate,taskEndDate;
  int taskId = task.attributeNode("Id").value().toInt();

  taskBeginDate = QDateTime::fromString( task.firstChildElement("beginTimeStamp").text() , "yyyyMMddhhmmss");
  taskEndDate = QDateTime::fromString( task.firstChildElement("endTimeStamp").text() , "yyyyMMddhhmmss");
  // Si es una tarea actual, asumimos el momento del reporte para los calculos
  if ( taskEndDate.isNull() )
    taskEndDate = QDateTime::currentDateTime();

  currentTask = entry.firstChildElement("Task");
  while (!currentTask.isNull()) {
    int entryId = currentTask.attributeNode("Id").value().toInt();
    if (entryId != taskId) {

      currentBeginDate = QDateTime::fromString( currentTask.firstChildElement("beginTimeStamp").text() , "yyyyMMddhhmmss");
      currentEndDate = QDateTime::fromString( currentTask.firstChildElement("endTimeStamp").text() , "yyyyMMddhhmmss");
      if ( currentEndDate.isNull() )
        taskEndDate = QDateTime::currentDateTime();

      if (currentBeginDate >= taskEndDate) {
        //Esta fuera de lo buscado, salgo.
        //El resto de las tareas al estar ordenadas por fecha de inicio estan fuera también
        break;
      }
      else if ( !(currentEndDate <= taskBeginDate) ) {
        //Compruebo que no se dio antes, sino no me sirve esta tarea tengo que ir a la proxima

        //agrego la tarea al mapa de la que intento sacar
        if (currentBeginDate <= taskBeginDate) {
         tasksDates.begin = taskBeginDate;
        } else {
         tasksDates.begin = currentBeginDate;
        }
        if (currentEndDate >= taskEndDate) {
          tasksDates.end = taskEndDate;
        } else {
          tasksDates.end = currentEndDate;
        }
        tasksTime[entryId] = tasksDates;

      }
    }
    currentTask = currentTask.nextSiblingElement("Task");
  }

  float w = obtenerTarea(&tasksTime,taskBeginDate,taskEndDate,taskId);
  return w;
}
