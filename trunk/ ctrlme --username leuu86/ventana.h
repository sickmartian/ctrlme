#include <QDialog>
#include "tareas.h"

class MainWnd : public QDialog
{
  Q_OBJECT

public:
  MainWnd(QWidget *parent = 0);
  void closeEvent( QCloseEvent *event);
  
private:
  Tasks *tskmng;
};
