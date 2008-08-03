#include "log.h"

/*Log& Log::Instance() {
  static Log inst;
  return inst;
}*/

Log::Log(TaskList *taskList,QSettings *settings)
: settings(settings),
  started(false),
  fileAlreadyRead(false),
  taskCount(0),
  taskList(taskList),
  idMananger( EntryId::getInstance() )
{

  readSettings();

  doc = new QDomDocument("Log");
  readLog(filename);
  currentEntry = new QDomElement();
  *currentEntry = doc->createElement("Entry");

  QDomElement beginTime = doc->createElement("beginTimeStamp");
  QDomText timeText = doc->createTextNode( QDateTime::currentDateTime().toString("yyyyMMddhhmmss") );
  beginTime.appendChild(timeText);
  currentEntry->appendChild(beginTime);

  currentEntry->setAttribute("Id",idMananger.getEntryId());

  doc->firstChildElement("Log").appendChild(*currentEntry);
}

void Log::readSettings() {
  filename = settings->value("log/filename","log.xml").toString();
}

void Log::saveSettings() {
  settings->setValue("log/filename",filename);
}

void Log::writeSessionEnd() {
  QDomElement endTime = doc->createElement("endTimeStamp");
  QDomText timeText = doc->createTextNode( QDateTime::currentDateTime().toString("yyyyMMddhhmmss") );
  endTime.appendChild(timeText);
  currentEntry->insertAfter(endTime,currentEntry->firstChildElement("beginTimeStamp"));
}

Log::~Log() {
  taskList->clearSelections();
  stopLog();
  writeSessionEnd();
  saveSettings();
  saveLog();
  delete currentEntry;
  delete doc;
}

void Log::startLog() {
  if (started) {
    stopLog();
  }
  //New tasks timestamp
  startDt = new QDateTime( QDateTime::currentDateTime() );
  started = true;

}

void Log::stopLog() {
  // Search if any unselected task is still open
  if (!currentEntry->isNull()) {

    QDomElement currentTask = currentEntry->firstChildElement("Task");
    while ( (!currentTask.isNull()) ) {
      if ( currentTask.firstChildElement("endTimeStamp").isNull() )
        //If the task is open and not selected, we should close it now
        if ( !taskList->getTask( currentTask.firstChildElement("Id").text().toUInt() ).getSelected() ) {
          QDomElement endTime = doc->createElement("endTimeStamp");
          QDomText timeText = doc->createTextNode( QDateTime::currentDateTime().toString("yyyyMMddhhmmss") );
          endTime.appendChild(timeText);
          QDomElement beginTime = currentTask.firstChildElement("beginTimeStamp");
          currentTask.insertAfter(endTime,beginTime);
        }
      currentTask = currentTask.nextSiblingElement("Task");
      }
    }

  if (started) {
    started = false;
    delete startDt;
  }

}

void Log::write(Task task) {

  bool taskFound = false;

  if (!started)
    startLog();

  // Search if the task is still open
  if (!currentEntry->isNull()) {

    QDomElement currentTask = currentEntry->firstChildElement("Task");
    while ( (!currentTask.isNull()) && taskFound==false ) {
      if ( currentTask.firstChildElement("Id").text().toUInt() != task.getId() ) {
        currentTask = currentTask.nextSiblingElement("Task");
      } else {
        //We found the task, check if it's still open
        if ( currentTask.firstChildElement("endTimeStamp").isNull() ) {
          taskFound=true;
        }
        else {
          currentTask = currentTask.nextSiblingElement("Task");
        }
      }
    }
  }

  //Either, the task was not found or is the first task of the entry
  if (!taskFound) {
    QDomElement taskElement = doc->createElement("Task");
    QDomElement taskIdElement = doc->createElement("Id");
    QDomElement descriptionElement = doc->createElement("Description");
    QDomElement notesElement = doc->createElement("Notes");
    QDomElement beginTimeElement = doc->createElement("beginTimeStamp");

    QDomText taskIdText = doc->createTextNode( QString::number( task.getId() ) );
    QDomText descriptionText = doc->createTextNode( task.getDescription() );
    QDomText timeText = doc->createTextNode( QDateTime::currentDateTime().toString("yyyyMMddhhmmss") );

    taskElement.setAttribute("Id",++taskCount);

    taskIdElement.appendChild( taskIdText );
    taskElement.appendChild( taskIdElement );
    beginTimeElement.appendChild( timeText );
    taskElement.appendChild( beginTimeElement );
    descriptionElement.appendChild( descriptionText );
    taskElement.appendChild( descriptionElement );
    taskElement.appendChild( notesElement );

    currentEntry->appendChild(taskElement);
  }

}

void Log::readLog(QString filename) {
  if (fileAlreadyRead)
    return;

  QFile file( filename );
  if ( file.open( QIODevice::ReadOnly ) ) {
    doc->setContent(&file);
    file.close();
  }
  //Ensures(and creates on the first run) the log element
  QDomElement log = doc->firstChildElement("Log");
  if ( log.isNull() ) {
    log = doc->createElement("Log");
    doc->appendChild(log);
  }
}

void Log::saveLog() {

  readLog(filename);

  if (!doc->firstChild().isProcessingInstruction()) {
    QDomNode xmlNode = doc->createProcessingInstruction("xml","version=\"1.0\"");
    doc->insertBefore(xmlNode, doc->firstChild());
  }

  if ( !(*currentEntry).isNull() ) {
    QDomElement log = doc->firstChildElement("Log");
    log.appendChild( (*currentEntry) );
  }

  QFile outFile( filename );
  if ( outFile.open( QIODevice::WriteOnly | QIODevice::Text ) ) {
    QTextStream out(&outFile);
    out << doc->toString();
    outFile.close();
  }
}

Report Log::getReport(QDate *beginDate, QDate *endDate) {

  Report report;
  taskDate reportHeader;

  if ( beginDate->isNull() || endDate->isNull() ) return report;

  bool actualEntry = false;

  QDate nullDate;
  QDate beginDateRange = (*beginDate);
  QDate endDateRange = (*endDate);
  QDate entryBeginDate;
  QDate entryEndDate;
  QDate taskBeginDate;
  QDate taskEndDate;
  //Empezamos devolviendo null hasta que encuentre fechas
  (*endDate) = nullDate;
  (*beginDate) = nullDate;

  QDomElement entry;
  QDomElement task;

  QDomElement log = (*doc).firstChildElement("Log");
  entry = log.firstChildElement("Entry");
  while (!entry.isNull()) {

    entryBeginDate = QDateTime::fromString( entry.firstChildElement("beginTimeStamp").text(), "yyyyMMddhhmmss").date();
    entryEndDate = QDateTime::fromString( entry.firstChildElement("endTimeStamp").text(), "yyyyMMddhhmmss").date();

    //También puede ser la entrada actual, sin endDate.. hay que dejarla entrar.
    if ( entryEndDate.isNull() ) {
      entryEndDate = QDate::currentDate();
      actualEntry = true;
    }

    //Si el final esta antes del principio de lo que busco, sigo con la proxima entrada
    if (entryEndDate < beginDateRange) {
      entry = entry.nextSiblingElement("Entry");
      continue;
    }
    //Si el inicio de esta esta despues de lo que busco, me voy (estan ordenadas por fecha de inicio las entries)
    else if (entryBeginDate > endDateRange) {
      break;
    }

    task = entry.firstChildElement("Task");
    while (!task.isNull()) {

      taskBeginDate = QDateTime::fromString( task.firstChildElement("beginTimeStamp").text(), "yyyyMMddhhmmss").date();
      taskEndDate = QDateTime::fromString( task.firstChildElement("endTimeStamp").text(), "yyyyMMddhhmmss").date();
      //If it's the actual entry and it doesn't have an end time we use now
      if ( (actualEntry) && taskEndDate.isNull() ) {
         taskEndDate = QDateTime::currentDateTime().date();
      }

      //Solo dentro del rango
      if ( (taskBeginDate >= beginDateRange) && (taskEndDate <= endDateRange) ) {

      //Si no tenia fecha de inicio, y esta pasa el chequeo.. la uso, la 1era que encuentro es, por el orden de las tareas..
        if (beginDate->isNull()) {
          (*beginDate) = taskBeginDate;
        }
      //Si no tiene fecha de finalizacion o la que tiene es menor a la que puede tener, le mando la nueva
        if ( (endDate->isNull()) || ( (*endDate) < taskEndDate ) ) {
          (*endDate) = taskEndDate;
        }

        reportHeader.task = task.firstChildElement("Id").text().toUInt();
        reportHeader.date = taskEndDate;
        report[reportHeader] = report[reportHeader] + calculateTask(task,entry)/3600;

        task = task.nextSiblingElement("Task");

      }
    }

    entry = entry.nextSiblingElement("Entry");
  }

 return report;

}

void Log::setFilename(QString filename) {
  this->filename = filename;
}
