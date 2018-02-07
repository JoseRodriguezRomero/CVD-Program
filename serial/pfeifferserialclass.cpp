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
    bool enquiry;
    bool enquirying;
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
    reconnect_timer.setSingleShot(true);

    event_timer.setInterval(50);
    event_timer.setSingleShot(true);

    private_struct = new PfeifferSerialStruct;

    connect(&reconnect_timer,SIGNAL(timeout()),this,SLOT(connectDevice()));
    connect(&event_timer,SIGNAL(timeout()),
            this,SLOT(processSerialRequestQueue()));
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
    if (event_timer.isActive())
    {
        return;
    }

    if (!private_struct->process_queue.length())
    {
        return;
    }

    if (serial_port == nullptr)
    {
        if (!reconnect_timer.isActive())
        {
            reconnect_timer.start();
        }
        return;
    }

    if (!serial_port->isOpen())
    {
        checkState();
        disconnectDevice();

        if (!reconnect_timer.isActive())
        {
            reconnect_timer.start();
        }

        return;
    }

    if (private_struct->process_queue.first().pending)
    {
        return;
    }

    char mneumonic = private_struct->process_queue.first().mneumonic;
    QByteArray msg;
    msg.clear();
    msg.append(mneumonic);
    QVector<uchar> args = private_struct->process_queue.first().args;

    for (int i = 0; i < args.length(); i++)
    {
        msg.append(args.at(i));
    }

    msg.append('\r');
    msg.append('\n');

    if (serial_port->isWritable())
    {
        serial_port->write(msg);

        if (serial_port->waitForBytesWritten())
        {
            private_struct->process_queue[0].pending = true;
        }
    }
    else
    {
        event_timer.start();
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
        disconnectDevice();
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
        reconnect_timer.stop();
    }
    else
    {
        disconnectDevice();
        reconnect_timer.start();
    }

    return status;
}

void PfeifferSerialclass::connectDevice()
{
    if (reconnect_timer.isActive())
    {
        return;
    }

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

void PfeifferSerialclass::ManageReply()
{
    QByteArray read_buffer = serial_port->readAll();
    serial_port->flush();

    if ((read_buffer.at(read_buffer.length()-2) != '\r') ||
            (read_buffer.at(read_buffer.length()-1) != '\n'))
    {
        private_struct->process_queue[0].pending = false;
        event_timer.start();
    }

    if (private_struct->process_queue.first().enquiry)
    {
        if (private_struct->process_queue.first().enquirying)
        {
            switch (private_struct->process_queue.first().mneumonic) {
            case BAU:
                break;
            case CAX:
                break;
            case CID:
                break;
            case DCB:
                break;
            case DCC:
                break;
            case DCD:
                break;
            case DCS:
                break;
            case DGS:
                break;
            case ERR:
                break;
            case FIL:
                break;
            case FSR:
                break;
            case LOC:
                break;
            case NAD:
                break;
            case OFC:
                break;
            case PNR:
                break;
            case PRX:
                break;
            case PUC:
                break;
            case RSX:
                break;
            case SAV:
                break;
            case SCX:
                break;
            case SEN:
                break;
            case SPX:
                break;
            case SPS:
                break;
            case TAI:
                break;
            case TAS:
                break;
            case TDI:
                break;
            case TEE:
                break;
            case TEP:
                break;
            case TID:
                break;
            case TKB:
                break;
            case TRA:
                break;
            case UNI:
                break;
            case WDT:
                break;
            }
        }
        else
        {
            if (serial_port->isWritable())
            {
                serial_port->write('\x05');

                if (serial_port->waitForBytesWritten())
                {
                    private_struct->process_queue[0].pending = true;
                }
            }
            else
            {
                private_struct->process_queue[0].pending = false;
                event_timer.start();
            }
        }
    }
    else
    {
        private_struct->process_queue.remove(0);
        private_struct->process_queue[0].pending = false;
        event_timer.start();
    }
}
