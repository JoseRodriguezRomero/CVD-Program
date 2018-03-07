#include "eurothermserialclass.h"

#include <QDebug>

#define MAX_QUEUE_LEN           20

#define EUROTHERM_DEFAULT_PORT_NAME         "COM8"
#define EUROTHERM_DEFAULT_PARITY            QSerialPort::NoParity
#define EUROTHERM_DEFAULT_BAUD_RATE         QSerialPort::Baud19200
#define EUROTHERM_DEFAULT_STOP_BITS         QSerialPort::OneStop
#define EUROTHERM_DEFAULT_DATA_BITS         QSerialPort::Data8
#define EUROTHERM_DEFAULT_FLOW_CONTROL      QSerialPort::NoFlowControl

#define IEEE_REGION             0x8000
#define CRC_16_ANSI_POLY        0x18005

/* Modbus Adressess for Eurotherm 32xx series */

#define PV_IN                   1       // (input-register)     float32
#define TG_SP                   2       // (holding-register)   float32             Do not write continously on this value!!
#define MAN_OP                  3       // (input-register)     float32
#define WRK_OP                  4       // (input-register)     float32
#define WRK_SP                  5       // (input-register)     float32
#define PB                      6       // (holding-register)   float32
#define CTRL_A                  7       // (input-register)     uint8
#define TI                      8       // (holding-register)   float32
#define TD                      9       // (holding-register)   float32
#define RNG_LO                  11      // (input-register)     float32
#define RNG_HI                  12      // (input-register)     float32
#define A1                      13      // (holding-register)   float32
#define A2                      14      // (holding-register)   float32
#define SP_SEL                  15      // (holding-register)   uint8
#define D_BAND                  16      // (holding-register)   float32
#define CB_LO                   17      // (holding-register)   float32
#define CB_HI                   18      // (holding-register)   float32
#define R2G                     19      // (holding-register)   float32
#define T_STAT                  23      // (holding-resiter)    uint8
#define SP1                     24      // (holding-register)   float32             Do not write continously on this value!!
#define SP2                     25      // (holding-register)   float32             Do not write continously on this value!!
#define RM_SP                   26      // (holding-register)   float32
#define LOC_T                   27      // (holding-register)   float32
#define MR                      28      // (holding-register)   float32
#define OP_HI                   30      // (holding-register)   float32
#define OP_LO                   31      // (holding-register)   float32
#define SAFE                    34      // (holding-register)   float32
#define SP_RAT                  35      // (holding-register)   float32
#define P_ERR                   39      // (input-register)     float32
#define A1_HYS                  47      // (holding-register)   float32
#define A2_HYS                  68      // (holding-register)   float32
#define A3_HYS                  69      // (holding-register)   float32
#define A4_HYS                  71      // (holding-register)   float32
#define STAT                    75      // (input-register)     int16
#define LL_AMP                  79      // (input-register)     float32
#define LD_AMP                  80      // (input-register)     float32
#define A3                      81      // (holding-register)   float32
#define A4                      82      // (holding-register)   float32
#define LBT                     83      // (holding-register)   float32
#define HYST_H                  86      // (holding-register)   float32
#define DI_IP                   87      // (input-register)     uint8
#define HYST_C                  88
#define FILT_T                  101     // (holding-register)   float32
#define HOME                    106     // (holding-register)   uint8
#define SP_HI                   111     // (holding-register)   float32
#define SP_LO                   112     // (holding-register)   float32
#define ADDR                    131     // (holding-register)   uint8
#define PV_OFS                  141     // (holding-register)   float32
#define C_ADJ                   146
#define IM                      199     // (holding-register)   uint8
#define MV_IN                   202     // (input-register)     float32
#define PV_CM                   203     // (holding-register)   float32
#define CJC_IN                  215     // (input-register)     float32
#define SBR                     258     // (discrete-input)     bool
#define NEW_AL                  260     // (input-register)     uint8
#define LBR                     263     // (discrete-input)     bool
#define A_TUNE                  270     // (coil)               bool
#define A_M                     273     // (holding-register)   uint8
#define AC_ALL                  274     // (holding-register)   uint8
#define L_R                     276     // (holding-register)   uint8
#define A1_STS                  294     // (discrete-input)     bool
#define A2_STS                  295
#define A3_STS                  296
#define A4_STS                  297
#define LD_ALM                  304     // (holding-register)   float32
#define LK_ALM                  305     // (holding-register)   float32
#define HC_ALM                  306     // (holding-register)   float32
#define LOAD_A                  307     // (input-register)     uint8
#define LEAK_A                  308     // (input-register)     uint8
#define HILC_A                  309     // (input-register)     uint8
#define REC_NO                  313     // (holding-register)   uint8
#define STORE                   314     // (holding-register)   uint8
#define TM_CFG                  320     // (holding-register)   uint8
#define TM_RES                  321
#define SS_SP                   322
#define SS_PWR                  323
#define DWELL                   324
#define T_ELAP                  325
#define T_REMN                  326
#define THRES                   327
#define END_T                   328
#define SERVO                   329
#define CTRL_H                  512     // (input-register)     uint8
#define CTRL_C                  513     // (input-register)     uint8
#define PB_UNIT                 514     // (input-register)     uint8
#define LEV2_P                  515     // (input-register)     uint16
#define UNITS                   516     // (holding-register)   uint8
#define LEV_3P                  517     // (input-register)     int16
#define CONF_P                  518     // (input-register)     int16
#define COLD                    519     // (input-register)     uint8
#define COOL_T                  524
#define DEC_P                   525     // (input-register)     uint8
#define STBY_T                  530     // (holding-register)   uint8
#define RAMP_UNITS              531     // (holding-register)   uint8
#define METER                   532     // (holding-register)   uint8
#define UCAL                    533     // (holding-register)   uint8
#define A1_TYP                  536     // (input-register)     uint8
#define A2_TYP                  537     // (input-register)     uint8
#define A3_TYP                  538     // (input-register)     uint8
#define A4_TYP                  539     // (input-register)     uint8
#define A1_LAT                  540 	// (holding-register)   uint8
#define A2_LAT                  541
#define A3_LAT                  542
#define A4_LAT                  543
#define A1_BLK                  544     // (coil)               bool
#define A2_BLK                  545
#define A3_BLK                  546
#define A4_BLK                  547
#define DI_OP                   551     // (holding-register)   uint8
#define OFS_HI                  560     // (holding-register)   float32
#define OFS_LO                  561     // (holding-register)   float32
#define PNT_HI                  562     // (holding-register)   float32
#define PNT_LO                  563     // (holding-register)   float32
#define CT_RNG                  572     // (input-register)     float32
#define SB_TYP                  578     // (input-register)     uint8
#define CUSTM_ID                629     // (holding-register)   int16
#define PHASE                   768     // (input-register)     uint8
#define GO                      769
#define K_LOC                   1104    // (input-register)     uint8
#define DWEL_1                  1280    // 3208 only (not implementing)
#define TSP_1                   1281    // 3208 only (not implementing)
#define RMP_1                   1282    // 3208 only (not implementing)
#define DWEL_2                  1283    // 3208 only (not implementing)
#define TSP_2                   1284    // 3208 only (not implementing)
#define RMP_2                   1285    // 3208 only (not implementing)
#define DWEL_3                  1286    // 3208 only (not implementing)
#define TSP_3                   1287    // 3208 only (not implementing)
#define RMP_3                   1288    // 3208 only (not implementing)
#define DWEL_4                  1289    // 3208 only (not implementing)
#define TSP_4                   1290    // 3208 only (not implementing)
#define RMP_4                   1291    // 3208 only (not implementing)
#define IN_TYP                  12290   // (input-register)     uint8
#define CJ_TYP                  12291   // (input-register)     uint8
#define MV_HI                   12306
#define MV_LO                   12307
#define L_TYPE                  12352   // (input-register)     uint8
#define L_D_IN                  12353   // (input-register)     uint8
#define L_SENS                  12361   // (input-register)     uint8
#define L_TYPE_LB               12368
#define L_D_IN_LB               12369
#define L_SENS_LB               12377
#define ID                      12544   // (input-register)     uint8
#define BAUD                    12548   // (input-register)     uint8
#define PRTY                    12549   // (input-register)     uint8
#define DELAY                   12550   // (input-register)     uint8
#define RETRN                   12551   // (input-register)     uint8
#define REG_AD                  12552   // (input-register)     int16
#define CT_ID                   12608   // (input-register)     uint8
#define CT_SRC                  12609   // (input-register)     uint8
#define CT_LAT                  12610   // (input-register)     uint8
#define ID_1                    12672   // (input-register)     uint8
#define D_IN_1                  12673
#define FUNC_1                  12675   // (input-register)     uint8
#define RNG_1                   12676
#define SRC_A_1                 12678
#define SRC_B_1                 12679
#define SRC_C_1                 12680
#define SRC_D_1                 12681
#define SENS_1                  12682   // (input-register)     uint8
#define PLS_1                   12706   // (holding-register)   float32
#define ID_2                    12736   // (input-register)     uint8
#define FUNC_2                  12739   // (input-register)     uint8
#define RNG_2                   12740
#define SRC_A_2                 12742   // (input-register)     uint8
#define SRC_B_2                 12743   // (input-register)     uint8
#define SRC_C_2                 12744   // (input-register)     uint8
#define SRC_D_2                 12745   // (input-register)     uint8
#define SENS_2                  12746   // (input-register)     uint8
#define PLS_2                   12770
#define ID_3                    12800
#define FUNC_3                  12803
#define RNG_3                   12804
#define SRC_A_3                 12806
#define SRC_B_3                 12807
#define SRC_C_3                 12808
#define SRC_D_3                 12809
#define SENS_3                  12810
#define PLS_3                   12834
#define TYPE_4                  13056   // (input-register)     uint8
#define FUNC_4                  13059   // (input-register)     uint8
#define SRC_A_4                 13062
#define SRC_B_4                 13063
#define SRC_C_4                 13064
#define SRC_D_4                 13065
#define SENS_4                  13066
#define PLS_4                   13090

/* Modbus Adressess for Eurotherm 32xx series */

quint16 crc_lookup_table[65536];  // 2^16 combinations...

enum RegisterType {
    Coil,
    DiscreteInput,
    HoldingRegister,
    InputRegister
};

enum RequestType {
    ReadRequest,
    WriteRequest
};

struct EurothermRequestStruct {
    quint16 server_address;
    quint16 start_address;
    RegisterType reg_type;
    RequestType req_type;
    QVector<quint16> values;
    bool pending;
};

void computeCRC16LookupTable()
{
    const quint64 generator = CRC_16_ANSI_POLY;
    quint64 crc,aux_gen,mask;

    for (int i = 0; i < 65536; i++)
    {
        crc = i;
        crc <<= 16;
        mask = 1;
        mask <<= 31;

        aux_gen = generator;
        aux_gen <<= 15;

        for (int i = 0; i < 16; i++)
        {
            if (mask & crc)
            {
                crc ^= aux_gen;
            }

            mask >>= 1;
            aux_gen >>= 1;
        }

        crc_lookup_table[i] = static_cast<quint16>(crc);
    }
}

quint16 modbus16BitCRC(quint16 server_address, quint16 start_address,
                    const QVector<quint16> &values)
{
    QVector<quint16> data;

    data.append(values);
    quint32 computed_crc = 0x00;

    for (int i = 0; i < data.length(); i++)
    {
        computed_crc ^= data.at(i);
        computed_crc = crc_lookup_table[computed_crc];
    }

    return computed_crc;
}

void add16BitRequestToQueue(QVector<void*> &request_queue,
                            const quint16 server_address,
                            const quint16 start_address,
                            const RegisterType reg_type,
                            const RequestType req_type,
                            const quint16 value = 0)
{
    while (request_queue.length() > MAX_QUEUE_LEN)
    {
        delete request_queue.at(0);
        request_queue.removeAt(0);
    }

    EurothermRequestStruct *new_request = new EurothermRequestStruct;
    new_request->server_address = server_address;
    new_request->start_address = start_address;
    new_request->reg_type = reg_type;
    new_request->req_type = req_type;
    new_request->values.clear();
    new_request->values.append(value);
    new_request->pending = false;

    request_queue.append(new_request);
}

void add8BitRequestToQueue(QVector<void*> &request_queue,
                           const quint16 server_address,
                           const quint16 start_address,
                           const RegisterType reg_type,
                           const RequestType req_type,
                           const quint8 value = 0)
{
    add16BitRequestToQueue(request_queue,server_address,
                           start_address,reg_type,req_type,
                           static_cast<quint16>(value));
}

void addFloatRequestToQueue(QVector<void*> &request_queue,
                            const quint16 server_address,
                            const quint16 start_address,
                            const RegisterType reg_type,
                            const RequestType req_type,
                            const float value = 0)
{
    while (request_queue.length() > MAX_QUEUE_LEN)
    {
        delete request_queue.at(0);
        request_queue.removeAt(0);
    }

    float const* value_ptr = &value;
    quint32 const* cast_val_ptr = reinterpret_cast<quint32 const*>(value_ptr);
    quint32 cast_val = *cast_val_ptr;

    quint16 cast_val_low = static_cast<quint16>(cast_val & 0xFFFF);
    cast_val = cast_val & 0xFFFF0000;
    quint16 cast_val_high = static_cast<quint16>(cast_val >> 16);

    EurothermRequestStruct *new_request = new EurothermRequestStruct;
    new_request->server_address = server_address;
    new_request->start_address = (start_address << 1) | IEEE_REGION;
    new_request->reg_type = reg_type;
    new_request->req_type = req_type;
    new_request->values.append(cast_val_high);
    new_request->values.append(cast_val_low);
    new_request->pending = false;

    request_queue.append(new_request);
}

EurothermSerialClass::EurothermSerialClass(QObject *parent)
{
    this->setParent(parent);

    port_name = EUROTHERM_DEFAULT_PORT_NAME;
    baud_rate = EUROTHERM_DEFAULT_BAUD_RATE;
    port_parity = EUROTHERM_DEFAULT_PARITY;
    stop_bits = EUROTHERM_DEFAULT_STOP_BITS;
    data_bits = EUROTHERM_DEFAULT_DATA_BITS;

    reconnect_timer.setParent(this);
    event_timer.setParent(this);

    reconnect_timer.setInterval(1000);
    event_timer.setInterval(30);

    modbus_client = nullptr;  // never forgetti mom's spaghetti
    reply = nullptr;

    /* Dummy code */

    quint16 server_address = 0x01;
    quint16 start_addresss = 0x09;
    QVector<quint16> msg;
    msg.append(0x43);

    computeCRC16LookupTable();
    modbus16BitCRC(server_address,start_addresss,msg);

    /* Dummy code */
}

EurothermSerialClass::~EurothermSerialClass()
{
    if (modbus_client != nullptr)
    {
        modbus_client->disconnectDevice();
        modbus_client->deleteLater();
    }
}

bool EurothermSerialClass::deviceConnected() const
{
    if (modbus_client == nullptr)
    {
        return false;
    }

    if (modbus_client->state() == QModbusDevice::UnconnectedState)
    {
        return false;
    }

    return true;
}

bool EurothermSerialClass::deviceDisconnected() const
{
    return !deviceConnected();
}

void EurothermSerialClass::connectDevice()
{
    if (modbus_client == nullptr)
    {
        modbus_client = new QModbusRtuSerialMaster(this);
        modbus_client->setTimeout(200);
        modbus_client->setNumberOfRetries(3);
    }
    else if (modbus_client->state() == QModbusDevice::ConnectedState ||
             modbus_client->state() == QModbusDevice::ConnectingState)
    {
        return;
    }

    modbus_client->setConnectionParameter(
                QModbusDevice::SerialPortNameParameter,port_name);
    modbus_client->setConnectionParameter(
                QModbusDevice::SerialBaudRateParameter,baud_rate);
    modbus_client->setConnectionParameter(
                QModbusDevice::SerialParityParameter,port_parity);
    modbus_client->setConnectionParameter(
                QModbusDevice::SerialStopBitsParameter,stop_bits);
    modbus_client->setConnectionParameter(
                QModbusDevice::SerialDataBitsParameter,data_bits);

    modbus_client->connectDevice();
    while (modbus_client->state() == QModbusDevice::ConnectingState)
    {
    }

    if (modbus_client->state() == QModbusDevice::ConnectedState)
    {
        emit stopReconnectTimer();
        emit startEventLoopTimer();
    }
    else
    {
        modbus_client->deleteLater();
        modbus_client = nullptr;
        emit stopEventLoopTimer();
        emit startReconnectTimer();
    }
}

void EurothermSerialClass::disconnectDevice()
{
    if (modbus_client == nullptr)
    {
        return;
    }

    modbus_client->disconnectDevice();
    modbus_client->deleteLater();

    emit stopEventLoopTimer();
    emit stopReconnectTimer();

    modbus_client = nullptr;
    reply = nullptr;
}

bool EurothermSerialClass::checkState()
{
    QString reply_string;
    bool status = false;

    if (modbus_client == nullptr)
    {
        emit errorString("Eurotherm: CONNECTION ERROR",false);
        return false;
    }

    QModbusDevice::Error client_error = modbus_client->error();

    switch (client_error) {
    case QModbusDevice::NoError :
        reply_string = "CONNECTED";
        status = true;
        break;
    case QModbusDevice::ReadError :
        reply_string = "READ ERROR";
        break;
    case QModbusDevice::WriteError :
        reply_string = "WRITE ERROR";
        break;
    case QModbusDevice::ConnectionError :
        reply_string = "CONNECTION ERROR";
        break;
    case QModbusDevice::ConfigurationError :
        reply_string = "CONFIGURATION ERROR";
        break;
    case QModbusDevice::TimeoutError :
        reply_string = "TIMEOUT ERROR";
        break;
    case QModbusDevice::ProtocolError :
        reply_string = "PROTOCOL ERROR";
        break;
    case QModbusDevice::ReplyAbortedError :
        reply_string = "REPLY ABORTED ERROR";
        break;
    default :
        reply_string = "UNKNOWN ERROR";
        break;
    }

    if (!status)
    {
        emit errorString("Eurotherm: " + reply_string,status);
        return false;
    }

    if (modbus_client->state() != QModbusDevice::ConnectedState)
    {
        emit errorString("Eurotherm: CONNECTION ERROR",false);
        return false;
    }

    emit errorString("Eurotherm: " + reply_string,true);
    return status;
}

bool EurothermSerialClass::processPending() const
{
    if (request_queue.isEmpty())
    {
        return false;
    }

    const EurothermRequestStruct *first = static_cast<EurothermRequestStruct*>(
                request_queue.first());

    return first->pending;
}

void EurothermSerialClass::requestReadPVInputValue(const int server_address)
{
    addFloatRequestToQueue(request_queue,server_address,PV_IN,
                           InputRegister,ReadRequest);
}

void EurothermSerialClass::requestReadTargetSetpoint(const int server_address)
{
    addFloatRequestToQueue(request_queue,server_address,TG_SP,
                           HoldingRegister,ReadRequest);
}

void EurothermSerialClass::requestReadManualOutputValue(
        const int server_address)
{
    addFloatRequestToQueue(request_queue,server_address,MAN_OP,
                           InputRegister,ReadRequest);
}

void EurothermSerialClass::requestReadWorkingOutput(const int server_address)
{
    addFloatRequestToQueue(request_queue,server_address,WRK_OP,
                           InputRegister,ReadRequest);
}

void EurothermSerialClass::requestReadWorkingSetpoint(const int server_address)
{
    addFloatRequestToQueue(request_queue,server_address,WRK_SP,
                           InputRegister,ReadRequest);
}

void EurothermSerialClass::requestReadProportionalBand(const int server_address)
{
    addFloatRequestToQueue(request_queue,server_address,PB,HoldingRegister,
                           ReadRequest);
}

void EurothermSerialClass::requestReadControlAction(const int server_address)
{
    add16BitRequestToQueue(request_queue,server_address,CTRL_A,
                           InputRegister,ReadRequest);
}

void EurothermSerialClass::requestReadIntegralTime(const int server_address)
{
    addFloatRequestToQueue(request_queue,server_address,TI,HoldingRegister,
                           ReadRequest);
}

void EurothermSerialClass::requestReadDerivativeTime(const int server_address)
{
    addFloatRequestToQueue(request_queue,server_address,TD,HoldingRegister,
                           ReadRequest);
}

void EurothermSerialClass::requestReadInputRangeLowLimit(
        const int server_address)
{
    addFloatRequestToQueue(request_queue,server_address,RNG_LO,
                           InputRegister,ReadRequest);
}

void EurothermSerialClass::requestReadInputRangeHighLimit(
        const int server_address)
{
    addFloatRequestToQueue(request_queue,server_address,RNG_HI,
                           InputRegister,ReadRequest);
}

void EurothermSerialClass::requestReadAlarmThreshold(const int server_address,
                                                     const Alarm alarm)
{
    int start_address = 0;

    switch (alarm) {
    case Alarm1:
        start_address = A1;
        break;
    case Alarm2:
        start_address = A2;
        break;
    default:
        return;
    }

    addFloatRequestToQueue(request_queue,server_address,start_address,
                           HoldingRegister,ReadRequest);
}

void EurothermSerialClass::requestReadActiveSetpoint(const int server_address)
{
    add8BitRequestToQueue(request_queue,server_address,SP_SEL,
                          HoldingRegister,ReadRequest);
}

void EurothermSerialClass::requestReadChannel2Deadband(const int server_address)
{
    addFloatRequestToQueue(request_queue,server_address,D_BAND,
                           HoldingRegister,ReadRequest);
}

void EurothermSerialClass::requestReadCutbackLow(const int server_address)
{
    addFloatRequestToQueue(request_queue,server_address,CB_LO,
                           HoldingRegister,ReadRequest);
}

void EurothermSerialClass::requestReadCutbackHigh(const int server_address)
{
    addFloatRequestToQueue(request_queue,server_address,CB_HI,
                           HoldingRegister,ReadRequest);
}

void EurothermSerialClass::requestReadRelativeCoolCh2Gain(
        const int server_address)
{
    addFloatRequestToQueue(request_queue,server_address,R2G,
                           HoldingRegister,ReadRequest);
}

void EurothermSerialClass::requestReadTimerStatus(const int server_address)
{
    add8BitRequestToQueue(request_queue,server_address,T_STAT,
                          HoldingRegister,ReadRequest);
}

void EurothermSerialClass::requestReadSetpoint(const int server_address,
                                               int sp_num)
{
    int start_address = 0;

    switch (sp_num) {
    case 1:
        start_address = SP1;
        break;
    case 2:
        start_address = SP2;
        break;
    default:
        return;
    }

    addFloatRequestToQueue(request_queue,server_address,start_address,
                           HoldingRegister,ReadRequest);
}

void EurothermSerialClass::requestReadRemoteSetpoint(const int server_address)
{
    addFloatRequestToQueue(request_queue,server_address,RM_SP,
                           HoldingRegister,ReadRequest);
}

void EurothermSerialClass::requestReadLocalTrim(const int server_address)
{
    addFloatRequestToQueue(request_queue,server_address,LOC_T,
                           HoldingRegister,ReadRequest);
}

void EurothermSerialClass::requestReadManualReset(const int server_address)
{
    addFloatRequestToQueue(request_queue,server_address,MR,HoldingRegister,
                           ReadRequest);
}

void EurothermSerialClass::requestReadOutputHighLimit(const int server_address)
{
    addFloatRequestToQueue(request_queue,server_address,OP_HI,
                           HoldingRegister,ReadRequest);
}

void EurothermSerialClass::requestReadOutputLowLimit(const int server_address)
{
    addFloatRequestToQueue(request_queue,server_address,OP_LO,
                           HoldingRegister,ReadRequest);
}

void EurothermSerialClass::requestReadSafeOutputValueforSensorBreak(
        const int server_address)
{
    addFloatRequestToQueue(request_queue,server_address,SAFE,
                           HoldingRegister,ReadRequest);
}

void EurothermSerialClass::requestReadSetpointRateLimitValue(
        const int server_address)
{
    addFloatRequestToQueue(request_queue,server_address,SP_RAT,
                           HoldingRegister,ReadRequest);
}

void EurothermSerialClass::requestReadCalculatedError(const int server_address)
{
    addFloatRequestToQueue(request_queue,server_address,P_ERR,
                           InputRegister,ReadRequest);
}

void EurothermSerialClass::requestReadAlarmHysteresis(const int server_address,
                                                      const Alarm alarm)
{
    int start_address = 0;

    switch (alarm) {
    case Alarm1:
        start_address = A1_HYS;
        break;
    case Alarm2:
        start_address = A2_HYS;
        break;
    case Alarm3:
        start_address = A3_HYS;
        break;
    case Alarm4:
        start_address = A4_HYS;
        break;
    default:
        return;
    }

    addFloatRequestToQueue(request_queue,server_address,start_address,
                           HoldingRegister,ReadRequest);
}

void EurothermSerialClass::resquestReadInstrumentStatus(
        const int server_address)
{
    addFloatRequestToQueue(request_queue,server_address,STAT,InputRegister,
                           ReadRequest);
}

void EurothermSerialClass::requestWriteTargetSetpoint(
        const int server_address, const float setpoint)
{
    addFloatRequestToQueue(request_queue,server_address,TG_SP,
                           HoldingRegister,WriteRequest,setpoint);
}

void EurothermSerialClass::requestWriteProportionalBand(
        const int server_address, const float pb)
{
    addFloatRequestToQueue(request_queue,server_address,PB,HoldingRegister,
                           WriteRequest,pb);
}

void EurothermSerialClass::requestWriteIntegralTime(const int server_address,
                                                    const float ti)
{
    addFloatRequestToQueue(request_queue,server_address,TI,HoldingRegister,
                           WriteRequest,ti);
}

void EurothermSerialClass::requestWriteDerivativeTime(const int server_address,
                                                      const float td)
{
    addFloatRequestToQueue(request_queue,server_address,TD,HoldingRegister,
                           WriteRequest,td);
}

void EurothermSerialClass::requestWriteAlarmThreshold(const int server_address,
                                                      const Alarm alarm,
                                                      const float a)
{
    int start_address = 0;

    switch (alarm) {
    case Alarm1:
        start_address = A1;
        break;
    case Alarm2:
        start_address = A2;
        break;
    default:
        return;
    }

    addFloatRequestToQueue(request_queue,server_address,start_address,
                           HoldingRegister,WriteRequest,a);
}

void EurothermSerialClass::requestWriteActiveSetpoint(
        const int server_address, const EurothermSerialClass::Setpoint setpoint)
{
    int sp_sel;

    switch (setpoint) {
    case Setpoint1:
        sp_sel = 0;
        break;
    case Setpoint2:
        sp_sel = 1;
        break;
    default:
        return;
    }

    add8BitRequestToQueue(request_queue,server_address,SP_SEL,
                          HoldingRegister,WriteRequest,sp_sel);
}

void EurothermSerialClass::requestWriteChannel2Deadband(
        const int server_address, const float d_band)
{
    addFloatRequestToQueue(request_queue,server_address,A2,HoldingRegister,
                           WriteRequest,d_band);
}

void EurothermSerialClass::requestWriteCutbackLow(
        const int server_address, const float cb_lo)
{
    addFloatRequestToQueue(request_queue,server_address,CB_LO,
                           HoldingRegister,WriteRequest,cb_lo);
}

void EurothermSerialClass::requestWriteCutbackHigh(
        const int server_address, const float cb_hi)
{
    addFloatRequestToQueue(request_queue,server_address,CB_HI,
                           HoldingRegister,WriteRequest,cb_hi);
}

void EurothermSerialClass::requestWriteRelativeCoolCh2Gain(
        const int server_address, const float r2g)
{
    addFloatRequestToQueue(request_queue,server_address,R2G,
                           HoldingRegister,WriteRequest,r2g);
}

void EurothermSerialClass::requestWriteTimerStatus(
        const int server_address,
        const EurothermSerialClass::TimerStatus status)
{
    int status_sel;

    switch (status) {
    case Reset:
        status_sel = 0;
        break;
    case Run:
        status_sel = 1;
        break;
    case Hold:
        status_sel = 2;
        break;
    case End:
        status_sel = 3;
        break;
    default:
        return;
    }

    add8BitRequestToQueue(request_queue,server_address,T_STAT,
                          HoldingRegister,WriteRequest,status_sel);
}

void EurothermSerialClass::requestWriteSetpoint(const int server_address,
                                                const int sp_num,
                                                const float sp)
{
    int start_address = 0;

    switch (sp_num) {
    case 1:
        start_address = SP1;
        break;
    case 2:
        start_address = SP2;
        break;
    default:
        return;
    }

    addFloatRequestToQueue(request_queue,server_address,start_address,
                           HoldingRegister,WriteRequest,sp);
}

void EurothermSerialClass::requestWriteRemoteSetpoint(
        const int server_address, const float rm_sp)
{
    addFloatRequestToQueue(request_queue,server_address,RM_SP,
                           HoldingRegister,WriteRequest,rm_sp);
}

void EurothermSerialClass::requestWriteLocalTrim(const int server_address,
                                                 const float loc_t)
{
    addFloatRequestToQueue(request_queue,server_address,LOC_T,
                           HoldingRegister,WriteRequest,loc_t);
}

void EurothermSerialClass::requestWriteManualReset(
        const int server_address, const float mr)
{
    addFloatRequestToQueue(request_queue,server_address,MR,HoldingRegister,
                           WriteRequest,mr);
}

void EurothermSerialClass::requestWriteOutputHighLimit(
        const int server_address, const float op_hi)
{
    addFloatRequestToQueue(request_queue,server_address,OP_HI,
                           HoldingRegister,WriteRequest,op_hi);
}

void EurothermSerialClass::requestWriteOutputLowLimit(
        const int server_address, const float op_lo)
{
    addFloatRequestToQueue(request_queue,server_address,OP_LO,
                           HoldingRegister,WriteRequest,op_lo);
}

void EurothermSerialClass::requestWriteSafeOutputValueforSensorBreak(
        const int server_address, const float safe)
{
    addFloatRequestToQueue(request_queue,server_address,SAFE,
                           HoldingRegister,WriteRequest,safe);
}

void EurothermSerialClass::requestWriteSetpointRateLimitValue(
        const int server_address, const float sp_rat)
{
    addFloatRequestToQueue(request_queue,server_address,SP_RAT,
                           HoldingRegister,WriteRequest,sp_rat);
}

void EurothermSerialClass::requestWriteAlarmHysteresis(const int server_address,
                                                       const Alarm alarm,
                                                       const float a_hys)
{
    int start_address = 0;

    switch (alarm) {
    case Alarm1:
        start_address = A1_HYS;
        break;
    case Alarm2:
        start_address = A2_HYS;
        break;
    case Alarm3:
        start_address = A3_HYS;
        break;
    case Alarm4:
        start_address = A4_HYS;
        break;
    default:
        return;
    }

    addFloatRequestToQueue(request_queue,server_address,start_address,
                           HoldingRegister,WriteRequest, a_hys);
}

void EurothermSerialClass::manageReply()
{
    EurothermRequestStruct *request =static_cast<EurothermRequestStruct*>(
                request_queue.first());

    if (reply == nullptr || modbus_client == nullptr)
    {
        request->pending = false;
        return;
    }

    if (!(reply->isFinished()))
    {
        request->pending = true;
        return;
    }

    if (reply->error() != QModbusDevice::NoError)
    {
        request->pending = false;
        reply->deleteLater();
        reply = nullptr;
        failed_attempts++;
        return;
    }

    QModbusDataUnit data_unit = reply->result();
    int server_address = reply->serverAddress();

    if (!data_unit.isValid())
    {
        reply->deleteLater();
        reply = nullptr;
        request->pending = false;
        failed_attempts++;
        return;
    }

    quint8  value_uint8  = reply->result().value(0);
    quint16 value_uint16 = reply->result().value(0);

    quint32 value_uint32 = 0;
    int value_count = reply->result().valueCount();

    for (int i = 0; i < value_count; i++)
    {
        quint16 aux = reply->result().value(i);
        value_uint32 <<= 16;
        value_uint32 = value_uint32 | aux;
    }

    quint16 const* value_uint16_ptr = &value_uint16;
    qint16 const* value_int16_ptr = reinterpret_cast<qint16 const*>(
                value_uint16_ptr);
    qint8 value_int16 = *value_int16_ptr;

    quint32 const *value_uint32_ptr = &value_uint32;
    float const* value_float32_ptr = reinterpret_cast<float const*>(
                value_uint32_ptr);
    float value_float32 = *value_float32_ptr;

    int start_address = data_unit.startAddress();

    if (start_address & IEEE_REGION)
    {
        start_address -= IEEE_REGION;
        start_address >>= 1;
    }

    switch (start_address)
    {
    case PV_IN:
        emit PVInputValue(server_address, value_float32);
        break;
    case TG_SP:
        emit targetSetpoint(server_address,value_float32);
        break;
    case MAN_OP:
        emit manualOutputValue(server_address, value_float32);
        break;
    case WRK_OP:
        emit workingOutput(server_address, value_float32);
        break;
    case WRK_SP:
        emit workingSetpoint(server_address, value_float32);
        break;
    case PB:
        emit proportionalBand(server_address, value_float32);
        break;
    case CTRL_A:
        ControlAction control_action;

        switch (value_uint8) {
        case 0:
            control_action = ReverseActing;
            break;
        case 1:
            control_action = DirectActing;
            break;
        default:
            return;
        }

        emit controlAction(server_address,control_action);
        break;
    case TI:
        emit integralTime(server_address, value_float32);
        break;
    case TD:
        emit derivativeTime(server_address, value_float32);
        break;
    case RNG_LO:
        emit inputRangeLowLimit(server_address, value_float32);
        break;
    case RNG_HI:
        emit inputRangeLowLimit(server_address, value_float32);
        break;
    case A1:
        emit alarmThreshold(server_address, 1, value_float32);
        break;
    case A2:
        emit alarmThreshold(server_address, 2, value_float32);
        break;
    case SP_SEL:
        Setpoint setpoint;
        switch (value_uint8) {
        case 0:
            setpoint = Setpoint1;
            break;
        case 1:
            setpoint = Setpoint2;
            break;
        default:
            return;
        }
        emit activeSetpoint(server_address, setpoint);
        break;
    case D_BAND:
        emit channel2Deadband(server_address, value_float32);
        break;
    case CB_LO:
        emit cutbackLow(server_address, value_float32);
        break;
    case CB_HI:
        emit cutbackHigh(server_address, value_float32);
        break;
    case R2G:
        emit relativeCoolCh2Gain(server_address, value_float32);
        break;
    case T_STAT:
        TimerStatus status;
        switch (value_uint8) {
        case 0:
            status = Reset;
            break;
        case 1:
            status = Run;
            break;
        case 2:
            status = Hold;
            break;
        case 3:
            status = End;
        default:
            return;
        }

        emit currentTimerStatus(server_address, status);
        break;
    case SP1:
        emit currentSetpointValue(server_address, Setpoint1, value_float32);
        break;
    case SP2:
        emit currentSetpointValue(server_address, Setpoint2, value_float32);
        break;
    case RM_SP:
        emit remoteSetpoint(server_address, value_float32);
        break;
    case LOC_T:
        emit localTrim(server_address, value_float32);
        break;
    case MR:
        emit manualReset(server_address, value_float32);
        break;
    case OP_HI:
        emit outputHighLimit(server_address, value_float32);
        break;
    case OP_LO:
        emit outputLowLimit(server_address, value_float32);
        break;
    case SAFE:
        emit safeOutputValueforSensorBreak(server_address, value_float32);
        break;
    case SP_RAT:
        emit setpointRateLimitValue(server_address, value_float32);
        break;
    case P_ERR:
        break;
    case A1_HYS:
        break;
    case A2_HYS:
        break;
    case A3_HYS:
        break;
    case A4_HYS:
        break;
    case STAT:
        for (int i = 0; i < 4; i++)
        {
            if (value_int16 & (1 << i))
            {
                emit alarmStatus(server_address, i, true);
            }
            else
            {
                emit alarmStatus(server_address, i, false);
            }
        }

        void (EurothermSerialClass::* foo[12])(const int, const bool);

        foo[0] = &EurothermSerialClass::autoManualStatus;
        foo[1] = &EurothermSerialClass::sensorBreakStatus;
        foo[2] = &EurothermSerialClass::loopBreakStatus;
        foo[3] = &EurothermSerialClass::CTLowLoadCurrentAlarmStatus;
        foo[4] = &EurothermSerialClass::CTHighLeakageCurrentAlarmStatus;
        foo[5] = &EurothermSerialClass::programEndStatus;
        foo[6] = &EurothermSerialClass::PVOverrangeStatus;
        foo[7] = &EurothermSerialClass::CTOvercurrentAlarmStatus;
        foo[8] = &EurothermSerialClass::newAlarmStatus;
        foo[9] = &EurothermSerialClass::timeRampRunningStatus;
        foo[10] = &EurothermSerialClass::remoteSPFailStatus;
        foo[11] = &EurothermSerialClass::autotuneStatus;

        for (int i = 4; i < 16; i++)
        {
            if (value_int16 & (1 << i))
            {
                emit (*this.*foo[i-4])(server_address, true);
            }
            else
            {
                emit (*this.*foo[i-4])(server_address, false);
            }
        }
        break;
    case LL_AMP:
        break;
    case LD_AMP:
        break;
    case A3:
        break;
    case A4:
        break;
    case HYST_H:
        break;
    case DI_IP:
        break;
    case HYST_C:
        break;
    case FILT_T:
        break;
    case HOME:
        break;
    case SP_HI:
        break;
    case SP_LO:
        break;
    case ADDR:
        break;
    case PV_OFS:
        break;
    case C_ADJ:
        break;
    case IM:
        break;
    case MV_IN:
        break;
    case PV_CM:
        break;
    case CJC_IN:
        break;
    case SBR:
        break;
    case NEW_AL:
        break;
    case LBR:
        break;
    case A_TUNE:
        break;
    case A_M:
        break;
    case AC_ALL:
        break;
    case L_R:
        break;
    case A1_STS:
        break;
    case A2_STS:
        break;
    case A3_STS:
        break;
    case A4_STS:
        break;
    case LD_ALM:
        break;
    case LK_ALM:
        break;
    case HC_ALM:
        break;
    case LOAD_A:
        break;
    case LEAK_A:
        break;
    case HILC_A:
        break;
    case REC_NO:
        break;
    case STORE:
        break;
    case TM_CFG:
        break;
    case TM_RES:
        break;
    case SS_SP:
        break;
    case SS_PWR:
        break;
    case DWELL:
        break;
    case T_ELAP:
        break;
    case T_REMN:
        break;
    case THRES:
        break;
    case END_T:
        break;
    case SERVO:
        break;
    case CTRL_H:
        break;
    case CTRL_C:
        break;
    case PB_UNIT:
        break;
    case LEV2_P:
        break;
    case UNITS:
        break;
    case LEV_3P:
        break;
    case CONF_P:
        break;
    case COLD:
        break;
    case COOL_T:
        break;
    case DEC_P:
        break;
    case STBY_T:
        break;
    case RAMP_UNITS:
        break;
    case METER:
        break;
    case UCAL:
        break;
    case A1_TYP:
        break;
    case A2_TYP:
        break;
    case A3_TYP:
        break;
    case A4_TYP:
        break;
    case A1_LAT:
        break;
    case A2_LAT:
        break;
    case A3_LAT:
        break;
    case A4_LAT:
        break;
    case A1_BLK:
        break;
    case A2_BLK:
        break;
    case A3_BLK:
        break;
    case A4_BLK:
        break;
    case DI_OP:
        break;
    case OFS_HI:
        break;
    case OFS_LO:
        break;
    case PNT_HI:
        break;
    case PNT_LO:
        break;
    case CT_RNG:
        break;
    case SB_TYP:
        break;
    case CUSTM_ID:
        break;
    case PHASE:
        break;
    case GO:
        break;
    case K_LOC:
        break;
    case DWEL_1:
        break;
    case TSP_1:
        break;
    case RMP_1:
        break;
    case DWEL_2:
        break;
    case TSP_2:
        break;
    case RMP_2:
        break;
    case DWEL_3:
        break;
    case TSP_3:
        break;
    case RMP_3:
        break;
    case DWEL_4:
        break;
    case TSP_4:
        break;
    case RMP_4:
        break;
    case IN_TYP:
        break;
    case CJ_TYP:
        break;
    case MV_HI:
        break;
    case MV_LO:
        break;
    case L_TYPE:
        break;
    case L_D_IN:
        break;
    case L_SENS:
        break;
    case L_TYPE_LB:
        break;
    case L_D_IN_LB:
        break;
    case L_SENS_LB:
        break;
    case ID:
        break;
    case BAUD:
        break;
    case PRTY:
        break;
    case DELAY:
        break;
    case RETRN:
        break;
    case REG_AD:
        break;
    case CT_ID:
        break;
    case CT_SRC:
        break;
    case CT_LAT:
        break;
    case ID_1:
        break;
    case D_IN_1:
        break;
    case FUNC_1:
        break;
    case RNG_1:
        break;
    case SRC_A_1:
        break;
    case SRC_B_1:
        break;
    case SRC_C_1:
        break;
    case SRC_D_1:
        break;
    case SENS_1:
        break;
    case PLS_1:
        break;
    case ID_2:
        break;
    case FUNC_2:
        break;
    case RNG_2:
        break;
    case SRC_A_2:
        break;
    case SRC_B_2:
        break;
    case SRC_C_2:
        break;
    case SRC_D_2:
        break;
    case SENS_2:
        break;
    case PLS_2:
        break;
    case ID_3:
        break;
    case FUNC_3:
        break;
    case RNG_3:
        break;
    case SRC_A_3:
        break;
    case SRC_B_3:
        break;
    case SRC_C_3:
        break;
    case SRC_D_3:
        break;
    case SENS_3:
        break;
    case PLS_3:
        break;
    case TYPE_4:
        break;
    case FUNC_4:
        break;
    case SRC_A_4:
        break;
    case SRC_B_4:
        break;
    case SRC_C_4:
        break;
    case SRC_D_4:
        break;
    case SENS_4:
        break;
    case PLS_4:
        break;
    default:
        break;
    }

    reply->deleteLater();
    reply = nullptr;
    delete request;
    request_queue.remove(0);

    if (!request_queue.length())
    {
        return;
    }

    request = static_cast<EurothermRequestStruct*>(request_queue.first());
    request->pending = false;
}

void EurothermSerialClass::processRequestQueue()
{
    EurothermRequestStruct *request =
            static_cast<EurothermRequestStruct*>(request_queue[0]);
    QModbusDataUnit::RegisterType reg_type;
    int start_address = request->start_address;
    int server_address = request->server_address;

    request->pending = true;

    switch (request->reg_type) {
    case Coil:
        reg_type = QModbusDataUnit::Coils;
        break;
    case DiscreteInput:
        reg_type = QModbusDataUnit::DiscreteInputs;
        break;
    case HoldingRegister:
        reg_type = QModbusDataUnit::HoldingRegisters;
        break;
    case InputRegister:
        reg_type = QModbusDataUnit::InputRegisters;
        break;
    default:
        return;
    }

    QModbusDataUnit data_unit;
    data_unit.setRegisterType(reg_type);
    data_unit.setStartAddress(start_address);
    data_unit.setValueCount(request->values.length());

    switch (request->req_type)
    {
    case ReadRequest:
        reply = modbus_client->sendReadRequest(data_unit,server_address);
        break;
    case WriteRequest:
        data_unit.setValues(request->values);
        reply = modbus_client->sendWriteRequest(data_unit,server_address);
        break;
    }
}
