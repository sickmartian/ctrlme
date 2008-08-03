#include <QLineEdit>
#include <QFocusEvent>

#include "tasklist.h"

class LineEditTimer : public QLineEdit
{
  Q_OBJECT

public:
  LineEditTimer(TaskList *tasklist, QWidget *parent=0);

public slots:

signals:
  void gotFocus();

private:
  void focusInEvent ( QFocusEvent* );
  void findTask();
  bool event(QEvent*);
  TaskList *tasklist;
  List tasks;
  int lastIndex;
  QString searchWord;

};
