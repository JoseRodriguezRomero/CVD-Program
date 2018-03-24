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
    int mneumonic_id;
    QString mneumonic;
    QVector<QString> args;
    bool write_request;
    bool pending;
};

void deleteMKSRequest(void* request_ptr)
{
    MKSRequestStruct *aux_ptr;
    aux_ptr = static_cast<MKSRequestStruct*>(request_ptr);
    delete aux_ptr;
}

void addReadRequestToQueue(QVector<void*> &request_queue, int mneumonic_id,
                           QString mneumonic, MKSSerialClass::Channel channel)
{
    while (request_queue.length() > MAX_QUEUE_LEN)
    {
        deleteMKSRequest(request_queue.at(0));
        request_queue.removeAt(0);
    }

    MKSRequestStruct *new_request = new MKSRequestStruct;
    new_request->mneumonic_id = mneumonic_id;
    new_request->mneumonic = mneumonic;
    new_request->pending = false;
    new_request->write_request = false;
    new_request->args.clear();

    switch (channel) {
    case MKSSerialClass::Channel1:
        new_request->mneumonic.append("1");
        break;
    case MKSSerialClass::Channel2:
        new_request->mneumonic.append("2");
        break;
    case MKSSerialClass::NoChannel:
        break;
    default:
        return;
    }

    request_queue.append(new_request);
}

void addWriteRequestToQueue(QVector<void*> &request_queue, int mneumonic_id,
                            QString mneumonic, MKSSerialClass::Channel channel,
                            const QVector<QString> &args)
{
    while (request_queue.length() > MAX_QUEUE_LEN)
    {
        deleteMKSRequest(request_queue.at(0));
        request_queue.removeAt(0);
    }

    MKSRequestStruct *new_request = new MKSRequestStruct;
    new_request->mneumonic_id = mneumonic_id;
    new_request->mneumonic = mneumonic;
    new_request->pending = false;
    new_request->write_request = true;
    new_request->args = args;

    switch (channel) {
    case MKSSerialClass::Channel1:
        new_request->mneumonic.append("1");
        break;
    case MKSSerialClass::Channel2:
        new_request->mneumonic.append("2");
        break;
    case MKSSerialClass::NoChannel:
        break;
    default:
        return;
    }

    request_queue.append(new_request);
}

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
        request->pending = false;
        return;
    }

    QVector<QString> args;
    args.resize(1);

    for (int i = 0; i < buffer.length()-1; i++)
    {
        if (buffer.at(i) != ',')
        {
            args.last().append(buffer.at(i));
        }
        else
        {
            args.append("");
        }
    }

    bool valid_reply;

    switch (request->mneumonic_id) {
    case DT_ID:
        emit displayText(args.at(0));
        valid_reply = true;
        break;
    case KY_ID:
        valid_reply = manageRequestKeyReply(args);
        break;
    case DG_ID:
        valid_reply = manageDisplayDialogReply(args);
        break;
    case ID_ID:
        valid_reply = manageIDStringReply(args);
        break;
    case RT_ID:
        break;
    case AC_ID:
        break;
    case AV_ID:
        break;
    case SP_ID:
        break;
    case EX_ID:
        break;
    case ST_ID:
        break;
    case VL_ID:
        break;
    case RL_ID:
        break;
    case DP_ID:
        break;
    case DP4_ID:
        break;
    case RG_ID:
        break;
    case GN_ID:
        break;
    case OF_ID:
        break;
    case RTD_ID:
        break;
    case AZ_ID:
        break;
    case IN_ID:
        break;
    case OT_ID:
        break;
    case EI_ID:
        break;
    case EO_ID:
        break;
    case SM_ID:
        break;
    case LN_ID:
        break;
    case LS_ID:
        break;
    case LM_ID:
        break;
    case DB_ID:
        break;
    case UP_ID:
        break;
    case LL_ID:
        break;
    case FR_ID:
        break;
    case FT_ID:
        break;
    case PY_ID:
        break;
    case BD_ID:
        break;
    case AD_ID:
        break;
    case IM_ID:
        break;
    case RS_ID:
        break;
    case RE_ID:
        break;
    case DF_ID:
        break;
    case UNLOCK_ID:
        break;
    case LOCK_ID:
        break;
    default:
        valid_reply = false;
        break;
    }

    if (!valid_reply)
    {
        request->pending = false;
        return;
    }

    buffer.clear();

    delete request;
    request_queue.remove(0);

    request = static_cast<MKSRequestStruct*>(request_queue.first());
    request->pending = false;
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

    msg.append('?');

    if (request->write_request)
    {
        msg.append('!');
    }

    for (int i = 0; i < request->mneumonic.length(); i++)
    {
        msg.append(request->mneumonic.at(i).toLatin1());
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

void MKSSerialClass::requestReadDisplayText()
{
    addReadRequestToQueue(request_queue,DT_ID,DT,NoChannel);
}

void MKSSerialClass::requestReadLastKey()
{
    addReadRequestToQueue(request_queue,KY_ID,KY,NoChannel);
}

void MKSSerialClass::requestReadDisplayDialog()
{
    addReadRequestToQueue(request_queue,DG_ID,DG,NoChannel);
}

void MKSSerialClass::requestReadIDString()
{
    addReadRequestToQueue(request_queue,ID_ID,ID,NoChannel);
}

void MKSSerialClass::requestReadRemoteMode()
{
    addReadRequestToQueue(request_queue,RT_ID,RT,NoChannel);
}

void MKSSerialClass::requestReadAccessChannel(
        const MKSSerialClass::Channel channel)
{
    addReadRequestToQueue(request_queue,AC_ID,AC,channel);
}

void MKSSerialClass::requestActualValue(const MKSSerialClass::Channel channel)
{
    addReadRequestToQueue(request_queue,AV_ID,AV,channel);
}

void MKSSerialClass::requestReadSetpoint(const MKSSerialClass::Channel channel)
{
    addReadRequestToQueue(request_queue,SP_ID,SP,channel);
}

void MKSSerialClass::requestWriteDisplayText(const QString &text)
{
    QVector<QString> args;
    args.append(text);
    addWriteRequestToQueue(request_queue,DT_ID,DT,NoChannel,args);
}

void MKSSerialClass::requestWriteDisplayDialog(
        const MKSSerialClass::DisplayDialog dialog)
{
    QVector<QString> args;

    switch (dialog) {
    case ConfigurableDisplay0:
        args.append("0");
        break;
    case ConfigurableDisplay1:
        args.append("1");
        break;
    case ConfigurableDisplay2:
        args.append("2");
        break;
    case UserDisplay:
        args.append("3");
        break;
    case StatusDisplay:
        args.append("4");
        break;
    case AutoTriggerAndSwitches:
        args.append("5");
        break;
    case SetupMode:
        args.append("6");
        break;
    case SetupDisplay1And2:
        args.append("7");
        break;
    case SetupDisplay3And4:
        args.append("8");
        break;
    case SetpointSetup:
        args.append("9");
        break;
    case RangeSetup:
        args.append("10");
        break;
    case Gain1:
        args.append("11");
        break;
    case Gain2:
        args.append("12");
        break;
    case RTDOffset:
        args.append("13");
        break;
    case Channel1IOScale:
        args.append("14");
        break;
    case Channel2IOScale:
        args.append("15");
        break;
    default:
        return;
    }

    addWriteRequestToQueue(request_queue,DG_ID,DG,NoChannel,args);
}

void MKSSerialClass::requestWriteRemoteMode(
        const MKSSerialClass::RemoteMode mode)
{
    QVector<QString> args;

    switch (mode) {
    case RemoteModeOn:
        args.append("ON");
        break;
    case RemoteModeOff:
        args.append("OFF");
        break;
    default:
        return;
    }

    addWriteRequestToQueue(request_queue,RT_ID,RT,NoChannel,args);
}

void MKSSerialClass::requestWriteAccessChannel(
        const MKSSerialClass::Channel channel, float setpoint)
{
    QVector<QString> args;
    args.append(QString::number(setpoint,'f',2));

    addWriteRequestToQueue(request_queue,AC_ID,AC,channel,args);
}

void MKSSerialClass::requestWriteAccessChannel(
        const MKSSerialClass::Channel channel, float setpoint,
        const bool valve_open)
{
    QVector<QString> args;
    args.append(QString::number(setpoint,'f',2));

    switch (valve_open) {
    case true:
        args.append("ON");
        break;
    default:
        args.append("OFF");
        break;
    }

    addWriteRequestToQueue(request_queue,AC_ID,AC,channel,args);
}

void MKSSerialClass::requestWriteActualValue(
        const MKSSerialClass::Channel channel, float setpoint)
{
    QVector<QString> args;
    args.append(QString::number(setpoint,'f',2));

    addWriteRequestToQueue(request_queue,AV_ID,AV,channel,args);
}

void MKSSerialClass::requestWriteSetpoint(const MKSSerialClass::Channel channel,
                                          float setpoint)
{
    QVector<QString> args;
    args.append(QString::number(setpoint,'f',2));

    addWriteRequestToQueue(request_queue,SP_ID,SP,channel,args);
}

bool MKSSerialClass::manageErrorReply()
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
        return false;
    }

    emit errorMessage(error_message);
    return true;
}

bool MKSSerialClass::manageRequestKeyReply(const QVector<QString> &args)
{
    int key = args.at(0).toInt();

    switch (key) {
    case OffKey:
        break;
    case OnKey:
        break;
    case EscKey:
        break;
    case EnterKey:
        break;
    case RightKey:
        break;
    case LeftKey:
        break;
    case UpKey:
        break;
    case DownKey:
        break;
    case NoKey:
        break;
    default:
        return false;
    }

    emit lastKey(static_cast<Key>(key),args.at(1).toInt());
    return true;
}

bool MKSSerialClass::manageDisplayDialogReply(const QVector<QString> &args)
{
    int dialog = args.at(0).toInt();

    switch (dialog) {
    case ConfigurableDisplay0:
        break;
    case ConfigurableDisplay1:
        break;
    case ConfigurableDisplay2:
        break;
    case UserDisplay:
        break;
    case StatusDisplay:
        break;
    case AutoTriggerAndSwitches:
        break;
    case SetupMode:
        break;
    case SetupDisplay1And2:
        break;
    case SetupDisplay3And4:
        break;
    case SetpointSetup:
        break;
    case RangeSetup:
        break;
    case Gain1:
        break;
    case Gain2:
        break;
    case RTDOffset:
        break;
    case Channel1IOScale:
        break;
    case Channel2IOScale:
        break;
    case SignalProcessingMode1:
        break;
    case SignalProcessingMode2:
        break;
    case Linearization1:
        break;
    case Linearization2:
        break;
    case LimitMode1:
        break;
    case LimitDefinitions1:
        break;
    case LimitMode2:
        break;
    case LimitDefinitions2:
        break;
    case RelayFormulas:
        break;
    case TemporaryFormulas1And2:
        break;
    case TemporaryFormulas3And4:
        break;
    case InterfaceSetup1:
        break;
    case InterfaceSetup2:
        break;
    case ResetToDefault:
        break;
    default:
        return false;
    }

    emit displayDialog(static_cast<DisplayDialog>(dialog));
    return true;
}

bool MKSSerialClass::manageIDStringReply(const QVector<QString> &args)
{
    QString reply = args.at(0);
    if (reply.length() != 13)
    {
        return false;
    }

    QString init,version,release,serial_number;

    for (int i = 0; i < 4; i++)
    {
        init.append(reply.at(i));
        serial_number.append(reply.at(8+i));
    }

    for (int i = 0; i < 2; i++)
    {
        version.append(4+i);
        release.append(6+i);
    }

    if (init != "PR42")
    {
        return false;
    }

    emit IDString(version,release,serial_number);
    return true;
}
