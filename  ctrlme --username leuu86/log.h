#ifndef LOG
  #define LOG

#include <QDateTime>
#include <QFile>
#include <QTextStream>
#include <QDomDocument>
#include <QSettings>

#include <list>
#include <map>

#include "tasklist.h"
#include "task.h"
#include "entry_id.h"
#include "reporting.h"

using namespace std;

class Log
{
public:
    Log(TaskList*, QSettings*);
    ~Log();
    void write(Task);
    void startLog();
    void stopLog();
    void setFilename(QString);
    Report getReport(QDate*, QDate*);
    void saveLog();
    void createEntry();

protected:
  Log(const Log&);
  Log& operator=(const Log&);

private:

  QSettings *settings;

  void readSettings();
  void saveSettings();
  void readLog(QString filename);
  void saveFile();
  void writeSessionEnd();

  QString filename;

  bool started;
  bool fileAlreadyRead;
  int taskCount;

  QDateTime *startDt;

  QDomDocument *doc;
  QDomElement *currentEntry;

  TaskList *taskList;
  EntryId &idMananger;

};

#endif
