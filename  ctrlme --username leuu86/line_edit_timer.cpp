#include "line_edit_timer.h"

LineEditTimer::LineEditTimer(TaskList *tasklist, QWidget *parent)
: QLineEdit(parent),
	tasklist(tasklist)
{
  setFocusPolicy( Qt::StrongFocus );
  lastIndex = -1;
  searchWord = "";
}

void LineEditTimer::focusInEvent ( QFocusEvent *event ) {
  if ( event->gotFocus() ) {
    emit gotFocus();
  }
  QLineEdit::focusInEvent(event);
}

void LineEditTimer::findTask() {
	if (this->text()!="") {
		if ( (searchWord=="") || isModified() ){
			//If it's a new word, reset the index
			searchWord = this->text();
			lastIndex = -1;
			tasks = tasklist->getTaskList(ByDescription,false);
		}
		int j = 0;
		for (List::const_iterator i=tasks.begin(); i!=tasks.end(); i++) {
			if ( (j>lastIndex) && (*i).getDescription().startsWith(searchWord,Qt::CaseInsensitive) ) {
				setText((*i).getDescription());
				lastIndex = j;
				break;
			}
			j++;
		}
	}
}
bool LineEditTimer::event(QEvent *event) {
	if (event->type() == QEvent::KeyPress) {
		QKeyEvent *ke = static_cast<QKeyEvent *>(event);
		if (ke->key() == Qt::Key_Tab) {
			findTask();
			return true;
		}

	}
	return QLineEdit::event(event);
}
