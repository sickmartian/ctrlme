# #####################################################################
# Automatically generated by qmake (2.01a) dom 8. jun 16:57:03 2008
# #####################################################################
TEMPLATE = app
TARGET = ctrlme
CONFIG += qt \
    warn_on \
    release \
    no_mocdepend
DEPENDPATH += .
INCLUDEPATH += .
QT += xml \
    core \
    gui
RC_FILE = icon.rc

# Input
HEADERS += clickable_label.h \
    graphic_task.h \
    timeline.h \
    tasklist_style.h \
    tasks_wdg.h \
    task.h \
    tasklist.h \
    inputtimewnd.h \
    taskid.h \
    log.h \
    tray_menu.h \
    line_edit_timer.h \
    entry_id.h \
    ask_date_interval.h \
    reporting.h \
    settings.h
SOURCES += clickable_label.cpp \
    graphic_task.cpp \
    timeline.cpp \
    tasklist_style.cpp \
    ctrlme.cpp \
    tasks_wdg.cpp \
    task.cpp \
    tasklist.cpp \
    inputtimewnd.cpp \
    taskid.cpp \
    log.cpp \
    tray_menu.cpp \
    line_edit_timer.cpp \
    entry_id.cpp \
    ask_date_interval.cpp \
    reporting.cpp \
    settings.cpp
RESOURCES = icons.qrc
