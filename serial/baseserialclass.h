#ifndef BASESERIALCLASS_H
#define BASESERIALCLASS_H

#include <QSerialPort>
#include <QVector>
#include <QString>
#include <QObject>
#include <QTimer>

class BaseSerialClass : public QObject
{
    Q_OBJECT
protected:
    QVector<void*> request_queue;  // QObjects do not support templating....

    QSerialPort *serial_port;
    QByteArray buffer;

    QString port_name;
    QSerialPort::BaudRate baud_rate;
    QSerialPort::Parity port_parity;
    QSerialPort::StopBits stop_bits;
    QSerialPort::DataBits data_bits;
    QSerialPort::FlowControl flow_control;

    QTimer event_timer;
    QTimer reconnect_timer;

    uint failed_attempts;

public:
    void setEventTimeInterval(const uint interval);             // in msec
    void setReconnectTimeInterval(const uint interval);         // in msec

    virtual bool deviceConnected() const;
    virtual bool deviceDisconnected() const;

public slots:
    QString serialPortName() const;
    QSerialPort::Parity parity() const;
    QSerialPort::BaudRate baudRate() const;
    QSerialPort::StopBits stopBits() const;
    QSerialPort::DataBits dataBits() const;
    QSerialPort::FlowControl flowControl() const;

    void setSerialPortName(const QString &port_name);
    void setParity(const QSerialPort::Parity port_parity);
    void setBaudRate(const QSerialPort::BaudRate baud_rate);
    void setStopBits(const QSerialPort::StopBits stop_bits);
    void setDataBits(const QSerialPort::DataBits data_bits);
    void setFlowControl(const QSerialPort::FlowControl flow_control);

    virtual bool processPending() const = 0;

    virtual void manageReply() = 0;
    virtual void processRequestQueue() = 0;

    virtual void connectDevice();
    virtual void disconnectDevice();

    virtual bool checkState();
    void clearRequestQueue();

    void startEventLoop();

signals:
    void errorString(const QString &error_string, bool status);

    void stopEventLoopTimer();
    void startEventLoopTimer();

    void stopReconnectTimer();
    void startReconnectTimer();

    void deviceConnected(const QSerialPort::SerialPortError error);

private slots:
    void eventLoop();
};

#endif // BASESERIALCLASS_H
