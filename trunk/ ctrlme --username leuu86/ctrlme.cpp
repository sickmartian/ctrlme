#include <QApplication>
#include "ventana.h"

#include <QSystemTrayIcon>
#include <QAction>
#include <QIcon>
#include <QMenu>

int main(int argc, char *argv[])
{
  // Inst. QApp
  QApplication app(argc, argv);
  // Global application style
  app.setStyleSheet("QLineEdit#linea { font: bold 16px; }"
  "QLineEdit#linea:focus { border-color: red; border-width: 2px; border-style: outset; }");

  MainWnd window;
  window.resize(480, 320);
//  window.show();
  
  QSystemTrayIcon tray;
  QAction showAct("show",&app);  
  QAction quitAct("quit",&app);
  QIcon icono("trash.svg");
  
  QMenu trayMenu(&window);
  trayMenu.addAction(&quitAct);
  trayMenu.addAction(&showAct);  
  tray.setContextMenu(&trayMenu);
  tray.setIcon(icono);
  tray.show();
  QObject::connect(&quitAct, SIGNAL(triggered()), &app, SLOT(quit()));
  QObject::connect(&showAct, SIGNAL(triggered()), &window, SLOT(show()));  
  
    /*
    QHBoxLayout layout;
    layout.addWidget(&hello);
    layout.addWidget(&leo);
    window.setLayout(&layout);

    QTimer timer(&app);
    QObject::connect(&timer, SIGNAL(timeout()), &app, SLOT(quit()));
    timer.start(10000);    */
  return app.exec();
}
