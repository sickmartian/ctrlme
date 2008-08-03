#include "settings.h"

Settings::Settings(QSettings *settings,Log *log,InputTimeWnd *itw,TaskList *taskList,TasksWdg *tasksWdg,const TrayMenu *trayMenu)
: settings(settings),
  log(log),
  itw(itw),
  taskList(taskList),
  trayMenu(trayMenu),
  tasksWdg(tasksWdg)
{
  layout = new QGridLayout(this);

  automaticLogGroup = new QGroupBox(tr("Periodic logging"));
  taskListGroup = new QGroupBox(tr("Tasklist"));
  inputGroup = new QGroupBox(tr("Input selection"));
  behaviourGroup = new QGroupBox(tr("Misc Behaviour"));

  vboxAutomatic = new QVBoxLayout;
  vboxTaskList = new QVBoxLayout;
  vboxBehaviour = new QVBoxLayout;
  vboxInput = new QVBoxLayout;
  hboxButtons = new QHBoxLayout;
  hboxTimer1 = new QHBoxLayout;
  hboxTimer2 = new QHBoxLayout;

  lAskInterval = new QLabel(this);
  lAskInterval->setText(tr("Ask Interval (in minutes)"));
  lAskTimeOut = new QLabel(this);
  lAskTimeOut->setText(tr("Ask Timeout (in seconds)"));
  askInterval = new QSpinBox;
  askTimeOut = new QSpinBox;

  startLogging = new QCheckBox(tr("Ask at program start"));
  markOnlyOne = new QCheckBox(tr("Unselect tasks on new task"));
  showAccept = new QCheckBox(tr("Show accept button"));

//TaskList
  taskListSortGroup = new QGroupBox(tr("Sort"));
  taskListSortLayout = new QGridLayout;
  ByIdTL = new QRadioButton(tr("Creation time"),taskListSortGroup);
  taskListSortLayout->addWidget(ByIdTL,0,0);
  ByDescriptionTL = new QRadioButton(tr("Description"),taskListSortGroup);
  taskListSortLayout->addWidget(ByDescriptionTL,0,1);
  ByLastUseTL = new QRadioButton(tr("Last use"),taskListSortGroup);
  taskListSortLayout->addWidget(ByLastUseTL,1,0);
  BySelectedFirstLastUseTL = new QRadioButton(tr("Relevance"),taskListSortGroup);
  taskListSortLayout->addWidget(BySelectedFirstLastUseTL,1,1);
  taskListSortGroup->setLayout(taskListSortLayout);

//ITW
  itwSortGroup = new QGroupBox(tr("Sort"));
  itwSortLayout = new QGridLayout;
  ByIdITW = new QRadioButton(tr("Creation time"),itwSortGroup);
  itwSortLayout->addWidget(ByIdITW,0,0);
  ByDescriptionITW = new QRadioButton(tr("Description"),itwSortGroup);
  itwSortLayout->addWidget(ByDescriptionITW,0,1);
  ByLastUseITW = new QRadioButton(tr("Last use"),itwSortGroup);
  itwSortLayout->addWidget(ByLastUseITW,1,0);
  BySelectedFirstLastUseITW = new QRadioButton(tr("Relevance"),itwSortGroup);
  itwSortLayout->addWidget(BySelectedFirstLastUseITW,1,1);
  itwSortGroup->setLayout(itwSortLayout);

//Buttons
  accept = new QPushButton(tr("Accept"));
  cancel = new QPushButton(tr("Cancel"));
  QObject::connect(accept,SIGNAL(clicked()),this,SLOT(acceptSlot()));
  QObject::connect(cancel,SIGNAL(clicked()),this,SLOT(cancelSlot()));

  //Height
  hboxItwMaxH = new QHBoxLayout;
  lItwMaxH = new QLabel;
  lItwMaxH->setText("Maximum Height (from screen) 1/");
  itwMaxH = new QSpinBox;
  hboxItwMaxH->addWidget(lItwMaxH);
  hboxItwMaxH->addWidget(itwMaxH);
  //Width
  hboxItwMaxW = new QHBoxLayout;
  lItwMaxW = new QLabel;
  lItwMaxW->setText("Maximum Width (from screen) 1/");
  itwMaxW = new QSpinBox;
  hboxItwMaxW->addWidget(lItwMaxW);
  hboxItwMaxW->addWidget(itwMaxW);

  setOptions();

  vboxInput->addWidget(markOnlyOne);
  vboxInput->addWidget(showAccept);
  vboxInput->addLayout(hboxItwMaxH);
  vboxInput->addLayout(hboxItwMaxW);
  vboxInput->addWidget(itwSortGroup);

  hboxTimer1->addWidget(lAskInterval);
  hboxTimer1->addWidget(askInterval);
  hboxTimer2->addWidget(lAskTimeOut);
  hboxTimer2->addWidget(askTimeOut);
  vboxAutomatic->addWidget(startLogging);
  vboxAutomatic->addLayout(hboxTimer1);
  vboxAutomatic->addLayout(hboxTimer2);

  vboxTaskList->addWidget(taskListSortGroup);

  hboxButtons->addWidget(accept);
  hboxButtons->addWidget(cancel);

  automaticLogGroup->setLayout(vboxAutomatic);
  inputGroup->setLayout(vboxInput);
  behaviourGroup->setLayout(vboxBehaviour);
  taskListGroup->setLayout(vboxTaskList);

  layout->addWidget(automaticLogGroup,0,0);
  layout->addWidget(taskListGroup,0,1);
  layout->addWidget(inputGroup,1,0);
  layout->addWidget(behaviourGroup,1,1);
  layout->addLayout(hboxButtons,2,1);

  readSettings();

}

void Settings::setOptions() {
  if ( settings->value("itw/onlyOneTask",false).toBool() ) {
    markOnlyOne->setCheckState(Qt::Checked);
  }
  else {
    markOnlyOne->setCheckState(Qt::Unchecked);
  }
  if ( settings->value("itw/acceptBtnVisible",true).toBool() ) {
    showAccept->setCheckState(Qt::Checked);
  }
  else {
    showAccept->setCheckState(Qt::Unchecked);
  }
  if ( settings->value("itw/autoLogOnStart",true).toBool() ) {
    startLogging->setCheckState(Qt::Checked);
  }
  else {
    startLogging->setCheckState(Qt::Unchecked);
  }
  askInterval->setValue( itw->getAskInterval() );
  askTimeOut->setValue( itw->getAskTimeOut() );
  itwMaxW->setValue( settings->value("itw/maximumWidth",5).toInt() );
  itwMaxH->setValue( settings->value("itw/maximumHeight",2).toInt() );

  setItwSort();
  setTlSort();

}

void Settings::setItwSort() {
  int sortAux = settings->value("itw/sort",(int)BySelectedFirstLastUse).toInt();
  ListSort sort = (ListSort)sortAux;
  switch(sort) {
    case ById:
      ByIdITW->setChecked(true);
      break;
    case ByDescription:
      ByDescriptionITW->setChecked(true);
      break;
    case ByLastUse:
      ByLastUseITW->setChecked(true);
      break;
    case BySelectedFirstLastUse:
      BySelectedFirstLastUseITW->setChecked(true);
      break;
  }

}

void Settings::setTlSort() {
  int sortAux = settings->value("tasklist/sort",(int)ByDescription).toInt();
  ListSort sort = (ListSort)sortAux;
  switch(sort) {
    case ById:
      ByIdTL->setChecked(true);
      break;
    case ByDescription:
      ByDescriptionTL->setChecked(true);
      break;
    case ByLastUse:
      ByLastUseTL->setChecked(true);
      break;
    case BySelectedFirstLastUse:
      BySelectedFirstLastUseTL->setChecked(true);
      break;
  }
}

void Settings::writeItwSort() {

  if ( ByIdITW->isChecked() ) {
    settings->setValue("itw/sort",ById);
  }
  else if ( ByDescriptionITW->isChecked() ) {
    settings->setValue("itw/sort",ByDescription);
  }
  else if ( ByLastUseITW->isChecked() ) {
    settings->setValue("itw/sort",ByLastUse);
  }
  else if ( BySelectedFirstLastUseITW->isChecked() ) {
    settings->setValue("itw/sort",BySelectedFirstLastUse);
  }

}

void Settings::writeTlSort() {

  if ( ByIdTL->isChecked() ) {
    settings->setValue("tasklist/sort",ById);
  }
  else if ( ByDescriptionTL->isChecked() ) {
    settings->setValue("tasklist/sort",ByDescription);
  }
  else if ( ByLastUseTL->isChecked() ) {
    settings->setValue("tasklist/sort",ByLastUse);
  }
  else if ( BySelectedFirstLastUseTL->isChecked() ) {
    settings->setValue("tasklist/sort",BySelectedFirstLastUse);
  }

}

void Settings::readSettings() {
  adjustSize();
  QRect scr = QApplication::desktop()->availableGeometry();
  move( settings->value("settings/pos", scr.center() - rect().center()).toPoint() );
}

void Settings::writeSettings() {
  settings->setValue("settings/pos", pos());
}

void Settings::applySettings() {
  settings->setValue("itw/onlyOneTask",markOnlyOne->checkState()==Qt::Checked ? true : false);
  settings->setValue("itw/acceptBtnVisible",showAccept->checkState()==Qt::Checked ? true : false);
  settings->setValue("itw/autoLogOnStart",startLogging->checkState()==Qt::Checked ? true : false);
  settings->setValue("itw/askInterval", askInterval->value()*60000);// Minutes -> miliseconds
  settings->setValue("itw/askTimeOut", askTimeOut->value()*1000);// Seconds -> miliseconds
  settings->setValue("itw/maximumWidth", itwMaxW->value());
  settings->setValue("itw/maximumHeight", itwMaxH->value());
  writeItwSort();
  itw->readSettings();
  writeTlSort();
  tasksWdg->readSettings();
}

Settings::~Settings()
{
  writeSettings();
  delete automaticLogGroup;
  delete taskListGroup;
  delete behaviourGroup;
  delete inputGroup;
  delete startLogging;
  delete markOnlyOne;
  delete showAccept;
  delete vboxAutomatic;
  delete vboxTaskList;
  delete vboxBehaviour;
  delete hboxTimer1;
  delete hboxTimer2;
  delete askInterval;
  delete askTimeOut;
  delete itwSortGroup;
  delete taskListSortGroup;
  delete itwSortLayout;
  delete taskListSortLayout;
}

void Settings::closeEvent( QCloseEvent *event )
{
  writeSettings();
  event->ignore();
  hide();
}

void Settings::showEvent( QShowEvent *event ) {
  setOptions();
}


void Settings::acceptSlot() {
  applySettings();
  hide();
}

void Settings::cancelSlot() {
  hide();
}
