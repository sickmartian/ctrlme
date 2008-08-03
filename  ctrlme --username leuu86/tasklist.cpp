#include "tasklist.h"

/*
 SORT Functions
 */
bool compareDescription(Task first, Task second) {
	if (first.getDescription().toUpper() < second.getDescription().toUpper()) {
		return true;
	} else {
		return false;
	}
}

bool compareId(Task first, Task second) {
	if (first.getId() < second.getId()) {
		return true;
	} else {
		return false;
	}
}

bool compareLastUsage(Task first, Task second) {
	if (first.getLastUse() > second.getLastUse()) {
		return true;
	} else {
		return false;
	}
}

bool compareSelectedUsage(Task first, Task second) {
	if ((first.getSelected()) && (second.getSelected())) {
		if (first.getLastUse() > second.getLastUse()) {
			return true;
		} else {
			return false;
		}
	} else {
		if (first.getSelected()) {
			return true;
		} else {
			return false;
		}
	}
}

TaskList::TaskList(QSettings *settings) :
	settings(settings), idMananger(TaskId::getInstance()) {

	readSettings();

	tasks = new List;

	QFile file(filename);
	QDomDocument doc("Tasks");
	if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
		if (doc.setContent(&file)) {
			file.close();
			QDomNodeList tareas = doc.elementsByTagName("Task");
			for (int i = 0; i < tareas.size(); ++i) {
				appendTask(tareas.item(i).toElement());
			}
		}
	}
}

void TaskList::readSettings() {
	setFilename(settings->value("intTaskList/filename", "tasks.xml").toString());
	Task::setLastUseTime(
			settings->value("intTaskList/lastUseTime", 604800).toInt());
}

void TaskList::writeSettings() {
	settings->setValue("intTaskList/filename", getFilename());
	settings->setValue("intTaskList/lastUseTime", Task::getLastUseTime());
}

TaskList::~TaskList() {
	writeSettings();
	saveToXML();
	delete tasks;
}

Task TaskList::getTaskCopy(task_id id) const {
	List::const_iterator i;
	Task t;
	for (i = tasks->begin(); i != tasks->end(); ++i) {
		if ((*i).getId() == id) {
			return (*i);
		}
	}
	return t;
}

Task TaskList::getTaskCopy(QString description) const {
	List::const_iterator i;
	Task t;
	//First we try with the exact case
	for (i = tasks->begin(); i != tasks->end(); ++i) {
		if ((*i).getDescription().simplified() == description.simplified()) {
			return (*i);
		}
	}
	//Then we try with any case
	for (i = tasks->begin(); i != tasks->end(); ++i) {
		if ((*i).getDescription().toUpper().simplified() == description.toUpper().simplified()) {
			return (*i);
		}
	}
	return t;
}

List::iterator TaskList::getTaskRef(task_id id, bool *ok) const {
	List::iterator i;
	for (i = tasks->begin(); i != tasks->end(); ++i) {
		if ((*i).getId() == id) {
			(*ok) = true;
			return i;
		}
	}
	(*ok) = false;
	return i;
}

List TaskList::getTaskList(ListSort sort, bool onlyActive) const {

	List sortedTasks;

	for (List::const_iterator i = tasks->begin(); i != tasks->end(); ++i) {
		if ((*i).getActive() && onlyActive) {
			sortedTasks.push_back((*i));
		} else if (!(onlyActive)) {
			sortedTasks.push_back((*i));
		}
	}

	switch (sort) {

	case ById:
		sortedTasks.sort(compareId);
		break;
	case ByDescription:
		sortedTasks.sort(compareDescription);
		break;
	case ByLastUse:
		sortedTasks.sort(compareLastUsage);
		break;
	case BySelectedFirstLastUse:
		sortedTasks.sort(compareSelectedUsage);
		break;
	}

	return sortedTasks;

}

bool TaskList::saveToXML() const {
	QDomDocument doc("Nombre_doc");
	QDomElement root = doc.createElement("Tasks");
	doc.appendChild(root);
	for (List::const_iterator i = tasks->begin(); i != tasks->end(); ++i) {
		root.appendChild((*i).getElement());
	}

	QFile file(filename);
	if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
		QTextStream out(&file);
		out << "<?xml version='1.0'?>";
		out << doc.toString();
		file.close();
		return true;
	} else {
		return false;
	}
}

bool TaskList::checkTaskDesc(QString description) const {
	QRegExp notOnlySpaces("\\w");

	return description.contains(notOnlySpaces);
}

bool TaskList::appendTask(QString description, task_id *newId, bool selected) {

	if (checkTaskDesc(description)) {
		(*newId) = idMananger.getTaskId();
		Task dummy(description, (*newId), selected);
		tasks->push_back(dummy);
		emit taskAdded(dummy.getId());
		return true;
	} else {
		return false;
	}

}

void TaskList::appendTask(QDomElement element) {
	Task dummy(element);
	tasks->push_back(dummy);
	emit taskAdded(dummy.getId());
}

bool TaskList::setDescription(task_id id, QString description) {
	bool ok;
	if (checkTaskDesc(description)) {
		List::iterator i = getTaskRef(id, &ok);
		if (ok) {
			(*i).setDescription(description);
			emit taskAdded((*i).getId());
			return true;
		} else {
			return false;
		}
	} else {
		return false;
	}
}

void TaskList::setLastUse(task_id id) {
	bool ok;
	List::iterator i = getTaskRef(id, &ok);
	if (ok) {
		(*i).setLastUse();
	}
}

bool TaskList::getSelected(task_id id) const {
	return getTaskCopy(id).getSelected();
}

bool TaskList::getActive(task_id id) const {
	return getTaskCopy(id).getActive();
}

Task TaskList::getTask(task_id id) const {
	return getTaskCopy(id);
}

Task TaskList::getTask(QString description) const {
	return getTaskCopy(description);
}

void TaskList::setSelected(task_id id, bool opt) {
	bool ok;
	List::iterator i = getTaskRef(id, &ok);
	if (ok) {
		(*i).setSelected(opt);
	}
}

void TaskList::setActive(task_id id, bool opt) {
	bool ok;
	List::iterator i = getTaskRef(id, &ok);
	if (ok) {
		(*i).setActive(opt);
		emit taskDeleted(id);
	}
}

void TaskList::deleteTask(task_id id) {
	List::iterator i;
	for (i = tasks->begin(); i != tasks->end(); ++i) {
		if ((*i).getId() == id) {
			i = tasks->erase(i);
			emit taskDeleted(id);
			break;
		}
	}
}

int TaskList::size() const {
	return tasks->size();
}

void TaskList::clearSelections() {
	for (List::iterator i = tasks->begin(); i != tasks->end(); ++i) {
		(*i).setSelected(false);
	}
}

void TaskList::setFilename(QString filename) {
	this->filename = filename;
}

QString TaskList::getFilename() const {
	return this->filename;
}
