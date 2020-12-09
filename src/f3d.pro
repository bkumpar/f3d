#-------------------------------------------------
#
# Project created by QtCreator 2012-12-10T13:57:03
#
#-------------------------------------------------

QT += core gui
QT += widgets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = f3d
TEMPLATE = app


SOURCES += main.cpp\
    directorylist.cpp \
    mainwindow.cpp \
    fileinfowidget.cpp \
    browsedirectorydialog.cpp \
    timedisplay.cpp \
    optionswidget.cpp \
    spinboxex.cpp \
    clickablelabel.cpp \
    actionswidget.cpp \
    filevisitor.cpp \
    filebrowser.cpp \
    exceptions.cpp \
    popupbutton.cpp \
    duplicatesfinder.cpp \
    duplicatestreeview.cpp \
    collector.cpp \
    filemanager.cpp

HEADERS  += mainwindow.h \
    directorylist.h \
    fileinfowidget.h \
    browsedirectorydialog.h \
    timedisplay.h \
    optionswidget.h \
    spinboxex.h \
    clickablelabel.h \
    actionswidget.h \
    filevisitor.h \
    popupbutton.h \
    duplicatesfinder.h \
    duplicatestreeview.h \
    collector.h \
    filemanager.h

FORMS +=

OTHER_FILES += \
    READ_ME.txt \
    ../ffdui-build-desktop-Qt_4_8_1_in_PATH__System__Release/Makefile \
    stylesheets.txt \
    internationalisation.txt

RESOURCES += \
    icons.qrc

win32 {
CONFIG -= embed_manifest_exe
RC_FILE = f3d.rc
}

TRANSLATIONS = ffdui_hr.ts \
               ffdui_en.ts \
               ffdui_de.ts \
               ffdui_fr.ts

DEFINES += DEBUG

CODECFORTR = UTF-8

DISTFILES += \
    EmbededManifest.txt \
    f3d.exe.manifest \
    f3d.rc

