
#include <QDebug>
#include <QApplication>
#include <QTranslator>
#include <QTextCodec>

#include "mainwindow.h"


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

//    QTextCodec::setCodecForTr(QTextCodec::codecForName("utf8"));

    QTranslator translator;
    translator.load("lang/ffdui_hr");
    a.installTranslator(&translator);

    MainWindow w;
    w.show();
    return a.exec();
}
