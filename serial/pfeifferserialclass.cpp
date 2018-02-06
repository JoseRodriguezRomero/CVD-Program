#include "pfeifferserialclass.h"

#define PFEIFFER_FLOW_CONTROL       QSerialPort::NoFlowControl
#define PFEIFFER_STOP_BITS          QSerialPort::OneStop
#define PFEIFFER_DATA_BITS          QSerialPort::Data8
#define PFEIFFER_PARITY             QSerialPort::NoParity

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

struct PfeifferRequestStruct
{
    char mneumonic;
    bool pending;
    QVector<uchar> args;
};

struct PfeifferSerialStruct {
    QVector<PfeifferRequestStruct> process_queue;
};

PfeifferSerialclass::PfeifferSerialclass(QObject *parent) :
    QObject(parent)
{
    serial_port = nullptr;

    reconnect_timer.setInterval(1000);
    reconnect_timer.setSingleShot(false);

    event_timer.setInterval(1000);
    event_timer.setSingleShot(false);

    private_struct = new PfeifferSerialStruct;
}

PfeifferSerialclass::~PfeifferSerialclass()
{
    if (serial_port != nullptr)
    {
        while (serial_port->isOpen()) {
            serial_port->close();
        }

        serial_port->deleteLater();
    }

    delete private_struct;
}

QString PfeifferSerialclass::PortName() const
{
    return port_name;
}

QSerialPort::FlowControl PfeifferSerialclass::FlowControl() const
{
    return PFEIFFER_FLOW_CONTROL;
}

QSerialPort::BaudRate PfeifferSerialclass::BaudRate() const
{
    return baud_rate;
}

QSerialPort::StopBits PfeifferSerialclass::StopBits() const
{
    return PFEIFFER_STOP_BITS;
}

QSerialPort::DataBits PfeifferSerialclass::DataBits() const
{
    return PFEIFFER_DATA_BITS;
}

QSerialPort::Parity PfeifferSerialclass::Parity() const
{
    return PFEIFFER_PARITY;
}

void PfeifferSerialclass::processSerialRequestQueue()
{
    if (!private_struct->process_queue.length())
    {
        event_timer.stop();
        return;
    }

    if (serial_port == nullptr)
    {
        return;
    }

    if (!serial_port->isOpen())
    {
        checkState();
        disconnectDevice();
        event_timer.start();
        return;
    }

    if (private_struct->process_queue.first().pending)
    {
        return;
    }

    QByteArray msg(private_struct->process_queue.first().mneumonic);
    QVector<uchar> args = private_struct->process_queue.first().args;

    for (int i = 0; i < args.length(); i++)
    {
        msg.append(args.at(i));
    }

    if (serial_port->waitForReadyRead())
    {
        serial_port->write(msg);

        if (serial_port->waitForBytesWritten())
        {
            private_struct->process_queue[0].pending = true;
        }
    }
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
        event_timer.stop();
        reconnect_timer.start();
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
    emit deviceConnected(serial_port->error());

    if (status)
    {
        event_timer.start();
        reconnect_timer.stop();
    }
    else
    {
        event_timer.stop();
        reconnect_timer.start();
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
    serial_port->setFlowControl(PFEIFFER_FLOW_CONTROL);
    serial_port->setStopBits(PFEIFFER_STOP_BITS);
    serial_port->setDataBits(PFEIFFER_DATA_BITS);
    serial_port->setParity(PFEIFFER_PARITY);

    serial_port->setParity(QSerialPort::NoParity);

    if (serial_port->open(QIODevice::ReadWrite))
    {
        emit ErrorString("Pfeiffer: CONNECTED", true);
        emit deviceConnected(serial_port->error());
    }
    {
        serial_port->deleteLater();
        serial_port = nullptr;
        emit ErrorString("Pfeiffer: CONNECTION ERROR", true);
        emit deviceConnected(QSerialPort::NotOpenError);
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
