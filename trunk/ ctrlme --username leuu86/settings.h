#ifndef SETTINGS
  #define SETTINGS

#include <QSettings>
#include <QGridLayout>
#include <QGroupBox>
#include <QCheckBox>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QSpinBox>
#include <QLabel>
#include <QRadioButton>

#include "log.h"
#include "inputtimewnd.h"
#include "tasklist.h"
#include "tasks_wdg.h"

class TrayMenu;

class Settings : public QWidget
{
  Q_OBJECT

public:
  Settings(QSettings*,Log*,InputTimeWnd*,TaskList*,TasksWdg*,const TrayMenu *trayMenu);
  ~Settings();

private slots:
  void acceptSlot();
  void cancelSlot();

private:
  void applySettings();
  void readSettings();
  void writeSettings();
  void setOptions();
  void setItwSort();
  void setTlSort();
  void writeItwSort();
  void writeTlSort();
  void closeEvent(QCloseEvent*);
  void showEvent(QShowEvent*);

  QSettings *settings;
  Log *log;
  InputTimeWnd *itw;
  TaskList *taskList;
  const TrayMenu *trayMenu;
  TasksWdg *tasksWdg;

  QGridLayout *layout;
  QGroupBox *automaticLogGroup;
  QGroupBox *taskListGroup;
  QGroupBox *behaviourGroup;

  QVBoxLayout *vboxAutomatic;
  QVBoxLayout *vboxTaskList;
  QVBoxLayout *vboxBehaviour;
  QVBoxLayout *vboxInput;
  QHBoxLayout *hboxButtons;
  QHBoxLayout *hboxTimer1;
  QHBoxLayout *hboxTimer2;

  QGroupBox *inputGroup;
  QCheckBox *startLogging;
  QCheckBox *markOnlyOne;
  QCheckBox *showAccept;
  QSpinBox *askInterval;
  QSpinBox *askTimeOut;

  QGroupBox *taskListSortGroup;
  QRadioButton *ByIdTL;
  QRadioButton *ByDescriptionTL;
  QRadioButton *ByLastUseTL;
  QRadioButton *BySelectedFirstLastUseTL;
  QGridLayout *taskListSortLayout;

  QGroupBox *itwSortGroup;
  QRadioButton *ByIdITW;
  QRadioButton *ByDescriptionITW;
  QRadioButton *ByLastUseITW;
  QRadioButton *BySelectedFirstLastUseITW;
  QGridLayout *itwSortLayout;

  QLabel *lAskInterval;
  QLabel *lAskTimeOut;

  QPushButton *accept;
  QPushButton *cancel;

  QHBoxLayout *hboxItwMaxH;
  QHBoxLayout *hboxItwMaxW;
  QLabel *lItwMaxH;
  QLabel *lItwMaxW;
  QSpinBox *itwMaxH;
  QSpinBox *itwMaxW;

};

#endif
