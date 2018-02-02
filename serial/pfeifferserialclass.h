#ifndef PFEIFFERSERIALCLASS_H
#define PFEIFFERSERIALCLASS_H

#include <QTimer>
#include <QVector>
#include <QObject>
#include <QSerialPort>

struct PfeifferSerialStruct;

class PfeifferSerialclass : public QObject
{
    Q_OBJECT
private:
    PfeifferSerialStruct *private_struct;

    QTimer event_timer;
    QTimer reconnect_timer;
    QSerialPort *serial_port;

    QString port_name;
    QSerialPort::BaudRate baud_rate;

public:
    PfeifferSerialclass();
    ~PfeifferSerialclass();

    QString PortName() const;
    QSerialPort::FlowControl FlowControl() const;
    QSerialPort::BaudRate BaudRate() const;
    QSerialPort::StopBits StopBits() const;
    QSerialPort::DataBits DataBits() const;
    QSerialPort::Parity Parity() const;

signals:
    void ErrorString(const QString &error_string, bool status);

public slots:
    void processSerialRequestQueue();

    void setPortName(const QString &port_name);
    void setBaudRate(const QSerialPort::BaudRate baud_rate);

    bool checkState();

    void connectDevice();
    void disconnectDevice();

    //void
};

#endif // PFEIFFERSERIALCLASS_H
