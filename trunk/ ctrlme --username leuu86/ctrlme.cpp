#include <QApplication>
#include <QObject>
#include <QTextCodec>

#include "tray_menu.h"

using namespace std;

int main(int argc, char *argv[])
{
  // Inst. QApp
  QApplication app(argc, argv);
  
  QTextCodec::setCodecForTr(QTextCodec::codecForName("UTF-8"));
  QTextCodec::setCodecForCStrings(QTextCodec::codecForName("UTF-8"));
  QTextCodec::setCodecForLocale(QTextCodec::codecForName("UTF-8"));
  
  // Global application style
  app.setStyleSheet("QLineEdit#linea { font: bold 16px; }"
                    "QLineEdit#linea:focus { border-color: red; border-width: 2px; border-style: outset; }");

  TrayMenu tm;
  tm.show();
  
  QObject::connect(&tm, SIGNAL(quit()), &app, SLOT(quit()));
  
  return app.exec();
}
