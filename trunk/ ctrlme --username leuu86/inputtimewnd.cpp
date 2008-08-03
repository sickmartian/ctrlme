#include "inputtimewnd.h"

InputTimeWnd::InputTimeWnd(TaskList *tasklist, Log *logInstance,
		QSettings *settings, QWidget *parent) :
	QWidget(parent), settings(settings), logInstance(logInstance), tasklist(
			tasklist) {

	//Creating instances
	timer = new QTimer();
	subTimer = new QTimer();

	savedTaskList = new List;
	shownTaskList = new List;
	buttonList = new BList;

	sa = new QScrollArea(this);
	window = new QWidget(sa);
	layout = new QVBoxLayout(window);
	layout_out = new QVBoxLayout(this);
	le = new LineEditTimer(tasklist,this);
	le->setObjectName("linea");
	acceptBtn = new QPushButton("Accept", this);

	//Let the user add a task from this widget
	QObject::connect(le, SIGNAL(returnPressed()), this, SLOT(addTask()));
	QObject::connect(tasklist, SIGNAL(taskAdded(task_id)), this, SLOT(taskAddedExternally(task_id)));
	QObject::connect(tasklist, SIGNAL(taskDeleted(task_id)), this, SLOT(taskDeletedExternally(task_id)));

	//Setting the layout
	layout->setSizeConstraint(QLayout::SetNoConstraint);
	setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);
	sa->setWidgetResizable(true);
	sa->setWidget(window);
	layout_out->addWidget(acceptBtn);
	layout_out->addWidget(le);
	layout_out->addWidget(sa);
	setLayout(layout_out);

	window->show();
	sa->show();
	le->show();

	setWindowFlags(Qt::Tool | Qt::WindowStaysOnTopHint);

	//Actions
	explicitAcceptActEnt = new QAction(le);
	explicitAcceptActRet = new QAction(le);
	explicitAcceptActEnt->setShortcut(Qt::CTRL + Qt::Key_Enter);
	explicitAcceptActRet->setShortcut(Qt::CTRL + Qt::Key_Return);
	connect(explicitAcceptActEnt, SIGNAL(triggered()), this, SLOT(explicitAceptance()));
	connect(acceptBtn, SIGNAL(pressed()), explicitAcceptActEnt, SIGNAL(triggered()));
	connect(explicitAcceptActRet, SIGNAL(triggered()), this, SLOT(explicitAceptance()));
	addAction(explicitAcceptActRet);
	addAction(explicitAcceptActEnt);

	//Timers
	QObject::connect(timer, SIGNAL(timeout()), this, SLOT(showWithTimeout()));
	QObject::connect(subTimer, SIGNAL(timeout()), this, SLOT(implicitAceptance()));

	//Focus
	setFocusPolicy(Qt::StrongFocus);
	sa->setFocusPolicy(Qt::StrongFocus);
	sa->setFocusProxy(this);
	QObject::connect(le, SIGNAL(gotFocus()), this, SLOT(gotFocus()));

	//Various
	readSettings();
	fillTasks();
	setAskForInput(askForInput);

}

void InputTimeWnd::readSettings() {
	askInterval = settings->value("itw/askInterval", 1200000).toInt();
	askTimeOut = settings->value("itw/askTimeOut", 20000).toInt();
	onlyOneTask = settings->value("itw/onlyOneTask", false).toBool();
	btnHeight = settings->value("itw/btnHeight", 50).toInt();
	int sortAux =
			settings->value("itw/sort", (int) BySelectedFirstLastUse).toInt();
	sort = (ListSort) sortAux;
	askForInput = settings->value("itw/autoLogOnStart", true).toBool();
	acceptBtnVisible = settings->value("itw/acceptBtnVisible", true).toBool();
	if (acceptBtnVisible) {
		acceptBtn->show();
		acceptBtn->setMinimumHeight(btnHeight);
	} else {
		acceptBtn->hide();
	}
	int w = QApplication::desktop()->availableGeometry().width();
	setMaximumWidth(w / settings->value("itw/maximumWidth", 8).toInt());
	int h = QApplication::desktop()->availableGeometry().height();
	setMaximumHeight(h / settings->value("itw/maximumHeight", 2).toInt());
}

void InputTimeWnd::writeSettings() {
	settings->setValue("itw/askInterval", askInterval);
	settings->setValue("itw/askTimeOut", askTimeOut);
	settings->setValue("itw/onlyOneTask", onlyOneTask);
	settings->setValue("itw/btnHeight", btnHeight);
	settings->setValue("itw/acceptBtnVisible", acceptBtnVisible);
	settings->setValue("itw/sort", sort);
}

//GETTERS

int InputTimeWnd::getAskInterval() const {
	//To minutes
	return askInterval / 60000;
}

int InputTimeWnd::getAskTimeOut() const {
	//To seconds
	return askTimeOut / 1000;
}

//SETTERS

void InputTimeWnd::setAskInterval(int interval) {
	askInterval = interval * 60000;
	if (askForInput) {
		timer->setInterval(askInterval);
	}
}

void InputTimeWnd::setAskTimeOut(int interval) {
	askTimeOut = interval * 1000;
}

InputTimeWnd::~InputTimeWnd() {
	writeSettings();
	delete subTimer;
	delete timer;
	delete shownTaskList;
	delete buttonList;
}

void InputTimeWnd::startTimer() {
	if (askForInput) {
		timer->start(askInterval);
	}
}

void InputTimeWnd::stopTimer() {
	timer->stop();
}

void InputTimeWnd::setAskForInput(bool ask) {
	askForInput = ask;
	if (askForInput) {
		//  Before we just started the timer, now we ask the user for the first input here too...
		showWithTimeout();
	} else {
		stopTimer();
		stopSubTimer();
	}
}

void InputTimeWnd::startSubTimer() {
	//ONLY if it's automatic
	if (askForInput) {
		subTimer->start(askTimeOut);
	}
}

void InputTimeWnd::stopSubTimer() {
	subTimer->stop();
}
void InputTimeWnd::showWithTimeout() {
	stopTimer();
	startSubTimer();
	QWidget *father = this;
	father->show();
}

void InputTimeWnd::show() {
	stopTimer();
	QWidget *father = this;
	father->show();
}

void InputTimeWnd::hide() {
	stopSubTimer();
	startTimer();
	QWidget *father = this;
	father->hide();
}

void InputTimeWnd::logSelectedTasks() {
	for (List::const_iterator i = shownTaskList->begin(); i!= shownTaskList->end(); ++i) {
		if ((*i).getSelected() == true) {
			//If it's selected, we add it to the log and update the last logged timestamp
			logInstance->write((*i));
			tasklist->setLastUse((*i).getId());
		}
		//We update the general tasklist
		tasklist->setSelected((*i).getId(), (*i).getSelected());
	}
	logInstance->stopLog();
}

void InputTimeWnd::explicitAceptance() {
	//If we have a task to add on the LineEdit, add it.
	addTaskInt();
	logSelectedTasks();
	hide();
}

void InputTimeWnd::implicitAceptance() {
	//If the timer is done and the window doesn't have a focus (ie. the user wasn't trying to select anything)
	if ((!hasFocus()) && (isVisible())) {
		hide();
	}
}

void InputTimeWnd::backUpTaskList() {
	*savedTaskList = tasklist->getTaskList(ById,false);
}

void InputTimeWnd::restoreTaskList() {
	bool found = false;
	List l = tasklist->getTaskList(ById,true);
	for (List::const_iterator j=l.begin(); j!=l.end(); j++) {
		for (List::const_iterator i=savedTaskList->begin(); i!=savedTaskList->end(); i++) {
			if ( (*i).getId() == (*j).getId() ) {
				tasklist->setActive((*i).getId(),(*i).getActive());
				tasklist->setSelected((*i).getId(),(*i).getSelected());
				found = true;
			}
		}
		if (!found) tasklist->setSelected((*j).getId(),false);
		found = false;
	}
}

void InputTimeWnd::deleteTaskButtons() {
	//Delete all buttons from the scroll area and the internal list of buttons
	BList::iterator bListIterator;
	for (bListIterator = buttonList->begin(); bListIterator!= buttonList->end(); ++bListIterator) {
		layout->removeWidget((*bListIterator));
		delete (*bListIterator);
	}
	buttonList->clear();
}

void InputTimeWnd::createTaskButtons() {
	int j = 0;
	for (List::const_iterator i = shownTaskList->begin(); i!= shownTaskList->end(); ++i) {
		QPushButton *dummy = new QPushButton((*i).getDescription(), window);
		dummy->setFlat(true);
		dummy->setCheckable(true);
		dummy->setChecked((*i).getSelected());
		dummy->setFocusProxy(this);
		buttonList->push_back(dummy);
		QObject::connect(buttonList->at(j), SIGNAL( toggled(bool) ), this, SLOT( buttonStatusRefresh() ));
		layout->addWidget(buttonList->at(j));
		dummy->show();
		++j;
	}
}

void InputTimeWnd::updateShownTaskList() {
	List l = tasklist->getTaskList(sort);
	for (List::iterator i=l.begin();i!=l.end();i++) {
		for (List::const_iterator j=shownTaskList->begin();j!=shownTaskList->end();j++) {
			if ( (*i).getId() == (*j).getId() ) {
				(*i).setSelected((*j).getSelected());
				(*i).setActive((*j).getActive());
				break;
			}
		}
	}
	delete shownTaskList;
	shownTaskList = new List;
	*shownTaskList = l;
}

void InputTimeWnd::deleteShownTasks() {
	delete shownTaskList;
	shownTaskList = new List;
}

void InputTimeWnd::fillTasks() {
	deleteTaskButtons();
	updateShownTaskList();
	createTaskButtons();
}

void InputTimeWnd::buttonStatusRefresh() {
	/* Assuming same number of buttons as the number of shownTaskList */
	BList::const_iterator j = buttonList->begin();

	for (List::iterator i = shownTaskList->begin(); i != shownTaskList->end(); ++i) {
		if ((*i).getSelected() != (**j).isChecked()) {
			(*i).setSelected((**j).isChecked());
		}
		j++;
	}

}

void InputTimeWnd::taskAddedExternally(task_id task) {
	fillTasks();
}

void InputTimeWnd::taskDeletedExternally(task_id task) {
	fillTasks();
}

void InputTimeWnd::addTask() {
	if (le->text() == "") {
		logSelectedTasks();
		hide();
	}
	addTaskInt();
}

void InputTimeWnd::addTaskInt() {
	task_id newId;
	if (!(le->text() == "")) {

		// Set selected of all buttons to false
		if (onlyOneTask) {
			for (List::iterator i = shownTaskList->begin(); i != shownTaskList->end(); ++i) {
				(*i).setSelected(false);
			}
		}

		//If the task exist we use it..
		Task existingTask = tasklist->getTask(le->text());
		if (existingTask.getId() != 0) {
			//If it's on the list we select it directly
			for (List::iterator i=shownTaskList->begin(); i!=shownTaskList->end(); i++) {
				if ( (*i).getId() == existingTask.getId() ) {
					(*i).setSelected(true);
				}
			}
			//If it's inactive, we activate it and select it
			if (!existingTask.getActive()) {
				tasklist->setSelected(existingTask.getId(),true);
				tasklist->setActive(existingTask.getId(),true);
			}
			else {
			//When we activate a task all is refresh automatically. If the task was active we have to refresh manually.
				fillTasks();
			}
			le->setText("");
		}
		else
		{
			// Add the task if possible
			if (tasklist->appendTask(le->text(), &newId, true)) {
				le->setText("");
				refreshGeometry();
			}
		}
	}
}

void InputTimeWnd::closeEvent(QCloseEvent *event) {
	writeSettings();
	event->ignore();
	hide();
}

void InputTimeWnd::focusInEvent(QFocusEvent *event) {
	if (event->gotFocus()) {
		stopSubTimer();
		le->setFocus();
	}
}

void InputTimeWnd::gotFocus() {
	stopSubTimer();
}

void InputTimeWnd::showEvent(QShowEvent *event) {
	//On every actualization refresh the geometry
	refreshGeometry();
	backUpTaskList();
	deleteShownTasks();
	fillTasks();
}

void InputTimeWnd::refreshGeometry() {
	QRect scr = QApplication::desktop()->availableGeometry();
	resize(int(layout->sizeHint().width() * 1.5),
			int(layout->sizeHint().height() * 1.5 + acceptBtn->height()));
	move(QPoint((scr.right() - width() - 15), (scr.bottom() - height() - 30)));
}

bool InputTimeWnd::getOnlyOneTask() const {
	return onlyOneTask;
}

void InputTimeWnd::setOnlyOneTask(bool onlyOne) {
	onlyOneTask = onlyOne;
}

bool InputTimeWnd::getAskForInput() const {
	return askForInput;
}

void InputTimeWnd::keyPressEvent(QKeyEvent *event) {
	if (event->key() == Qt::Key_Escape) {
		restoreTaskList();
		hide();
	}
}
