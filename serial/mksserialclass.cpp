#include "mksserialclass.h"

#include <QDebug>

#define MAX_QUEUE_LEN   20

#define MKS_DEFAULT_PORT_NAME           "COM3"
#define MKS_DEFAULT_PARITY              QSerialPort::NoParity
#define MKS_DEFAULT_BAUD_RATE           QSerialPort::Baud9600
#define MKS_DEFAULT_STOP_BITS           QSerialPort::OneStop
#define MKS_DEFAULT_DATA_BITS           QSerialPort::Data8
#define MKS_DEFAULT_FLOW_CONTROL        QSerialPort::NoFlowControl

/* Mneumonics */

#define DT              "DT"
#define KY              "KY"
#define DG              "DG"
#define ID              "ID"
#define RT              "RT"
#define AC              "AC"
#define AV              "AV"
#define SP              "SP"
#define EX              "EX"
#define ST              "ST"
#define VL              "VL"
#define RL              "RL"
#define DP              "DP"
#define DP4             "DP4"
#define RG              "RG"
#define GN              "GN"
#define OF              "OF"
#define RTD             "RTD"
#define AZ              "AZ"
#define IN              "IN"
#define OT              "OT"
#define EI              "EI"
#define EO              "EO"
#define SM              "SM"
#define LN              "LN"
#define LS              "LS"
#define LM              "LM"
#define DB              "DB"
#define UP              "UL"
#define LL              "LL"
#define FR              "FR"
#define FT              "FT"
#define PY              "PY"
#define BD              "BD"
#define AD              "AD"
#define IM              "IM"
#define RS              "RS"
#define RE              "RE"
#define DF              "DF"

/* Mneumonics */

/* Mneumonic identifiers */

#define DT_ID               1
#define KY_ID               2
#define DG_ID               3
#define ID_ID               4
#define RT_ID               5
#define AC_ID               6
#define AV_ID               7
#define SP_ID               8
#define EX_ID               9
#define ST_ID               10
#define VL_ID               11
#define RL_ID               12
#define DP_ID               13
#define DP4_ID              14
#define RG_ID               15
#define GN_ID               16
#define OF_ID               17
#define RTD_ID              18
#define AZ_ID               19
#define IN_ID               20
#define OT_ID               21
#define EI_ID               22
#define EO_ID               23
#define SM_ID               24
#define LN_ID               25
#define LS_ID               26
#define LM_ID               27
#define DB_ID               28
#define UP_ID               29
#define LL_ID               30
#define FR_ID               31
#define FT_ID               32
#define PY_ID               33
#define BD_ID               34
#define AD_ID               35
#define IM_ID               36
#define RS_ID               37
#define RE_ID               38
#define DF_ID               39
#define UNLOCK_ID           40
#define LOCK_ID             41

/* Mneumonic identifiers */

struct MKSRequestStruct
{
    int device_address;
    int channel;
    int mneumonic_id;
    QString mneumonic;
    QVector<QString> args;
    bool pending;
    bool enquiry;
    bool enquirying;
};

MKSSerialClass::MKSSerialClass(QObject *parent)
{
    this->setParent(parent);

    reconnect_timer.setInterval(1000);
    reconnect_timer.setSingleShot(false);

    event_timer.setInterval(10);
    event_timer.setSingleShot(false);

    reconnect_timer.setParent(this);
    event_timer.setParent(this);

    port_name = MKS_DEFAULT_PORT_NAME;
    baud_rate = MKS_DEFAULT_BAUD_RATE;
    stop_bits = MKS_DEFAULT_STOP_BITS;
    data_bits = MKS_DEFAULT_DATA_BITS;
    port_parity = MKS_DEFAULT_PARITY;
    flow_control = MKS_DEFAULT_FLOW_CONTROL;

    failed_attempts = 0;
    serial_port = nullptr;
}

MKSSerialClass::~MKSSerialClass()
{
    if (serial_port != nullptr)
    {
        while (serial_port->isOpen()) {
            serial_port->close();
        }

        serial_port->deleteLater();
    }
}

bool MKSSerialClass::deviceConnected() const
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

bool MKSSerialClass::deviceDisconnected() const
{
    return !deviceConnected();
}

bool MKSSerialClass::processPending() const
{
    MKSRequestStruct *request = static_cast<MKSRequestStruct*>(
                request_queue.first());

    return request->pending;
}

void MKSSerialClass::manageReply()
{
    if (serial_port->bytesAvailable() < 1)
    {
        return;
    }

    MKSRequestStruct *request = static_cast<MKSRequestStruct*>(
                request_queue.first());

    buffer.append(serial_port->readAll());

    if (buffer.at(buffer.length()-1) != '\r')
    {
        return;
    }

    if (buffer.at(0) == '#')
    {
        manageErrorReply();
        buffer.clear();
        request->enquirying = false;
        request->pending = false;
        return;
    }
    else
    {
    }
}

void MKSSerialClass::processRequestQueue()
{
    if (!serial_port->isOpen())
    {
        return;
    }

    MKSRequestStruct *request = static_cast<MKSRequestStruct*>(
                request_queue.first());

    QByteArray msg;
    msg.clear();

    msg.append('@');
    msg.append(QString::number(request->device_address));
    msg.append('?');

    if (request->args.length() > 0)
    {
        msg.append('!');
    }

    for (int i = 0; i < request->mneumonic.length(); i++)
    {
        msg.append(request->mneumonic.at(i).toLatin1());
    }

    if (request->channel > 0)
    {
        msg.append(QString::number(request->channel));
    }

    for (int i = 0; i < request->args.length(); i++)
    {
        msg.append(',');
        QString arg = request->args.at(i);

        for (int j = 0; j < arg.length(); j++)
        {
            msg.append(arg.at(j));
        }
    }

    msg.append('\r');
    buffer.clear();
    serial_port->flush();

    if (serial_port->isWritable())
    {
        serial_port->write(msg);

        if (serial_port->waitForBytesWritten(500))
        {
            request->pending = true;
        }
        else
        {
            serial_port->flush();
        }
    }
}

void MKSSerialClass::connectDevice()
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
        emit errorString("Pfeiffer: CONNECTED", true);
        emit deviceConnected(serial_port->error());
        serial_port->flush();

        emit startEventLoopTimer();
        emit stopReconnectTimer();
    }
    else
    {
        serial_port->close();
        serial_port->deleteLater();
        serial_port = nullptr;
        emit errorString("Pfeiffer: CONNECTION ERROR", false);
        emit deviceConnected(QSerialPort::NotOpenError);

        emit stopEventLoopTimer();
        emit startReconnectTimer();
    }
}

void MKSSerialClass::disconnectDevice()
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

bool MKSSerialClass::checkState()
{
    QString reply_string;
    bool status = false;

    if (serial_port == nullptr)
    {
        emit errorString("Pfeiffer: CONNECTION ERROR", false);
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

    emit errorString("Pfeiffer: " + reply_string, status);
    emit deviceConnected(serial_port->error());

    return status;
}

void MKSSerialClass::manageErrorReply()
{
    QString error_string;
    ErrorMessage error_message;

    for (int i = 0; i < buffer.length() - 1; i++)
    {
        error_string.append(buffer.at(i));
    }

    if (error_string == "#E001")
    {
        error_message = CommunicationError;
    }
    else if (error_string == "#E002")
    {
        error_message = ADCOverflowOrUnderflow;
    }
    else if (error_string == "#E003")
    {
        error_message = RangeError;
    }
    else if (error_string == "#W001")
    {
        error_message = OffsetError;
    }
    else
    {
        return;
    }

    emit errorMessage(error_message);
}
