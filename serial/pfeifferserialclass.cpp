#include "pfeifferserialclass.h"

#define MAX_QUEUE_LEN   20

#define PFEIFFER_DEFAULT_PORT_NAME          "COM6"
#define PFEIFFER_DEFAULT_PARITY             QSerialPort::NoParity
#define PFEIFFER_DEFAULT_BAUD_RATE          QSerialPort::Baud9600
#define PFEIFFER_DEFAULT_STOP_BITS          QSerialPort::OneStop
#define PFEIFFER_DEFAULT_DATA_BITS          QSerialPort::Data8
#define PFEIFFER_DEFAULT_FLOW_CONTROL       QSerialPort::NoFlowControl

/* Mneumonics */

#define BAU             "BAU"
#define CA_X_           "CA"
#define CID             "CID"
#define DCB             "DCB"
#define DCC             "DCC"
#define DCD             "DCD"
#define DCS             "DCS"
#define DGS             "DGS"
#define ERR             "ERR"
#define FIL             "FIL"
#define FSR             "FSR"
#define LOC             "LOC"
#define NAD             "NAD"
#define OFC             "OFC"
#define PNR             "PNR"
#define PR_X_           "PR"
#define PUC             "PUC"
#define RSX             "RSX"
#define SAV             "SAV"
#define SC_X_           "SC"
#define SEN             "SEN"
#define SP_X_           "SP"
#define SPS             "SPS"
#define TAI             "TAI"
#define TAS             "TAS"
#define TDI             "TDI"
#define TEE             "TEE"
#define TEP             "TEP"
#define TID             "TID"
#define TKB             "TKB"
#define TRA             "TRA"
#define UNI             "UNI"
#define WDT             "WDT"

/* Mneumonics */

/* Mneumonic identifiers */

#define BAU_ID          1
#define CA_X_ID         2
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
#define PR_X_ID         16
#define PUC_ID          17
#define RSX_ID          18
#define SAV_ID          19
#define SC_X_ID         20
#define SEN_ID          21
#define SP_X_ID         22
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

/* Argument formats */

#define SEN_ARGS_FORMAT         "x,x,x,x,x,x"
#define SC_X_ARGS_FORMAT        "x,x,x.xxE±yy,x.xxE±yy"
#define PR_X_ARGS_FORMAT        "x,x.xxxEsx"
#define DCD_ARGS_FORMAT         "x"
#define CID_ARGS_FORMAT         "xxxx,xxxx,xxxx,xxxx,xxxx,xxxx"
#define UNI_ARGS_FORMAT         "x"
#define DCB_ARGS_FORMAT         "x"
#define DCC_ARGS_FORMAT         "xx"
#define DCS_ARGS_FORMAT         "xx"
#define SP_X_ARGS_FORMAT        "x,x.xxEsx,x.xxEsx"
#define SPS_ARGS_FORMAT         "x,x,x,x,x,x"
#define PUC_ARGS_FORMAT         "x,x,x,x,x,x"
#define LOC_ARGS_FORMAT         "x"
#define FIL_ARGS_FORMAT         "x,x,x,x,x,x"
#define CA_X_ARGS_FORMAT        "x.xxx"
#define OFC_ARGS_FORMAT         "x,x,x,x,x,x"
#define FSR_ARGS_FORMAT         "x,x,x,x,x,x"
#define DGS_ARGS_FORMAT         "0,0,0,x,x,x"
#define RSX_ARGS_FORMAT         "x"
#define BAU_ARGS_FORMAT         "x"
#define NAD_ARGS_FORMAT         "xx"
#define ERR_ARGS_FORMAT         "xxxxx,xxxxx"
#define PNR_ARGS_FORMAT         "BGxxxxxx-x"
#define TDI_ARGS_FORMAT         "xxxxx,xxxxx"
#define TKB_ARGS_FORMAT         "xx"
#define TRA_ARGS_FORMAT         "xxxxx,xxxxx"
#define TEP_ARGS_FORMAT         "xxxxx,xxxxx"
#define TEE_ARGS_FORMAT         "xxxxx,xxxxx"
#define TAI_ARGS_FORMAT         "x.xxx,x.xxx,x.xxx,x.xxx,x.xxx,x.xxx"
#define TAS_ARGS_FORMAT         "x.xxx,x.xxx,x.xxx,x.xxx,x.xxx,x.xxx"
#define TID_ARGS_FORMAT         "xxxxxxxxx,xxxxxxxxx,xxxxxxxxx,xxxxxxxxx,xxxxxxxxx,xxxxxxxxx"
#define WDT_ARGS_FORMAT         "x"

/* Argument formats */

struct PfeifferRequestStruct
{
    int mneumonic_id;
    QString mneumonic;
    QVector<QString> args;
    bool pending;
    bool enquiry;
    bool enquirying;
};

void deletePfeifferRequest(void* request_ptr)
{
    PfeifferRequestStruct *aux_ptr;
    aux_ptr = static_cast<PfeifferRequestStruct*>(request_ptr);
    delete aux_ptr;
}

void addReadRequestToQueue(QVector<void*> &request_queue,
                           int mneumonic_id, QString mneumonic)
{
    while (request_queue.length() > MAX_QUEUE_LEN)
    {
        deletePfeifferRequest(request_queue.at(0));
        request_queue.removeAt(0);
    }

    if (mneumonic.length() != 3)
    {
        return;
    }

    PfeifferRequestStruct *new_request = new PfeifferRequestStruct;
    new_request->mneumonic_id = mneumonic_id;
    new_request->mneumonic = mneumonic;
    new_request->pending = false;
    new_request->enquiry = true;
    new_request->enquirying = false;
    new_request->args.clear();

    request_queue.append(new_request);
}

void addWriteRequestToQueue(QVector<void*> &request_queue,
                            int mneumonic_id, QString mneumonic,
                            const QVector<QString> &args)
{
    while (request_queue.length() > MAX_QUEUE_LEN)
    {
        deletePfeifferRequest(request_queue.at(0));
        request_queue.removeAt(0);
    }

    if (mneumonic.length() != 3)
    {
        return;
    }

    PfeifferRequestStruct *new_request = new PfeifferRequestStruct;
    new_request->mneumonic_id = mneumonic_id;
    new_request->mneumonic = mneumonic;
    new_request->pending = false;
    new_request->enquiry = true;
    new_request->enquirying = false;
    new_request->args = args;

    request_queue.append(new_request);
}

bool validArgsFormat(const QByteArray &reply, const char *format,
                     const bool has_string_termination = true)
{
    int len = static_cast<int>(strlen(format));
    if (has_string_termination)
    {
        len += 2;

        if ((reply.at(reply.length()-1) != '\n') &&
                (reply.at(reply.length()-2) != '\r'))
        {
            return false;
        }
    }

    if (reply.length() != len)
    {
        return false;
    }

    for (int i = 0; i < len; i++)
    {
        if ((format[i] == ',') && (reply.at(i) != ','))
        {
            return false;
        }
    }

    return true;
}

PfeifferSerialclass::PfeifferSerialclass(QObject *parent)
{
    this->setParent(parent);

    serial_port = nullptr;

    reconnect_timer.setInterval(500);
    reconnect_timer.setSingleShot(false);

    event_timer.setInterval(50);
    event_timer.setSingleShot(false);

    event_timer.setParent(this);
    reconnect_timer.setParent(this);

    port_name = PFEIFFER_DEFAULT_PORT_NAME;
    baud_rate = PFEIFFER_DEFAULT_BAUD_RATE;
    stop_bits = PFEIFFER_DEFAULT_STOP_BITS;
    data_bits = PFEIFFER_DEFAULT_DATA_BITS;
    port_parity = PFEIFFER_DEFAULT_PARITY;
    flow_control = PFEIFFER_DEFAULT_FLOW_CONTROL;

    no_reply = true;

    failed_attempts = 0;
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
}

void PfeifferSerialclass::processRequestQueue()
{
    if (!serial_port->isOpen())
    {
        return;
    }

    PfeifferRequestStruct *request =static_cast<PfeifferRequestStruct*>(
                request_queue.first());

    QByteArray msg;
    msg.clear();

    for (int i = 0; i < 3; i++)
    {
        msg.append(request->mneumonic.at(i).toLatin1());
    }

    int arg_count = request->args.length();
    for (int i = 0; i < arg_count; i++)
    {
        msg.append(',');
        QString arg = request->args.at(i);

        for (int j = 0; j < arg.length(); j++)
        {
            msg.append(arg.at(j).toLatin1());
        }
    }
    msg.append('\r');
    msg.append('\n');

    buffer.clear();
    serial_port->clear();
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

void PfeifferSerialclass::requestReadSensorStatuses()
{
    addReadRequestToQueue(request_queue,SEN_ID,SEN);
}

void PfeifferSerialclass::requestReadSensorControl(const Sensor sensor)
{
    QString mneumonic(SC_X_);
    char mneumonic_idd;

    switch (sensor) {
    case Sensor::Sensor1 :
        mneumonic_idd = 'A';
        break;
    case Sensor::Sensor2 :
        mneumonic_idd = 'B';
        break;
    case Sensor::Sensor3 :
        mneumonic_idd = 'C';
        break;
    case Sensor::Sensor4 :
        mneumonic_idd = 'D';
        break;
    case Sensor::Sensor5 :
        mneumonic_idd = 'E';
        break;
    case Sensor::Sensor6 :
        mneumonic_idd = 'F';
        break;
    default :
        return;
    }

    mneumonic.append(mneumonic_idd);
    addReadRequestToQueue(request_queue,SC_X_ID,mneumonic);
}

void PfeifferSerialclass::requestReadStatusAndPressure(
        const Sensor sensor)
{
    QString mneumonic(PR_X_);
    char mneumonic_idd;

    switch (sensor) {
    case Sensor::Sensor1 :
        mneumonic_idd = '1';
        break;
    case Sensor::Sensor2 :
        mneumonic_idd = '2';
        break;
    case Sensor::Sensor3 :
        mneumonic_idd = '3';
        break;
    case Sensor::Sensor4 :
        mneumonic_idd = '4';
        break;
    case Sensor::Sensor5 :
        mneumonic_idd = '5';
        break;
    case Sensor::Sensor6 :
        mneumonic_idd = '6';
        break;
    default :
        return;
    }

    mneumonic.append(mneumonic_idd);
    addReadRequestToQueue(request_queue,PR_X_ID,mneumonic);
}

void PfeifferSerialclass::requestReadDecimalDigits()
{
    addReadRequestToQueue(request_queue,DCD_ID,DCD);
}

void PfeifferSerialclass::requestReadMeasurementPointNames()
{
    addReadRequestToQueue(request_queue,CID_ID,CID);
}

void PfeifferSerialclass::requestReadUnitsOfMeasurement()
{
    addReadRequestToQueue(request_queue,UNI_ID,UNI);
}

void PfeifferSerialclass::requestReadBaragraph()
{
    addReadRequestToQueue(request_queue,DCB_ID,DCB);
}

void PfeifferSerialclass::requestReadDisplayContrast()
{
    addReadRequestToQueue(request_queue,DCC_ID,DCC);
}

void PfeifferSerialclass::requestReadScreenSave()
{
    addReadRequestToQueue(request_queue,DCS_ID,DCS);
}

void PfeifferSerialclass::requestReadThresholdValueSetting(const Relay relay)
{
    QString mneumonic(SP_X_);
    char mneumonic_idd;

    switch (relay) {
    case Sensor1:
        mneumonic_idd = 'A';
        break;
    case Sensor2:
        mneumonic_idd = 'B';
        break;
    case Sensor3:
        mneumonic_idd = 'C';
        break;
    case Sensor4:
        mneumonic_idd = 'D';
        break;
    case Sensor5:
        mneumonic_idd = 'E';
        break;
    case Sensor6:
        mneumonic_idd = 'F';
        break;
    default:
        return;
    }

    mneumonic.append(mneumonic_idd);
    addReadRequestToQueue(request_queue,SP_X_ID,mneumonic);
}

void PfeifferSerialclass::requestWriteSensorStatus(
        const PfeifferSerialclass::Sensor sensor,
        const PfeifferSerialclass::SensorStatus status)
{
    QVector<QString> args;
    for (int i = 0; i < 6; i++)
    {
        args.append(QString('0'));
    }

    int sensor_num;
    switch (sensor) {
    case Sensor1:
        sensor_num = 0;
        break;
    case Sensor2:
        sensor_num = 1;
        break;
    case Sensor3:
        sensor_num = 2;
        break;
    case Sensor4:
        sensor_num = 3;
        break;
    case Sensor5:
        sensor_num = 4;
        break;
    case Sensor6:
        sensor_num = 5;
        break;
    default:
        return;
    }

    switch (status) {
    case PfeifferSerialclass::On:
        args[sensor_num] = QString('2');
        break;
    case PfeifferSerialclass::Off:
        args[sensor_num] = QString('1');
        break;
    default:
        return;
    }

    addWriteRequestToQueue(request_queue,SEN_ID,SEN,args);
}

void PfeifferSerialclass::requestWriteSensorControl(
        const PfeifferSerialclass::Sensor sensor,
        const PfeifferSerialclass::ControllingSource switch_on,
        const PfeifferSerialclass::ControllingSource switch_off,
        const float switch_on_value, const float switch_off_value)
{
    QString mneumonic(SC_X_);
    QVector<QString> args;

    switch (sensor) {
    case Sensor::Sensor1:
        mneumonic.append('A');
        break;
    case Sensor::Sensor2:
        mneumonic.append('B');
        break;
    case Sensor::Sensor3:
        mneumonic.append('C');
        break;
    case Sensor::Sensor4:
        mneumonic.append('D');
        break;
    case Sensor::Sensor5:
        mneumonic.append('E');
        break;
    case Sensor::Sensor6:
        mneumonic.append('F');
        break;
    default:
        return;
    }

    switch (switch_on) {
    case Sensor1Control:
        args.append(QString('0'));
        break;
    case Sensor2Control:
        args.append(QString('1'));
        break;
    case Sensor3Control:
        args.append(QString('2'));
        break;
    case Sensor4Control:
        args.append(QString('3'));
        break;
    case Sensor5Control:
        args.append(QString('4'));
        break;
    case Sensor6Control:
        args.append(QString('5'));
        break;
    case ExternalControl:
        args.append(QString('6'));
        break;
    case ManualControl:
        args.append(QString('7'));
        break;
    case HotStart:
        args.append(QString('8'));
        break;
    default:
        return;
    }

    switch (switch_off) {
    case Sensor1Control:
        args.append(QString('0'));
        break;
    case Sensor2Control:
        args.append(QString('1'));
        break;
    case Sensor3Control:
        args.append(QString('2'));
        break;
    case Sensor4Control:
        args.append(QString('3'));
        break;
    case Sensor5Control:
        args.append(QString('4'));
        break;
    case Sensor6Control:
        args.append(QString('5'));
        break;
    case ExternalControl:
        args.append(QString('6'));
        break;
    case ManualControl:
        args.append(QString('7'));
        break;
    default:
        return;
    }

    QString switch_on_value_string("");
    QString switch_off_value_string("");

    switch_on_value_string.sprintf("%1.1E",switch_on_value);
    switch_off_value_string.sprintf("%1.1E",switch_off_value);

    args.append(switch_on_value_string);
    args.append(switch_off_value_string);

    addWriteRequestToQueue(request_queue,SC_X_ID,mneumonic,args);
}

void PfeifferSerialclass::requestWriteDecimalDigits(
        const PfeifferSerialclass::DecimalDigits digits)
{
    QVector<QString> arg;

    switch (digits) {
    case TwoDigits:
        arg.append(QString('2'));
        break;
    case ThreeDigits:
        arg.append(QString('3'));
        break;
    default:
        return;
    }

    addWriteRequestToQueue(request_queue,DCD_ID,DCD,arg);
}

void PfeifferSerialclass::requestWriteMeasurementPointNames(
        const QString names[6])
{
    QVector<QString> args;

    for (int i = 0; i < 6; i++)
    {
        args.append(names[i]);
    }

    addWriteRequestToQueue(request_queue,CID_ID,CID,args);
}

void PfeifferSerialclass::requestWriteUnitsOfMeasurement(
        const PfeifferSerialclass::Units units)
{
    QVector<QString> arg;

    switch (units) {
    case mBar:
        arg.append(QString('0'));
        break;
    case Torr:
        arg.append(QString('1'));
        break;
    case Pascal:
        arg.append(QString('2'));
        break;
    default:
        return;
    }

    addWriteRequestToQueue(request_queue,UNI_ID,UNI,arg);
}

void PfeifferSerialclass::requestWriteBaragraph(
        PfeifferSerialclass::BaragraphMode bar_mode)
{
    QVector<QString> arg;

    switch (bar_mode) {
    case BaragraphOff:
        arg.append(QString('0'));
        break;
    case MeasurementRange:
        arg.append(QString('1'));
        break;
    case OneDecade:
        arg.append(QString('2'));
        break;
    default:
        return;
    }

    addWriteRequestToQueue(request_queue,DCB_ID,DCB,arg);
}

void PfeifferSerialclass::requestWriteDisplayContrast(const int contrast)
{
    if (contrast < 0 || contrast > 20)
    {
        return;
    }

    QVector<QString> arg;
    arg.append(QString::number(contrast));

    addWriteRequestToQueue(request_queue,DCC_ID,DCC,arg);
}

void PfeifferSerialclass::requestWriteScreenSave(const int interval)
{
    if (interval < 0 || interval > 99)
    {
        return;
    }

    QVector<QString> arg;
    arg.append(QString::number(interval));

    addWriteRequestToQueue(request_queue,DCS_ID,DCS,arg);
}

void PfeifferSerialclass::requestWriteThresholdValueSetting(
        const PfeifferSerialclass::Relay relay,
        const PfeifferSerialclass::Sensor sensor,
        const float lower_threshold, const float upper_threshold)
{
    QString mneumonic(SP_X_);
    char mneumonic_idd;

    switch (relay) {
    case Relay1:
        mneumonic_idd = 'A';
        break;
    case Relay2:
        mneumonic_idd = 'B';
        break;
    case Relay3:
        mneumonic_idd = 'C';
        break;
    case Relay4:
        mneumonic_idd = 'D';
        break;
    case Relay5:
        mneumonic_idd = 'E';
        break;
    case Relay6:
        mneumonic_idd = 'F';
        break;
    default:
        return;
    }
    mneumonic.append(mneumonic_idd);

    QVector<QString> args;
    char sensor_idd;

    switch (sensor) {
    case Sensor1:
        sensor_idd = '0';
        break;
    case Sensor2:
        sensor_idd = '1';
        break;
    case Sensor3:
        sensor_idd = '2';
        break;
    case Sensor4:
        sensor_idd = '3';
        break;
    case Sensor5:
        sensor_idd = '4';
        break;
    case Sensor6:
        sensor_idd = '5';
        break;
    default:
        return;
    }
    args.append(QString(sensor_idd));

    QString lower_threshold_string;
    QString upper_threshold_string;

    lower_threshold_string.sprintf("%1.1E",lower_threshold);
    upper_threshold_string.sprintf("%1.1E",upper_threshold);

    args.append(lower_threshold_string);
    args.append(upper_threshold_string);

    addWriteRequestToQueue(request_queue,SP_X_ID,mneumonic,args);
}

bool PfeifferSerialclass::processPending() const
{
    PfeifferRequestStruct *request = static_cast<PfeifferRequestStruct*>(
                request_queue.first());

    return request->pending;
}

void PfeifferSerialclass::manageReply()
{
    if (serial_port->bytesAvailable() < 1)
    {
        failed_attempts++;
        return;
    }

    PfeifferRequestStruct *request = static_cast<PfeifferRequestStruct*>(
                request_queue.first());

    buffer.append(serial_port->readAll());

    if (buffer.length() < 2)
    {
        return;
    }

    if ((buffer.at(buffer.length()-2) != '\r') ||
            (buffer.at(buffer.length()-1) != '\n'))
    {
        failed_attempts++;
        if (failed_attempts >= MAX_QUEUE_LEN)
        {
            no_reply = true;
        }
        return;
    }

    if (request->enquiry)
    {
        if (request->enquirying)
        {
            bool valid_reply = false;

            switch (request->mneumonic_id) {
            case BAU_ID:
                break;
            case CA_X_ID:
                break;
            case CID_ID:
                valid_reply = manageMeasurementPointNamesReply();
                break;
            case DCB_ID:
                valid_reply = manageBaragraphModeReply();
                break;
            case DCC_ID:
                valid_reply = manageReplyDisplayContrast();
                break;
            case DCD_ID:
                valid_reply = manageDecimalDigitsReply();
                break;
            case DCS_ID:
                valid_reply = manageScreenSaveReply();
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
            case PR_X_ID:
                valid_reply = manageStatusAndPressureReply();
                break;
            case PUC_ID:
                break;
            case RSX_ID:
                break;
            case SAV_ID:
                break;
            case SC_X_ID:
                valid_reply = manageSensorControlReply();
                break;
            case SEN_ID:
                valid_reply = manageSensorStatusReply();
                break;
            case SP_X_ID:
                valid_reply = manageThresholdValueSettingReply();
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
                valid_reply = manageUnitsOfMeasurementReply();
                break;
            case WDT_ID:
                break;
            default:
                valid_reply = false;
                break;
            }

            buffer.clear();

            if (valid_reply)
            {
                delete request;

                if (no_reply)
                {
                    no_reply = false;
                    emit deviceConnected(serial_port->error(),no_reply);
                }

                failed_attempts = 0;
                request_queue.remove(0);
                if (request_queue.length() > 0)
                {
                    request = static_cast<PfeifferRequestStruct*>(
                                request_queue.first());
                    request->pending = false;
                }
            }
            else
            {
                failed_attempts++;
                request->pending = false;
                request->enquirying = false;
            }
        }
        else
        {
            if (serial_port->isWritable())
            {
                QByteArray enq;
                enq.append('\x05');

                if (buffer.at(0) != '\x06')
                {
                    buffer.clear();
                    return;
                }
                else
                {
                    buffer.clear();
                    serial_port->flush();
                    serial_port->write(enq);
                    failed_attempts++;

                    if (serial_port->waitForBytesWritten())
                    {
                        request->enquirying = true;
                    }
                }
            }
            else
            {
                request->pending = false;
            }
        }
    }
    else
    {
        delete request;
        request_queue.remove(0);
        request = static_cast<PfeifferRequestStruct*>(request_queue.first());
        request->pending = false;
    }
}

bool PfeifferSerialclass::manageSensorStatusReply()
{
    if (!validArgsFormat(buffer,SEN_ARGS_FORMAT))
    {
        return false;
    }

    Sensor sensors[6];
    SensorStatus statuses[6];

    for (int i = 0; i < 6; i++)
    {
        Sensor sensor;

        switch (i) {
        case 0:
            sensor = Sensor1;
            break;
        case 1:
            sensor = Sensor2;
            break;
        case 2:
            sensor = Sensor3;
            break;
        case 3:
            sensor = Sensor4;
            break;
        case 4:
            sensor = Sensor5;
            break;
        case 5:
            sensor = Sensor6;
            break;
        }

        SensorStatus status;

        switch (buffer.at(i)) {
        case '2':
            status = On;
            break;
        case '1':
            status = Off;
            break;
        default:
            return false;
        }

        sensors[i] = sensor;
        statuses[i] = status;
    }

    for (int i = 0; i < 6; i++)
    {
        emit sensorStatus(sensors[i], statuses[i]);
    }

    return true;
}

bool PfeifferSerialclass::manageSensorControlReply()
{
    if (!validArgsFormat(buffer,SC_X_ARGS_FORMAT))
    {
        return false;
    }

    PfeifferRequestStruct *request = static_cast<PfeifferRequestStruct*>(
                request_queue.first());

    Sensor sensor;
    char mnenumonic_idd = request->mneumonic.at(2).toLatin1();

    switch (mnenumonic_idd) {
    case 'A':
        sensor = Sensor1;
        break;
    case 'B':
        sensor = Sensor2;
        break;
    case 'C':
        sensor = Sensor3;
        break;
    case 'D':
        sensor = Sensor4;
        break;
    case 'E':
        sensor = Sensor5;
        break;
    case 'F':
        sensor = Sensor6;
        break;
    default:
        return false;
    }

    char switch_on_source_char  = buffer.at(0);
    char switch_off_source_char = buffer.at(1);

    ControllingSource switch_on_source;
    ControllingSource switch_off_source;

    switch (switch_on_source_char) {
    case '0':
        switch_on_source = Sensor1Control;
        break;
    case '1':
        switch_on_source = Sensor2Control;
        break;
    case '2':
        switch_on_source = Sensor3Control;
        break;
    case '3':
        switch_on_source = Sensor4Control;
        break;
    case '4':
        switch_on_source = Sensor5Control;
        break;
    case '5':
        switch_on_source = Sensor6Control;
        break;
    case '6':
        switch_on_source = ExternalControl;
        break;
    case '7':
        switch_on_source = ManualControl;
        break;
    case '8':
        switch_on_source = HotStart;
        break;
    default:
        return false;
    }

    switch (switch_off_source_char) {
    case '0':
        switch_off_source = Sensor1Control;
        break;
    case '1':
        switch_off_source = Sensor2Control;
        break;
    case '2':
        switch_off_source = Sensor3Control;
        break;
    case '3':
        switch_off_source = Sensor4Control;
        break;
    case '4':
        switch_off_source = Sensor5Control;
        break;
    case '5':
        switch_off_source = Sensor6Control;
        break;
    case '6':
        switch_off_source = ExternalControl;
        break;
    case '7':
        switch_off_source = ManualControl;
        break;
    default:
        return false;
    }

    QString switch_on_value_string;
    QString switch_off_value_string;

    for (int i = 0; i < 8; i++)
    {
        switch_on_value_string.append(buffer.at(i+2));
    }

    for (int i = 0; i < 8; i++)
    {
        switch_off_value_string.append(buffer.at(i+10));
    }

    bool switch_on_cast, switch_off_cast;
    float switch_on_value = switch_on_value_string.toFloat(&switch_on_cast);
    float switch_off_value = switch_on_value_string.toFloat(&switch_off_cast);

    if (switch_off_cast && switch_on_cast)
    {
        emit sensorControl(sensor,switch_on_source,switch_off_source,
                           switch_on_value,switch_off_value);
    }

    return switch_off_cast && switch_on_cast;
}

bool PfeifferSerialclass::manageStatusAndPressureReply()
{
    if (!validArgsFormat(buffer,PR_X_ARGS_FORMAT))
    {
        return false;
    }

    PfeifferRequestStruct *request = static_cast<PfeifferRequestStruct*>(
                request_queue.first());

    if (buffer.length() != 12)
    {
        return false;
    }

    Sensor sensor;
    char mnenumonic_idd = request->mneumonic.at(2).toLatin1();

    switch (mnenumonic_idd) {
    case '1':
        sensor = Sensor1;
        break;
    case '2':
        sensor = Sensor2;
        break;
    case '3':
        sensor = Sensor3;
        break;
    case '4':
        sensor = Sensor4;
        break;
    case '5':
        sensor = Sensor5;
        break;
    case '6':
        sensor = Sensor6;
        break;
    default:
        return false;
    }

    PressureMeasurementStatus status;
    char status_char = buffer.at(0);

    switch (status_char) {
    case '0':
        status = MeasurementDataOkay;
        break;
    case '1':
        status = Underrange;
        break;
    case '2':
        status = Overrange;
        break;
    case '3':
        status = SensorError;
        break;
    case '4':
        status = SensorOff;
        break;
    case '5':
        status = NoSensor;
        break;
    case '6':
        status = IdentificationError;
        break;
    default:
        return false;
    }

    QString pressure_value_string;
    float pressure_value;
    bool pressure_cast;

    for (int i = 0; i < 8; i++)
    {
        pressure_value_string.append(buffer.at(i+2));
    }

    pressure_value = pressure_value_string.toFloat(&pressure_cast);

    if (pressure_cast)
    {
        emit sensorPressureAndStautus(sensor,status,pressure_value);
    }

    return pressure_cast;
}

bool PfeifferSerialclass::manageDecimalDigitsReply()
{
    if (!validArgsFormat(buffer,DCD_ARGS_FORMAT))
    {
        return false;
    }

    switch (buffer.at(0)) {
    case '2':
        emit decimalDigits(TwoDigits);
        break;
    case '3':
        emit decimalDigits(ThreeDigits);
        break;
    default:
        return false;
    }

    return true;
}

bool PfeifferSerialclass::manageMeasurementPointNamesReply()
{
    if (!validArgsFormat(buffer,CID_ARGS_FORMAT))
    {
        return false;
    }

    QString aux_name;
    MeasurementPoint measure_points[6] = { MeasurePoint1, MeasurePoint2,
                                           MeasurePoint3, MeasurePoint4,
                                           MeasurePoint5, MeasurePoint6 };

    for (int i = 0; i < 6; i++)
    {
        aux_name.clear();
        for (int j = 0; j < 4; j++)
        {
            aux_name.append(buffer.at(4*i+j));
        }

        emit measurementPointName(measure_points[i],aux_name);
    }

    return true;
}

bool PfeifferSerialclass::manageUnitsOfMeasurementReply()
{
    if (!validArgsFormat(buffer,UNI_ARGS_FORMAT))
    {
        return false;
    }

    switch (buffer.at(0)) {
    case '0':
        emit unitsOfMeasurement(mBar);
        break;
    case '1':
        emit unitsOfMeasurement(Torr);
        break;
    case '2':
        emit unitsOfMeasurement(Pascal);
        break;
    default:
        return false;
    }

    return true;
}

bool PfeifferSerialclass::manageBaragraphModeReply()
{
    if (!validArgsFormat(buffer,DCB_ARGS_FORMAT))
    {
        return false;
    }

    switch (buffer.at(0)) {
    case '0':
        emit baragraphMode(BaragraphOff);
        break;
    case '1':
        emit baragraphMode(MeasurementRange);
        break;
    case '2':
        emit baragraphMode(OneDecade);
        break;
    default:
        return false;
    }

    return true;
}

bool PfeifferSerialclass::manageReplyDisplayContrast()
{
    if (!validArgsFormat(buffer,DCC_ARGS_FORMAT))
    {
        return false;
    }

    int contrast = 0;

    for (int i = 0; i < 2; i++)
    {
        char aux_arg = buffer.at(i) - '0';

        if (buffer.at(i) < 0 || buffer.at(i) > 9)
        {
            return false;
        }

        contrast += static_cast<int>(aux_arg*pow(10,1-i));
    }

    if (contrast > 20)
    {
        return false;
    }

    emit displayContrast(contrast);
    return true;
}

bool PfeifferSerialclass::manageScreenSaveReply()
{
    if (!validArgsFormat(buffer,DCS_ARGS_FORMAT))
    {
        return false;
    }

    int interval = 0;

    for (int i = 0; i < 2; i++)
    {
        char aux_arg = buffer.at(i) - '0';

        if (buffer.at(i) < 0 || buffer.at(i) > 9)
        {
            return false;
        }

        interval += static_cast<int>(aux_arg*pow(10,1-i));
    }

    if (interval > 99)
    {
        return false;
    }

    emit screenSave(interval);
    return true;
}

bool PfeifferSerialclass::manageThresholdValueSettingReply()
{
    if (!validArgsFormat(buffer,SP_X_ARGS_FORMAT))
    {
        return false;
    }

    PfeifferRequestStruct *request = static_cast<PfeifferRequestStruct*>(
                request_queue.first());

    Relay relay;
    char mnenumonic_idd = request->mneumonic.at(2).toLatin1();
    switch (mnenumonic_idd) {
    case 'A':
        relay = Relay1;
        break;
    case 'B':
        relay = Relay2;
        break;
    case 'C':
        relay = Relay3;
        break;
    case 'D':
        relay = Relay4;
        break;
    case 'E':
        relay = Relay5;
        break;
    case 'F':
        relay = Relay6;
        break;
    default:
        return false;
    }

    Sensor sensor;
    char sensor_idd = buffer.at(0);
    switch (sensor_idd) {
    case '0':
        sensor = Sensor1;
        break;
    case '1':
        sensor = Sensor2;
        break;
    case '2':
        sensor = Sensor3;
        break;
    case '3':
        sensor = Sensor4;
        break;
    case '4':
        sensor = Sensor5;
        break;
    case '5':
        sensor = Sensor6;
        break;
    default:
        return false;
    }

    QString thresholds[2];
    for (int i = 0; i < 2; i++)
    {
        for (int j = 0; j < 7; j++)
        {
            thresholds[i].append(buffer.at(1+j+i*7));
        }
    }

    bool lower_threshold_cast, upper_threshold_cast;
    float lower_threshold = thresholds[0].toFloat(&lower_threshold_cast);
    float upper_threshold = thresholds[1].toFloat(&upper_threshold_cast);

    if (lower_threshold && upper_threshold)
    {
        emit thresholdValueSetting(relay,sensor,
                                   lower_threshold,upper_threshold);
    }

    return lower_threshold && upper_threshold;
}
