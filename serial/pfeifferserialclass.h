#ifndef PFEIFFERSERIALCLASS_H
#define PFEIFFERSERIALCLASS_H

#include <QTimer>
#include <QObject>
#include <QSerialPort>

class PfeifferSerialclass : public QObject
{
    Q_OBJECT
private:
    QTimer event_timer;
    QSerialPort *serial_port;

    QString port_name;
    QSerialPort::BaudRate baud_rate;

public:
    PfeifferSerialclass();
    ~PfeifferSerialclass();
signals:
    void ErrorString(const QString &error_string, bool status);

public slots:
    void setPortName(const QString &port_name);
    void setBaudRate(const QSerialPort::BaudRate baud_rate);

    bool checkState();

    void connectDevice();
    void disconnectDevice();
};

#endif // PFEIFFERSERIALCLASS_H
