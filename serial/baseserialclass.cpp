#include <QDebug>

#define MAX_FAILED_ATTEMPTS         3

#include "baseserialclass.h"

void BaseSerialClass::setEventTimeInterval(
        const uint interval)
{
    event_timer.setInterval(interval);
}

void BaseSerialClass::setReconnectTimeInterval(
        const uint interval)
{
    reconnect_timer.setInterval(interval);
}

QString BaseSerialClass::serialPortName() const
{
    return port_name;
}

QSerialPort::Parity BaseSerialClass::parity() const
{
    return port_parity;
}

QSerialPort::BaudRate BaseSerialClass::baudRate() const
{
    return baud_rate;
}

QSerialPort::StopBits BaseSerialClass::stopBits() const
{
    return stop_bits;
}

QSerialPort::DataBits BaseSerialClass::dataBits() const
{
    return data_bits;
}

QSerialPort::FlowControl BaseSerialClass::flowControl() const
{
    return flow_control;
}

void BaseSerialClass::setSerialPortName(const QString &port_name)
{
    this->port_name = port_name;
}

void BaseSerialClass::setParity(const QSerialPort::Parity
                                               port_parity)
{
    this->port_parity = port_parity;
}

void BaseSerialClass::setBaudRate(const QSerialPort::BaudRate
                                                 baud_rate)
{
    this->baud_rate = baud_rate;
}

void BaseSerialClass::setStopBits(const QSerialPort::StopBits
                                                 stop_bits)
{
    this->stop_bits = stop_bits;
}

void BaseSerialClass::setDataBits(const QSerialPort::DataBits
                                                 data_bits)
{
    this->data_bits = data_bits;
}

void BaseSerialClass::setFlowControl(const QSerialPort::FlowControl
                                     flow_control)
{
    this->flow_control = flow_control;
}

void BaseSerialClass::clearRequestQueue()
{
    int len = request_queue.length();
    for (int i = 0; i < len; i++)
    {
        delete request_queue.at(i);
    }

    request_queue.clear();
}

bool BaseSerialClass::deviceDisconnected() const
{
    return !deviceConnected();
}

void BaseSerialClass::stopEventTimer(const bool stop)
{
    if (stop)
    {
        event_timer.stop();
    }
    else
    {
        event_timer.start();
    }
}

void BaseSerialClass::stopReconnectTimer(const bool stop)
{
    if (stop)
    {
        reconnect_timer.stop();
    }
    else
    {
        reconnect_timer.start();
    }
}

void BaseSerialClass::startEventTimer(const bool start)
{
    stopEventTimer(!start);
}

void BaseSerialClass::startReconnectTimer(const bool start)
{
    stopReconnectTimer(!start);
}

void BaseSerialClass::startEventLoop()
{
    connect(&event_timer,SIGNAL(timeout()),this,SLOT(eventLoop()));
    connect(&reconnect_timer,SIGNAL(timeout()),this,SLOT(connectDevice()));

    reconnect_timer.setSingleShot(false);
    event_timer.setSingleShot(false);

    connectDevice();

    if (!checkState())
    {
        disconnectDevice();
        reconnect_timer.start();
    }
    else
    {
        event_timer.start();
        reconnect_timer.stop();
    }
}

void BaseSerialClass::eventLoop()
{
    if (!deviceConnected())
    {
        event_timer.stop();
        reconnect_timer.start();
    }

    if (!checkState())
    {
        disconnectDevice();
        reconnect_timer.start();
        event_timer.stop();
        return;
    }

    if (request_queue.length() < 1)
    {
        return;
    }

    if (processPending())
    {
        manageReply();
    }
    else
    {
        processRequestQueue();
    }

    if (failed_attempts >= MAX_FAILED_ATTEMPTS)
    {
        failed_attempts = 0;
        disconnectDevice();
        reconnect_timer.start();
        event_timer.stop();
    }
}
