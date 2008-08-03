#ifndef TASKSWDG
  #define TASKSWDG

#include <QWidget>
#include <QListWidget>
#include <QShowEvent>
#include <QCloseEvent>
#include <QVBoxLayout>
#include <QLineEdit>
#include <QListWidget>
#include <QPushButton>
#include <QString>
#include <QListWidgetItem>
#include <QStringList>
#include <QAction>
#include <QMenu>
#include <QContextMenuEvent>
#include <QPoint>
#include <QPlastiqueStyle>
#include <QItemSelectionModel>
#include <QList>
#include <QModelIndex>
#include <QApplication>
#include <QDesktopWidget>
#include <QKeyEvent>
#include <QFont>
#include <QSettings>

#include "tasklist_style.h"
#include "tasklist.h"
#include "log.h"

class TasksWdg : public QWidget
{
  Q_OBJECT

public:
  TasksWdg(TaskList *tasklist, Log*, QSettings* settings, QWidget *parent = 0);
  ~TasksWdg();
  void readSettings();

public slots:
  void taskAddedExternally(task_id);

private slots:
  void addTask();
  void listMenu(const QPoint &);
  void deleteItem();
  void activateItem();
  void editTask(const QModelIndex&);

private:

  QSettings *settings;

  void writeSettings();

  void keyPressEvent(QKeyEvent*);
  void refreshShownTasks();
  void closeEvent( QCloseEvent *event);
  void fillTasks();
  void listAddTask(Task tarea);
  void showEvent( QShowEvent *event );

  bool editMode;
  task_id editId;
  ListSort sort;

  QLineEdit *text;
  QListWidget *list;
  QAction *deleteAct;
  QAction *activateAct;
  TaskList *tasklist;
  List *shownTasks;
  Log *log;
};

#endif
