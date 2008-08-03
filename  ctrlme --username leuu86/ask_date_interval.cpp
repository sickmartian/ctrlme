#include "ask_date_interval.h"

AskDateInterval::AskDateInterval() {

  //Instances
  beginDate = new QDateEdit(QDate::currentDate(),this);
  endDate = new QDateEdit(QDate::currentDate(),this);
  vLayout = new QVBoxLayout(this);
  hLayout = new QHBoxLayout();
  fromLbl = new QLabel("From: ",this);
  toLbl = new QLabel("To: ",this);
  buttonsLayout = new QHBoxLayout();
  acceptBtn = new QPushButton("Accept",this);
  cancelBtn = new QPushButton("Cancel",this);
  
  //Add date controls
  hLayout->addWidget(fromLbl);
  hLayout->addWidget(beginDate,0);
  hLayout->addWidget(toLbl);
  hLayout->addWidget(endDate,0);
  //Add buttons
  buttonsLayout->addWidget(acceptBtn);
  buttonsLayout->addWidget(cancelBtn);
  //Add layouts
  vLayout->addLayout(hLayout);
  vLayout->addLayout(buttonsLayout);

  setLayout(vLayout);

  QObject::connect(acceptBtn, SIGNAL(clicked()), this, (SLOT(accept())));
  QObject::connect(cancelBtn, SIGNAL(clicked()), this, (SLOT(cancel())));
}

void AskDateInterval::accept() {
  emit datesSetted(beginDate->date(),endDate->date(),true);
  hide();
}

void AskDateInterval::cancel() {
  QDate nullDate;
  emit datesSetted(nullDate,nullDate,false);
  hide();
}

void AskDateInterval::closeEvent( QCloseEvent *event )
{
  event->ignore();
  hide();
}
