#ifndef ITW
  #define ITW

#include <QScrollArea>
#include <QVBoxLayout>
#include <QPushbutton>
#include <QLineEdit>
#include <QWidget>
#include <QList>
#include <QCloseEvent>
#include <QMessageBox>
#include <QAction>
#include <QRegExp>
#include <QTimer>
#include <QFocusEvent>
#include <QShowEvent>
#include <QApplication>
#include <QDesktopWidget>
#include <QSizePolicy>

#include "line_edit_timer.h"
#include "tasklist.h"
#include "log.h"
//List of QPushButton references because of the Q_DISABLE_COPY macro on QPushButtons
typedef QList<QPushButton*> BList;

class InputTimeWnd : public QWidget
{
  Q_OBJECT

public:
  InputTimeWnd(TaskList *tasklist, Log *logInstance, QSettings *settings, QWidget *parent=0);
  ~InputTimeWnd();
  //GETTERS
  int getAskInterval() const;
  int getAskTimeOut() const;
  bool getOnlyOneTask() const;
  bool getAskForInput() const;
  //SETTERS
  void setAskInterval(int);
  void setAskTimeOut(int);
  void logSelectedTasks();
  void hide();
  void show();

public slots:
  void taskAddedExternally(task_id);
  void taskDeletedExternally(task_id);
  void setAskForInput(bool ask);
  void setOnlyOneTask(bool);
  void showWithTimeout();
  void readSettings();

private slots:
  void gotFocus();

private:
  void writeSettings();
  void keyPressEvent(QKeyEvent*);
  void showEvent( QShowEvent *event );

  void focusInEvent ( QFocusEvent* );
  void refreshGeometry();

  void fillTasks();
  void closeEvent( QCloseEvent *event);
  void startTimer();
  void stopTimer();
  void startSubTimer();
  void stopSubTimer();

  void addTaskInt();
  void deleteShownTasks();
  void createTaskButtons();
  void deleteTaskButtons();
  void updateShownTaskList();
  void backUpTaskList();
  void restoreTaskList();

  bool acceptBtnVisible;
  bool onlyOneTask;
  bool askForInput;
  int askInterval;
  int askTimeOut;
  int btnHeight;
  ListSort sort;

  QSettings *settings;
  QTimer *subTimer;
  QTimer *timer;
  Log *logInstance;
  List *shownTaskList;
  List *savedTaskList;
  TaskList *tasklist;
  QScrollArea *sa;
  QWidget *window;
  QVBoxLayout *layout;
  QVBoxLayout *layout_out;
  LineEditTimer *le;
  BList *buttonList;
  QPushButton *acceptBtn;
  QAction *explicitAcceptActEnt;
  QAction *explicitAcceptActRet;

private slots:
  void addTask();
  void explicitAceptance();
  void implicitAceptance();
  void buttonStatusRefresh();

};

#endif
