#include "pfeifferserialclass.h"

/* Mneumonic addresses */

#define BAU             95
#define CAX             92
#define CID             88
#define DCB             89
#define DCC             90
#define DCD             88
#define DCS             90
#define DGS             93
#define ERR             97
#define FIL             92
#define FSR             93
#define LOC             91
#define NAD             96
#define OFC             93
#define PNR             98
#define PRX             88
#define PUC             91
#define RSX             94
#define SAV             94
#define SCX             87
#define SEN             86
#define SPX             90
#define SPS             91
#define TAI             100
#define TAS             100
#define TDI             98
#define TEE             100
#define TEP             99
#define TID             101
#define TKB             99
#define TRA             99
#define UNI             89
#define WDT             101

/* Mneumonic addresses */

PfeifferSerialclass::PfeifferSerialclass()
{
    serial_port = nullptr;

    event_timer.setInterval(1000);
    event_timer.setSingleShot(false);
}

PfeifferSerialclass::~PfeifferSerialclass()
{
}

void PfeifferSerialclass::setPortName(const QString &port_name)
{
    this->port_name = port_name;
}

void PfeifferSerialclass::setBaudRate(const QSerialPort::BaudRate baud_rate)
{
    this->baud_rate = baud_rate;
}

bool PfeifferSerialclass::checkState()
{
    QString reply_string;
    bool status = false;

    if (serial_port == nullptr)
    {
        emit ErrorString("Pfeiffer: CONNECTION ERROR", false);
        event_timer.start();
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
    case QSerialPort::ParityError:
        reply_string = "PARITY ERROR";
        break;
    case QSerialPort::FramingError:
        reply_string = "FRAMING ERROR";
        break;
    case QSerialPort::BreakConditionError:
        reply_string = "BREAK CONDITION ERROR";
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

    emit ErrorString("Pfeiffer: " + reply_string, status);

    if (status)
    {
        event_timer.stop();
    }
    else
    {
        event_timer.start();
        disconnectDevice();
    }

    return status;
}

void PfeifferSerialclass::connectDevice()
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
    serial_port->setStopBits(QSerialPort::OneStop);
    serial_port->setDataBits(QSerialPort::Data8);
    serial_port->setFlowControl(QSerialPort::NoFlowControl);
    serial_port->setParity(QSerialPort::NoParity);

    if (serial_port->open(QIODevice::ReadWrite))
    {
        emit ErrorString("Pfeiffer: CONNECTED", true);
    }
    {
        serial_port->deleteLater();
        serial_port = nullptr;
        emit deviceConnected("Pfeiffer: CONNECTION ERROR", true);
    }
}

void PfeifferSerialclass::disconnectDevice()
{
    if (serial_port == nullptr)
    {
        return;
    }

    serial_port->close();
    serial_port->deleteLater();
    serial_port = nullptr;
}
