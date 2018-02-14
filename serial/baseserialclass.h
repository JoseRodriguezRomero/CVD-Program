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

    virtual bool deviceConnected() const = 0;
    virtual bool deviceDisconnected() const = 0;

public slots:
    QString serialPortName() const;
    QSerialPort::Parity parity() const;
    QSerialPort::BaudRate baudRate() const;
    QSerialPort::StopBits stopBits() const;
    QSerialPort::DataBits dataBits() const;
    QSerialPort::FlowControl flowControl() const;

    virtual void setSerialPortName(const QString &port_name);
    virtual void setParity(const QSerialPort::Parity port_parity);
    virtual void setBaudRate(const QSerialPort::BaudRate baud_rate);
    virtual void setStopBits(const QSerialPort::StopBits stop_bits);
    virtual void setDataBits(const QSerialPort::DataBits data_bits);
    virtual void setFlowControl(const QSerialPort::FlowControl flow_control);

    virtual bool processPending() const = 0;

    virtual void manageReply() = 0;
    virtual void processRequestQueue() = 0;

    virtual void connectDevice() = 0;
    virtual void disconnectDevice() = 0;

    virtual bool checkState() = 0;
    void clearRequestQueue();

    void stopEventTimer(const bool stop = true);
    void stopReconnectTimer(const bool stop = true);

    void startEventTimer(const bool start = true);
    void startReconnectTimer(const bool start = true);

    void startEventLoop();

signals:
    void ErrorString(const QString &error_string, bool status);

private slots:
    void eventLoop();
};

#endif // BASESERIALCLASS_H
