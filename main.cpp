#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    qRegisterMetaType<PfeifferSerialclass::Sensor>("PfeifferSerialclass::Sensor");
    qRegisterMetaType<PfeifferSerialclass::PressureMeasurementStatus>("PfeifferSerialclass::PressureMeasurementStatus");

    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    return a.exec();
}
