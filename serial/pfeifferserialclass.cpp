#include "pfeifferserialclass.h"

#define PFEIFFER_FLOW_CONTROL       QSerialPort::NoFlowControl
#define PFEIFFER_STOP_BITS          QSerialPort::OneStop
#define PFEIFFER_DATA_BITS          QSerialPort::Data8
#define PFEIFFER_PARITY             QSerialPort::NoParity

/* Mneumonics */

#define BAU             {'B','A','U'}
#define CA_X_           {'C','A'}
#define CID             {'C','I','D'}
#define DCB             {'D','C','B'}
#define DCC             {'D','C','C'}
#define DCD             {'D','C','D'}
#define DCS             {'D','C','S'}
#define DGS             {'D','G','S'}
#define ERR             {'E','R','R'}
#define FIL             {'F','I','L'}
#define FSR             {'F','S','R'}
#define LOC             {'L','O','C'}
#define NAD             {'N','A','D'}
#define OFC             {'O','F','C'}
#define PNR             {'P','N','R'}
#define PR_X_           {'P','R'}
#define PUC             {'P','U','C'}
#define RSX             {'R','S','X'}
#define SAV             {'S','A','V'}
#define SC_X_           {'S','C'}
#define SEN             {'S','E','N'}
#define SP_X_           {'S','P'}
#define SPS             {'S','P','S'}
#define TAI             {'T','A','I'}
#define TAS             {'T','A','S'}
#define TDI             {'T','D','I'}
#define TEE             {'T','E','E'}
#define TEP             {'T','E','P'}
#define TID             {'T','I','D'}
#define TKB             {'T','K','B'}
#define TRA             {'T','R','A'}
#define UNI             {'U','N','I'}
#define WDT             {'W','D','T'}

/* Mneumonics */

/* Mneumonic identifiers */

#define BAU_ID          1
#define CAX_ID          2
#define CID_ID          3
#define DCB_ID          4
#define DCC_ID          5
#define DCD_ID          6
#define DCS_ID          7
#define DGS_ID          8
#define ERR_ID          9
#define FIL_ID          10
#define FSR_ID          11
#define LOC_ID          12
#define NAD_ID          13
#define OFC_ID          14
#define PNR_ID          15
#define PRX_ID          16
#define PUC_ID          17
#define RSX_ID          18
#define SAV_ID          19
#define SCX_ID          20
#define SEN_ID          21
#define SPX_ID          22
#define SPS_ID          23
#define TAI_ID          24
#define TAS_ID          25
#define TDI_ID          26
#define TEE_ID          27
#define TEP_ID          28
#define TID_ID          29
#define TKB_ID          30
#define TRA_ID          31
#define UNI_ID          32
#define WDT_ID          33

/* Mneumonic identifiers */

struct PfeifferRequestStruct
{
    int mneumonic_id;
    QVector<char> mneumonic;
    bool pending;
    bool enquiry;
    bool enquirying;
    QVector<char> args;
};

struct PfeifferSerialStruct {
    QVector<PfeifferRequestStruct> process_queue;
};

void addReadRequestToQueue(PfeifferSerialStruct *parent,
                           int mneumonic_id, QVector<char> mneumonic)
{
    PfeifferRequestStruct new_request;
    new_request.mneumonic_id = mneumonic_id;
    new_request.mneumonic = mneumonic;
    new_request.pending = false;
    new_request.enquiry = true;
    new_request.enquirying = false;
    new_request.args.clear();

    parent->process_queue.append(new_request);
}

void addWriteRequestToQueue(PfeifferSerialStruct *parent,
                            int mneumonic_id, QVector<char> mneumonic,
                            const QVector<char> &args)
{
    PfeifferRequestStruct new_request;
    new_request.mneumonic_id = mneumonic_id;
    new_request.mneumonic = mneumonic;
    new_request.pending = false;
    new_request.enquiry = true;
    new_request.enquirying = false;
    new_request.args.clear();
    new_request.args.append(args);

    parent->process_queue.append(new_request);
}

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

void PfeifferSerialclass::requestReadSensorStatuses()
{
    addReadRequestToQueue(private_struct,SEN_ID,SEN);
}

void PfeifferSerialclass::requestReadSensorControl(
        PfeifferSerialclass::Sensor sensor)
{
    QVector<char> mneumonic(SC_X_);
    char mneumonic_idd = 'A';

    switch (sensor) {
    case Sensor::Sensor1 :
        break;
    case Sensor::Sensor2 :
        mneumonic_idd += 1;
        break;
    case Sensor::Sensor3 :
        mneumonic_idd += 2;
        break;
    case Sensor::Sensor4 :
        mneumonic_idd += 3;
        break;
    case Sensor::Sensor5 :
        mneumonic_idd += 4;
        break;
    case Sensor::Sensor6 :
        mneumonic_idd += 5;
        break;
    default :
        return;
    }

    mneumonic.append(mneumonic_idd);
    addReadRequestToQueue(private_struct,SCX_ID,mneumonic);
}

void PfeifferSerialclass::requestWriteSensorStatus(
        int sensor_num, PfeifferSerialclass::SensorStatus status)
{
    QVector<char> statuses;
    for (int i = 0; i < 6; i++)
    {
        statuses[i] = '0';
    }

    switch (status) {
    case PfeifferSerialclass::On:
        statuses[sensor_num-1] = '2';
        break;
    case PfeifferSerialclass::Off:
        statuses[sensor_num-1] = '1';
        break;
    default:
        return;
    }

    addWriteRequestToQueue(private_struct,SEN_ID,SEN,statuses);
}

void PfeifferSerialclass::requestWriteSensorControl(
        PfeifferSerialclass::Sensor sensor,
        PfeifferSerialclass::ControllingSource switch_on,
        PfeifferSerialclass::ControllingSource switch_off,
        float switch_on_value, float switch_off_value)
{

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
            switch (private_struct->process_queue.first().mneumonic_id) {
            case BAU_ID:
                break;
            case CAX_ID:
                break;
            case CID_ID:
                break;
            case DCB_ID:
                break;
            case DCC_ID:
                break;
            case DCD_ID:
                break;
            case DCS_ID:
                break;
            case DGS_ID:
                break;
            case ERR_ID:
                break;
            case FIL_ID:
                break;
            case FSR_ID:
                break;
            case LOC_ID:
                break;
            case NAD_ID:
                break;
            case OFC_ID:
                break;
            case PNR_ID:
                break;
            case PRX_ID:
                break;
            case PUC_ID:
                break;
            case RSX_ID:
                break;
            case SAV_ID:
                break;
            case SCX_ID:
                if (read_buffer.length() == 18)
                {
                    Sensor sensor;
                    char mnenumonic_idd = private_struct->process_queue.first().
                            mneumonic.last();

                    switch (mnenumonic_idd) {
                    case 'A':
                        sensor = Sensor::Sensor1;
                        break;
                    case 'B':
                        sensor = Sensor::Sensor2;
                        break;
                    case 'C':
                        sensor = Sensor::Sensor3;
                        break;
                    case 'D':
                        sensor = Sensor::Sensor4;
                        break;
                    case 'E':
                        sensor = Sensor::Sensor5;
                        break;
                    case 'F':
                        sensor = Sensor::Sensor6;
                        break;
                    }

                    char switch_on_source  = read_buffer.at(0);
                    char switch_off_source = read_buffer.at(1);
                }
                break;
            case SEN_ID:
                if (read_buffer.length() == 8)
                {
                    for (int i = 0; i < 6; i++)
                    {
                        if (read_buffer.at(i) == '2')
                        {
                            emit sensorStatus(i, On);
                        }
                        else if (read_buffer.at(i) == '1')
                        {
                            emit sensorStatus(i, Off);
                        }
                    }
                }
                break;
            case SPX_ID:
                break;
            case SPS_ID:
                break;
            case TAI_ID:
                break;
            case TAS_ID:
                break;
            case TDI_ID:
                break;
            case TEE_ID:
                break;
            case TEP_ID:
                break;
            case TID_ID:
                break;
            case TKB_ID:
                break;
            case TRA_ID:
                break;
            case UNI_ID:
                break;
            case WDT_iD:
                break;
            }

            private_struct->process_queue.remove(0);
            private_struct->process_queue[0].pending = false;
            event_timer.start();
        }
        else
        {
            if (serial_port->isWritable())
            {
                serial_port->flush();
                serial_port->write('\x05');

                if (serial_port->waitForBytesWritten())
                {
                    private_struct->process_queue[0].enquirying = true;
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
