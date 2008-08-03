#include "tasks_wdg.h"

//Initializes the layout, line edit and task list...
TasksWdg::TasksWdg(TaskList *tasklist, Log *log, QSettings *settings, QWidget *parent)
  : QWidget(parent),
    log(log)
{

  this->settings = settings;

  shownTasks = new List;
  QVBoxLayout *layout = new QVBoxLayout;

  // Line Edit
  text = new QLineEdit("");
  text->setObjectName("linea");
  layout->addWidget(text);
  QObject::connect(text, SIGNAL(returnPressed()), this, SLOT(addTask()));  
  // Task's list
  list = new QListWidget;
  list->setStyle(new TaskListStyle(list));
  list->setContextMenuPolicy(Qt::CustomContextMenu);
  QObject::connect(list, SIGNAL(customContextMenuRequested(const QPoint &)), this, SLOT(listMenu(const QPoint &)));
  QObject::connect(list, SIGNAL(doubleClicked(const QModelIndex&)), this, SLOT(editTask(const QModelIndex&)));
  // Actions
  deleteAct = new QAction("Delete",this);
  activateAct = new QAction("Activate",this);
  QObject::connect(deleteAct, SIGNAL(triggered()), this, SLOT(deleteItem()));
  QObject::connect(activateAct, SIGNAL(triggered()), this, SLOT(activateItem()));
  // Layout
  layout->addWidget(list); 
  setLayout(layout); 

  // TaskList reference
  this->tasklist = tasklist;
  QObject::connect(tasklist, SIGNAL(taskAdded(task_id)), this, SLOT(taskAddedExternally(task_id)));
  
  // Size & position
  readSettings();

  editMode = false;
  editId = 0;
  
}

void TasksWdg::editTask(const QModelIndex &item) {
  int objetive = item.row();
  editMode = true;
  for (List::const_iterator i = shownTasks->begin(); i!=shownTasks->end(); ++i) {
    if (objetive==0) {
      editId = (*i).getId();
      text->setText( (*i).getDescription() );
      break;
    }
    objetive--;
  }
}

void TasksWdg::showEvent( QShowEvent *event ) {
  text->setText("");
  list->clear();
  fillTasks();
  editMode = false;
}

TasksWdg::~TasksWdg() {
  writeSettings();
  delete text;
  delete list;
  delete deleteAct;
}

//Add a simple task to the list
void TasksWdg::listAddTask(Task tarea)
{
  QListWidgetItem *task = new QListWidgetItem(tarea.getDescription(),list);
  QFont font;
  font.setStrikeOut(!tarea.getActive());
  task->setFont(font);
  list->addItem(task);
}

void TasksWdg::refreshShownTasks() {
  shownTasks->clear();
  *shownTasks = tasklist->getTaskList(sort,false);
}

void TasksWdg::fillTasks() {
  refreshShownTasks();
  list->clear();
  List::const_iterator i;
  for ( i = shownTasks->begin(); i != shownTasks->end(); ++i )
    this->listAddTask(*i);
}

// SLOT which adds a task from the line edit
void TasksWdg::addTask()
{

  task_id newId;

  if (!editMode) {
  //If we are not editing, append the new task
    tasklist->appendTask(text->text(),&newId);
    text->setText("");
    fillTasks();
  } else {
  //If we are editing, change the description
    tasklist->setDescription( editId,text->text() );
    text->setText("");
    editMode = false; }
}

// Context menu
void TasksWdg::listMenu(const QPoint &point) {
  QMenu menu(this);
  menu.addAction(this->deleteAct);
  menu.addAction(this->activateAct);
  menu.exec(list->mapToGlobal(point));
}

// Action to delete all selected items
void TasksWdg::deleteItem() {

  List::iterator listIt ;

  QItemSelectionModel *sm = list->selectionModel();
  QList<QModelIndex> mil = sm->selectedIndexes();
  int j = 0;
  for (QList<QModelIndex>::iterator i = mil.begin(); i != mil.end(); ++i) {
    for (listIt=shownTasks->begin();listIt!=shownTasks->end(); ++listIt) {;
      if ( j == (*i).row() ) {
        //If i was editing, stop editing...
        if ( editId == (*listIt).getId() ) editMode = false;
        //If i was using it on the log, stop using it.
        tasklist->setSelected( (*listIt).getId() ,false );
        log->stopLog();
        //Delete it
        tasklist->deleteTask( (*listIt).getId() );
        break;
      }
      j++;
    }
  }

  refreshShownTasks();
  qDeleteAll(list->selectedItems());
  
}

// Action to deactivate a task
void TasksWdg::activateItem() {
  List::iterator listIt ;

  QItemSelectionModel *sm = list->selectionModel();
  QList<QModelIndex> mil = sm->selectedIndexes();
  int j = 0;
  for (QList<QModelIndex>::iterator i = mil.begin(); i != mil.end(); ++i) {
// NOTE: I don't have a random access iterator
    for (listIt=shownTasks->begin();listIt!=shownTasks->end(); ++listIt) {
      if ( j == (*i).row() ) {
        tasklist->setActive( (*listIt).getId(), !(*listIt).getActive());
        if ( (*listIt).getSelected() ) {
          tasklist->setSelected( (*listIt).getId(), false );
        }
        break;
      }
      j++;
    }
  }

  log->stopLog();
  
  fillTasks();
  
}

void TasksWdg::taskAddedExternally(task_id task) {
  fillTasks();
}

void TasksWdg::readSettings() {
  adjustSize();
  QRect scr = QApplication::desktop()->availableGeometry();
  move( settings->value("tasklist/pos", scr.center() - rect().center()).toPoint() );
  resize( settings->value("tasklist/size",QSize(320,240)).toSize() );
  int sortAux = settings->value("tasklist/sort",(int)ByDescription).toInt();
  sort = (ListSort)sortAux;
}

void TasksWdg::writeSettings() {
  settings->setValue("tasklist/pos", pos());
  settings->setValue("tasklist/size", size());
  settings->setValue("tasklist/sort",sort);
}

// Saves all tasks, hides the windows until
// the user closes the application from the tray
void TasksWdg::closeEvent( QCloseEvent *event )
{
  writeSettings();
  event->ignore();
  hide();
}

void TasksWdg::keyPressEvent(QKeyEvent *event) {
  if (event->key()==Qt::Key_Escape) hide();
}
