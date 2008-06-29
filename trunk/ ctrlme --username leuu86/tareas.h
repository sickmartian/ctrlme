#include <QWidget>
#include <QLineEdit>
#include <QListWidget>
#include <QString>
#include <QStringList>
#include <QAction>
#include <QContextMenuEvent>
#include <QPoint>

class Tasks : public QWidget
{
  Q_OBJECT
  
public:
  Tasks(QWidget *parent = 0);
  QString* getItems(int *);
  void addATask(QString tarea);
  
public slots:
  void addTask();
  void listMenu(const QPoint &);
  void borrarItem();

private:
  QLineEdit *text;
  QListWidget *list;
  QAction *deleteAct;
};
