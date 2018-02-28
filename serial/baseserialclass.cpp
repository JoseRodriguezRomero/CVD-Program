#define MAX_FAILED_ATTEMPTS         3

#include <QDebug>

#include "baseserialclass.h"

void BaseSerialClass::setEventTimeInterval(const uint interval)
{
    event_timer.setInterval(interval);
}

void BaseSerialClass::setReconnectTimeInterval(const uint interval)
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

void BaseSerialClass::setParity(const QSerialPort::Parity port_parity)
{
    this->port_parity = port_parity;
}

void BaseSerialClass::setBaudRate(const QSerialPort::BaudRate baud_rate)
{
    this->baud_rate = baud_rate;
}

void BaseSerialClass::setStopBits(const QSerialPort::StopBits stop_bits)
{
    this->stop_bits = stop_bits;
}

void BaseSerialClass::setDataBits(const QSerialPort::DataBits data_bits)
{
    this->data_bits = data_bits;
}

void BaseSerialClass::setFlowControl(
        const QSerialPort::FlowControl flow_control)
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

void BaseSerialClass::startEventLoop()
{
    connect(&event_timer,SIGNAL(timeout()),this,SLOT(eventLoop()));
    connect(&reconnect_timer,SIGNAL(timeout()),this,SLOT(connectDevice()));

    connect(this,SIGNAL(startReconnectTimer()),&reconnect_timer,SLOT(start()));
    connect(this,SIGNAL(stopReconnectTimer()),&reconnect_timer,SLOT(stop()));
    connect(this,SIGNAL(startEventLoopTimer()),&event_timer,SLOT(start()));
    connect(this,SIGNAL(stopEventLoopTimer()),&event_timer,SLOT(stop()));

    connectDevice();
    failed_attempts = 0;

    if (!checkState())
    {
        disconnectDevice();
        emit startReconnectTimer();
        emit stopEventLoopTimer();
    }
    else
    {
        emit startEventLoopTimer();
        emit stopReconnectTimer();
    }
}

void BaseSerialClass::eventLoop()
{
    if (!deviceConnected())
    {
        emit stopEventLoopTimer();
        emit startReconnectTimer();
    }

    if (!checkState())
    {
        disconnectDevice();
        emit startReconnectTimer();
        emit stopEventLoopTimer();
        return;
    }

    if (request_queue.isEmpty())
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

        emit startReconnectTimer();
        emit stopEventLoopTimer();
    }
}
