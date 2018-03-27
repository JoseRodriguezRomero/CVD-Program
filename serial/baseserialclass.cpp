#define MAX_FAILED_ATTEMPTS         12

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

bool BaseSerialClass::deviceConnected() const
{
    if (serial_port == nullptr)
    {
        return false;
    }

    QSerialPort::SerialPortError error = serial_port->error();

    if ((error == QSerialPort::NotOpenError) ||
            (error == QSerialPort::OpenError))
    {
        return false;
    }

    return true;
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

void BaseSerialClass::connectDevice()
{
    if (serial_port == nullptr)
    {
        serial_port = new QSerialPort(this);
    }
    else if (serial_port->isOpen())
    {
        return;
    }

    serial_port->setPortName(port_name);
    serial_port->setBaudRate(baud_rate);
    serial_port->setFlowControl(flow_control);
    serial_port->setStopBits(stop_bits);
    serial_port->setDataBits(data_bits);
    serial_port->setParity(port_parity);
    serial_port->open(QIODevice::ReadWrite);

    if (serial_port->error() == QSerialPort::NoError)
    {
        emit deviceConnected(serial_port->error(),no_reply);
        serial_port->flush();

        emit startEventLoopTimer();
        emit stopReconnectTimer();
    }
    else
    {
        serial_port->close();
        serial_port->deleteLater();
        serial_port = nullptr;
        emit deviceConnected(QSerialPort::NotOpenError,no_reply);

        emit stopEventLoopTimer();
        emit startReconnectTimer();
    }
}

void BaseSerialClass::disconnectDevice()
{
    if (serial_port == nullptr)
    {
        return;
    }

    emit stopReconnectTimer();
    emit stopEventLoopTimer();

    serial_port->close();
    serial_port->deleteLater();
    serial_port = nullptr;
}

bool BaseSerialClass::checkState()
{
    QString reply_string;
    bool status = false;

    if (serial_port == nullptr)
    {
        emit deviceConnected(QSerialPort::DeviceNotFoundError,no_reply);
        return false;
    }

    switch (serial_port->error()) {
    case QSerialPort::NoError:
        reply_string = "CONNECTED";
        status = true;
        break;
    case QSerialPort::DeviceNotFoundError:
        reply_string = "DEVICE NOT FOUND";
        break;
    case QSerialPort::PermissionError:
        reply_string = "PERMISSION ERROR";
        break;
    case QSerialPort::OpenError:
        reply_string = "OPEN ERROR";
        break;
    case QSerialPort::NotOpenError:
        reply_string = "NOT OPEN";
        break;
    case QSerialPort::WriteError:
        reply_string = "WRITE ERROR";
        break;
    case QSerialPort::ReadError:
        reply_string = "READ ERROR";
        break;
    case QSerialPort::ResourceError:
        reply_string = "RESOURCE ERROR";
        break;
    case QSerialPort::UnsupportedOperationError:
        reply_string = "UNSUPPORTED OPERATION";
        break;
    case QSerialPort::TimeoutError:
        reply_string = "TIMEOUT ERROR";
        break;
    default:
        reply_string = "UNKNOWN ERROR";
        break;
    }

    emit deviceConnected(serial_port->error(),no_reply);
    return status;
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
        no_reply = true;

        failed_attempts = 0;
        disconnectDevice();

        clearRequestQueue();

        emit startReconnectTimer();
        emit stopEventLoopTimer();
    }
}
