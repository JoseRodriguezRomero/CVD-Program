#include "pfeifferserialclass.h"

#define MAX_QUEUE_LEN           20

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

void addReadRequestToQueue(QVector<void*> &request_queue,
                           int mneumonic_id, QVector<char> mneumonic)
{
    while (request_queue.length() > MAX_QUEUE_LEN)
    {
        delete request_queue.at(0);
        request_queue.removeAt(0);
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
                            int mneumonic_id, QVector<char> mneumonic,
                            const QVector<char> &args)
{
    while (request_queue.length() > MAX_QUEUE_LEN)
    {
        delete request_queue.at(0);
        request_queue.removeAt(0);
    }

    PfeifferRequestStruct *new_request = new PfeifferRequestStruct;
    new_request->mneumonic_id = mneumonic_id;
    new_request->mneumonic = mneumonic;
    new_request->pending = false;
    new_request->enquiry = true;
    new_request->enquirying = false;
    new_request->args.clear();
    new_request->args.append(args);

    request_queue.append(new_request);
}

PfeifferSerialclass::PfeifferSerialclass(QObject *parent)
{
    this->setParent(parent);

    serial_port = nullptr;

    reconnect_timer.setInterval(1000);
    reconnect_timer.setSingleShot(false);

    event_timer.setInterval(30);
    event_timer.setSingleShot(false);

    port_name = "COM7";
    baud_rate = QSerialPort::Baud9600;
    stop_bits = PFEIFFER_STOP_BITS;
    data_bits = PFEIFFER_DATA_BITS;
    port_parity = PFEIFFER_PARITY;
    flow_control = PFEIFFER_FLOW_CONTROL;

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

bool PfeifferSerialclass::deviceConnected() const
{
    if (serial_port == nullptr)
    {
        return false;
    }

    QSerialPort::SerialPortError error = serial_port->error();

    if (error == QSerialPort::NotOpenError ||
            error == QSerialPort::OpenError)
    {
        return false;
    }

    return true;
}

bool PfeifferSerialclass::deviceDisconnected() const
{
    return !deviceConnected();
}

void PfeifferSerialclass::processRequestQueue()
{
    PfeifferRequestStruct *request =static_cast<PfeifferRequestStruct*>(
                request_queue.first());

    QVector<char> mneumonic = request->mneumonic;
    QByteArray msg;
    msg.clear();

    for (int i = 0; i < mneumonic.length(); i++)
    {
        msg.append(mneumonic.at(i));
    }

    QVector<char> args = request->args;

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
            request->pending = true;
        }
    }
}

void PfeifferSerialclass::requestReadSensorStatuses()
{
    addReadRequestToQueue(request_queue,SEN_ID,SEN);
}

void PfeifferSerialclass::requestReadSensorControl(const Sensor sensor)
{
    QVector<char> mneumonic(SC_X_);
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
    addReadRequestToQueue(request_queue,SCX_ID,mneumonic);
}

void PfeifferSerialclass::requestReadStatusAndPressure(
        const Sensor sensor)
{
    QVector<char> mneumonic(PR_X_);
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
    addReadRequestToQueue(request_queue,PRX_ID,mneumonic);
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
    addReadRequestToQueue(request_queue, DCC_ID,DCC);
}

void PfeifferSerialclass::requestWriteSensorStatus(
        const PfeifferSerialclass::Sensor sensor,
        const PfeifferSerialclass::SensorStatus status)
{
    QVector<char> statuses;
    for (int i = 0; i < 6; i++)
    {
        statuses[i] = '0';
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
        statuses[sensor_num] = '2';
        break;
    case PfeifferSerialclass::Off:
        statuses[sensor_num] = '1';
        break;
    default:
        return;
    }

    addWriteRequestToQueue(request_queue,SEN_ID,SEN,statuses);
}

void PfeifferSerialclass::requestWriteSensorControl(
        const PfeifferSerialclass::Sensor sensor,
        const PfeifferSerialclass::ControllingSource switch_on,
        const PfeifferSerialclass::ControllingSource switch_off,
        const float switch_on_value, const float switch_off_value)
{
    QVector<char> mneumonic(SC_X_);
    QVector<char> args;

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
        args.append('0');
        break;
    case Sensor2Control:
        args.append('1');
        break;
    case Sensor3Control:
        args.append('2');
        break;
    case Sensor4Control:
        args.append('3');
        break;
    case Sensor5Control:
        args.append('4');
        break;
    case Sensor6Control:
        args.append('5');
        break;
    case ExternalControl:
        args.append('6');
        break;
    case ManualControl:
        args.append('7');
        break;
    case HotStart:
        args.append('8');
        break;
    default:
        return;
    }

    switch (switch_off) {
    case Sensor1Control:
        args.append('0');
        break;
    case Sensor2Control:
        args.append('1');
        break;
    case Sensor3Control:
        args.append('2');
        break;
    case Sensor4Control:
        args.append('3');
        break;
    case Sensor5Control:
        args.append('4');
        break;
    case Sensor6Control:
        args.append('5');
        break;
    case ExternalControl:
        args.append('6');
        break;
    case ManualControl:
        args.append('7');
        break;
    default:
        return;
    }

    QString switch_on_value_string;
    QString switch_off_value_string;

    switch_on_value_string.sprintf("%1.1E",switch_on_value);
    switch_off_value_string.sprintf("%1.1E",switch_off_value);

    QString *value_args[2] = {&switch_on_value_string,
                              &switch_off_value_string};

    for (int i = 0; i < 2; i++)
    {
        for (int j = 0; j < 7; j++)
        {
            args.append(value_args[i]->at(j).toLatin1());
        }
    }

    addWriteRequestToQueue(request_queue,SCX_ID,mneumonic,args);
}

void PfeifferSerialclass::requestWriteDecimalDigits(
        const PfeifferSerialclass::DecimalDigits digits)
{
    QVector<char> arg;

    switch (digits) {
    case TwoDigits:
        arg.append('2');
        break;
    case ThreeDigits:
        arg.append('3');
        break;
    default:
        return;
    }

    addWriteRequestToQueue(request_queue,DCD_ID,DCD,arg);
}

void PfeifferSerialclass::requestWriteMeasurementPointNames(
        const QString names[6])
{
    QVector<char> args;

    for (int i = 0; i < 6; i++)
    {
        QString aux_name = names[i];

        while (aux_name.length() < 4) {
            aux_name.append(' ');
        }

        for (int j = 0; j < 4; j++)
        {
            args.append(aux_name.at(j).toLatin1());
        }
    }

    addWriteRequestToQueue(request_queue,CID_ID,CID,args);
}

void PfeifferSerialclass::requestWriteUnitsOfMeasurement(
        const PfeifferSerialclass::Units units)
{
    QVector<char> arg;

    switch (units) {
    case mBar:
        arg.append('0');
        break;
    case Torr:
        arg.append('1');
        break;
    case Pascal:
        arg.append('2');
        break;
    default:
        return;
    }

    addWriteRequestToQueue(request_queue,UNI_ID,UNI,arg);
}

void PfeifferSerialclass::requestWriteBaragraph(
        PfeifferSerialclass::BaragraphMode bar_mode)
{
    QVector<char> arg;

    switch (bar_mode) {
    case BaragraphOff:
        arg.append('0');
        break;
    case MeasurementRange:
        arg.append('1');
        break;
    case OneDecade:
        arg.append('2');
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

    QString contrast_str = QString::number(contrast);
    QVector<char> args;
    args.append(contrast_str.at(0).toLatin1());
    args.append(contrast_str.at(1).toLatin1());

    addWriteRequestToQueue(request_queue,DCC_ID,DCC,args);
}

bool PfeifferSerialclass::checkState()
{
    QString reply_string;
    bool status = false;

    if (serial_port == nullptr)
    {
        emit ErrorString("Pfeiffer: CONNECTION ERROR", false);
        disconnectDevice();
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
    serial_port->setFlowControl(flow_control);
    serial_port->setStopBits(stop_bits);
    serial_port->setDataBits(data_bits);
    serial_port->setParity(port_parity);

    if (serial_port->open(QIODevice::ReadWrite))
    {
        emit ErrorString("Pfeiffer: CONNECTED", true);
        emit deviceConnected(serial_port->error());
        event_timer.start();
        reconnect_timer.stop();
    }
    {
        serial_port->deleteLater();
        serial_port = nullptr;
        emit ErrorString("Pfeiffer: CONNECTION ERROR", true);
        emit deviceConnected(QSerialPort::NotOpenError);
        event_timer.stop();
        reconnect_timer.start();
    }
}

void PfeifferSerialclass::disconnectDevice()
{
    if (serial_port == nullptr)
    {
        return;
    }

    event_timer.stop();
    reconnect_timer.stop();

    serial_port->close();
    serial_port->deleteLater();
    serial_port = nullptr;
}

bool PfeifferSerialclass::processPending() const
{
    PfeifferRequestStruct *request = static_cast<PfeifferRequestStruct*>(
                request_queue.first());

    return request->pending;
}

void PfeifferSerialclass::manageReply()
{
    PfeifferRequestStruct *request = static_cast<PfeifferRequestStruct*>(
                request_queue.first());

    if (!(serial_port->isReadable() && serial_port->isDataTerminalReady()))
    {
        return;
    }

    QByteArray read_buffer = serial_port->readAll();
    serial_port->flush();

    if ((read_buffer.at(read_buffer.length()-2) != '\r') ||
            (read_buffer.at(read_buffer.length()-1) != '\n'))
    {
        request->pending = false;
        event_timer.start();
    }

    if (request->enquiry)
    {
        if (request->enquirying)
        {
            buffer = read_buffer;
            bool valid_reply = true;

            switch (request->mneumonic_id) {
            case BAU_ID:
                break;
            case CAX_ID:
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
                valid_reply = manageStatusAndPressureReply();
                break;
            case PUC_ID:
                break;
            case RSX_ID:
                break;
            case SAV_ID:
                break;
            case SCX_ID:
                valid_reply = manageSensorControlReply();
                break;
            case SEN_ID:
                valid_reply = manageSensorStatusReply();
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
                valid_reply = manageUnitsOfMeasurementReply();
                break;
            case WDT_ID:
                break;
            }

            buffer.clear();

            if (valid_reply)
            {
                delete request;
                request_queue.remove(0);
                request = static_cast<PfeifferRequestStruct*>(
                            request_queue.first());
                request->pending = false;
            }
            else
            {
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

                serial_port->flush();
                serial_port->write(enq);

                if (serial_port->waitForBytesWritten())
                {
                    request->enquirying = true;
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
        request = static_cast<PfeifferRequestStruct*>(
                    request_queue.first());
        request->pending = false;
    }
}

bool PfeifferSerialclass::manageSensorStatusReply()
{
    if (buffer.length() == 8)
    {
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

    return false;
}

bool PfeifferSerialclass::manageSensorControlReply()
{
    PfeifferRequestStruct *request = static_cast<PfeifferRequestStruct*>(
                request_queue.first());

    if (buffer.length() != 20)
    {
        return false;
    }

    Sensor sensor;
    char mnenumonic_idd = request->mneumonic.last();

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

    bool switch_on_cast;
    bool switch_off_cast;

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
    PfeifferRequestStruct *request = static_cast<PfeifferRequestStruct*>(
                request_queue.first());

    if (buffer.length() != 11)
    {
        return false;
    }

    Sensor sensor;
    char mnenumonic_idd = request->mneumonic.last();

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
        pressure_value_string.append(buffer.at(i+1));
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
    if (buffer.length() != 3)
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
    if (buffer.length() != 26)
    {
        return false;
    }

    QString aux_name;
    MeasurementPoint measure_points[6] = {MeasurePoint1, MeasurePoint2,
                                          MeasurePoint3, MeasurePoint4,
                                          MeasurePoint5, MeasurePoint6};

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
    if (buffer.length() != 3)
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
    if (buffer.length() != 3)
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
    if (buffer.length() != 4)
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
