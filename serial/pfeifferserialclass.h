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

    QSerialPort *serial_port;
    QTimer reconnect_timer;
    QTimer event_timer;

    QString port_name;
    QSerialPort::BaudRate baud_rate;

public:
    PfeifferSerialclass(QObject *parent);
    ~PfeifferSerialclass();

    QString PortName() const;
    QSerialPort::FlowControl FlowControl() const;
    QSerialPort::BaudRate BaudRate() const;
    QSerialPort::StopBits StopBits() const;
    QSerialPort::DataBits DataBits() const;
    QSerialPort::Parity Parity() const;

signals:
    void ErrorString(const QString &error_string, bool status);
    void deviceConnected(QSerialPort::SerialPortError);

public slots:
    void processSerialRequestQueue();

    void setPortName(const QString &port_name);
    void setBaudRate(const QSerialPort::BaudRate baud_rate);

    bool checkState();

    void connectDevice();
    void disconnectDevice();

private slots:
    void ManageReply();
};

#endif // PFEIFFERSERIALCLASS_H
