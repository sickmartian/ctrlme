#ifndef TASK
  #define TASK 0

#include <QString>
#include <QDateTime>
#include "taskid.h"

class QDomElement;

class Task
{
  public:
    //constructors
    Task(const Task &);    
    Task(QString description, task_id id, bool selected = false,bool active = true);
    Task(QDomElement element);
    Task();
    //destructor
    ~Task();
    //operators
    Task& operator=(const Task&);
    //getters
    QDomElement getElement() const;
    QString getDescription() const;
    task_id getId() const;    
    bool getSelected() const;
    bool getActive() const;
    QDateTime getLastUse() const;
    static int getLastUseTime();
    //setters
    void setDescription(QString );
    void setSelected(bool );
    void setActive(bool );
    void setLastUse();
    void setLastUse(QDateTime);
    static void setLastUseTime(int);
  private:
    static int lastUseTime;
    QString *description;
    QDateTime lastUse;
    task_id id;
    bool active;
    bool selected;
};

#endif
