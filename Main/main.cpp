#include "mainwindow.h"
#include<QtDebug>
#include<QDebug>

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;

    w.resize(400, 200);
    w.show();
    return a.exec();
}
