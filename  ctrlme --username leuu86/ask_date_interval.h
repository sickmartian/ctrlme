#ifndef ASK_DATE_INTERVAL
  #define ASK_DATE_INTERVAL

#include <QWidget>
#include <QDateEdit>
#include <QBoxLayout>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QLabel>
#include <QCloseEvent>

class AskDateInterval : public QWidget
{
  Q_OBJECT
  
public:
  AskDateInterval();

public slots:
  void accept();
  void cancel();

signals:
  void datesSetted(QDate,QDate,bool);
  
private:
  void closeEvent(QCloseEvent*);
  
  QDateEdit *beginDate;
  QDateEdit *endDate;
  QVBoxLayout *vLayout;
  QHBoxLayout *hLayout;
  QHBoxLayout *buttonsLayout;
  QBoxLayout *layout;
  QPushButton *acceptBtn;
  QPushButton *cancelBtn;
  QLabel *fromLbl;
  QLabel *toLbl;
  
};

#endif
