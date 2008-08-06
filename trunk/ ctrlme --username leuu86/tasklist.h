#ifndef TASKLIST
   #define TASKLIST 0

#include <QFile>
#include <QTextStream>
#include <QDomDocument>
#include <QDomNodeList>
#include <QObject>
#include <QRegExp>
#include <QList>
#include <QSettings>

#include "task.h"
#include "taskid.h"

//Using QList because vector's erase method deletes the last one
//But QList doesn't permit declaring functions as comparators for sorting, so resulting to std::list (WITHOUT RANDOM ACCESS ITERATORS)
typedef std::list<Task> List;
enum ListSort { ById, ByDescription, ByLastUse, BySelectedFirstLastUse };
class QString;

class TaskList : public QObject
{

Q_OBJECT

  public:
    //constructors
    TaskList(QSettings*);

    //destructor
    ~TaskList();

    //utilities
    bool saveToXML() const;
    void deleteTask(task_id id);
    void unselectAllTasks();

    //getters
    Task getTask(task_id id) const;
    Task getTask(QString description) const;
    List getTaskList(ListSort sort = ById,bool onlyActive = true) const;
    bool getSelected(task_id) const;
    bool getActive(task_id) const;
    int size() const;
    QString getFilename() const;

    //setters
    void setSelected(task_id , bool);
    void setActive(task_id, bool);
    void setLastUse(task_id);
    void clearSelections();
    bool setDescription(task_id, QString);
    bool appendTask(QString description,task_id*,bool selected = false);
    void appendTask(QDomElement element);
    void setFilename(QString);

  public slots:

  signals:
    void taskAdded(task_id id);
    void taskDeleted(task_id id);

  private:
    void readSettings();
    void writeSettings();
    bool checkTaskDesc(QString ) const;

    List::iterator getTaskRef(task_id id,bool *ok) const;
    Task getTaskCopy(task_id id) const;
    Task getTaskCopy(QString description) const;
    QString filename;
    List *tasks;
    QSettings *settings;
    TaskId &idMananger;
};

#endif
