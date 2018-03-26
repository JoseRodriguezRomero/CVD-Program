#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    qRegisterMetaType<PfeifferSerialclass::Sensor>("PfeifferSerialclass::Sensor");
    qRegisterMetaType<PfeifferSerialclass::PressureMeasurementStatus>("PfeifferSerialclass::PressureMeasurementStatus");

    qRegisterMetaType<MKSSerialClass::Channel>("MKSSerialClass::Channel");

    qRegisterMetaType<QSerialPort::Parity>("QSerialPort::Parity");
    qRegisterMetaType<QSerialPort::Parity>("QSerialPort::Parity");
    qRegisterMetaType<QSerialPort::StopBits>("QSerialPort::StopBits");
    qRegisterMetaType<QSerialPort::DataBits>("QSerialPort::DataBits");
    qRegisterMetaType<QSerialPort::BaudRate>("QSerialPort::BaudRate");

    qRegisterMetaType<QSerialPort::SerialPortError>("QSerialPort::SerialPortError");

    QApplication a(argc, argv);
    MainWindow w;
    w.initializeSerialDevices();
    w.show();

    return a.exec();
}
