#ifndef EUROTHERMSERIALCLASS_H
#define EUROTHERMSERIALCLASS_H

#include <QTimer>
#include <QObject>
#include <QVariant>
#include <QSerialPort>
#include <QModbusReply>
#include <QModbusClient>
#include <QModbusDataUnit>
#include <QModbusRtuSerialMaster>

struct EurothermSerialStruct;

class EurothermSerialClass : public QObject
{
    Q_OBJECT
private:
    EurothermSerialStruct *private_struct;
    QTimer reconnect_timer;
    QTimer event_timer;

public:
    enum ControlActions {
        ReverseActing   = 0,
        DirectActing    = 1
    };

    enum BaudRates {
        Baud9600        = 0,
        Baud19200       = 1,
        Baud4800        = 2,
        Baud2400        = 3,
        Baud1200        = 4
    };

    enum Parities {
        NoParity        = 0,
        EvenParity      = 1,
        OddParity       = 2
    };

    enum Setpoints {
        SP1             = 0,
        SP2             = 1
    };

    enum TimerStatuses {
        Reset           = 0,
        Run             = 1,
        Hold            = 2,
        End             = 3
    };

private:
    QModbusRtuSerialMaster *modbus_client;

    QString serial_port_name;
    QSerialPort::BaudRate baud_rate;
    QSerialPort::Parity parity;
    QSerialPort::StopBits stop_bits;
    QSerialPort::DataBits data_bits;

public:
    explicit EurothermSerialClass(QObject *parent = 0);
    ~EurothermSerialClass();

    QString SerialPortName() const;
    QSerialPort::BaudRate BaudRate() const;
    QSerialPort::Parity Parity() const;
    QSerialPort::StopBits StopBits() const;
    QSerialPort::DataBits DataBits() const;

signals:
    void ErrorString(const QString &error_string, bool status);

public slots:
    void setSerialPortName(const QString port_name);
    void setBaudRate(const QSerialPort::BaudRate baud_rate);
    void setParity(const QSerialPort::Parity parity);
    void setStopBits(const QSerialPort::StopBits stop_bits);
    void setDataBits(const QSerialPort::DataBits data_bits);

    void requestReadPVInputValue(const int server_address);
    void requestReadTargetSetpoint(const int server_address);
    void requestReadManualOutputValue(const int server_address);
    void requestReadWorkingOutput(const int server_address);
    void requestReadWorkingSetpoint(const int server_address);
    void requestReadProportionalBand(const int server_address);
    void requestReadControlAction(const int server_address);
    void requestReadIntegralTime(const int server_address);
    void requestReadDerivativeTime(const int server_address);
    void requestReadInputRangeLowLimit(const int server_address);
    void requestReadInputRangeHighLimit(const int server_address);
    void requestReadAlarmThreshold(const int server_address, const int alarm_num);
    void requestReadActiveSetpoint(const int server_address);
    void requestReadChannel2Deadband(const int server_address);
    void requestReadCutbackLow(const int server_address);
    void requestReadCutbackHigh(const int server_address);
    void requestReadRelativeCoolCh2Gain(const int server_address);
    void requestReadTimerStatus(const int server_address);
    void requestReadSetpoint(const int server_address, int sp_num);
    void requestReadRemoteSetpoint(const int server_address);
    void requestReadLocalTrim(const int server_address);
    void requestReadManualReset(const int server_address);
    void requestReadOutputHighLimit(const int server_address);
    void requestReadOutputLowLimit(const int server_address);
    void requestReadSafeOutputValueforSensorBreak(const int server_address);
    void requestReadSetpointRateLimitValue(const int server_address);
    void requestReadCalculatedError(const int server_address);
    void requestReadAlarmHysteresis(const int server_address, int alarm_num);
    void resquestReadInstrumentStatus(const int server_address);

    void requestWriteTargetSetpoint(const int server_address, const float setpoint);        // Do not use this function continuously!!
    void requestWriteProportionalBand(const int server_address, const float pb);
    void requestWriteIntegralTime(const int server_address, const float ti);
    void requestWriteDerivativeTime(const int server_address, const float td);
    void requestWriteAlarmThreshold(const int server_address, const int alarm_num, const float a);
    void requestWriteActiveSetpoint(const int server_address, const Setpoints sp_sel);
    void requestWriteChannel2Deadband(const int server_address, const float d_band);
    void requestWriteCutbackLow(const int server_address, const float cb_lo);
    void requestWriteCutbackHigh(const int server_address, const float cb_hi);
    void requestWriteRelativeCoolCh2Gain(const int server_address, const float r2g);
    void requestWriteTimerStatus(const int server_address, const TimerStatuses t_stat);
    void requestWriteSetpoint(const int server_address, const int sp_num, const float sp);  // Do not use this function continuously!!
    void requestWriteRemoteSetpoint(const int server_address, const float rm_sp);
    void requestWriteLocalTrim(const int server_address, const float loc_t);
    void requestWriteManualReset(const int server_address, const float mr);
    void requestWriteOutputHighLimit(const int server_address, const float op_hi);
    void requestWriteOutputLowLimit(const int server_address, const float op_lo);
    void requestWriteSafeOutputValueforSensorBreak(const int server_address, const float safe);
    void requestWriteSetpointRateLimitValue(const int server_address, const float sp_rat);
    void requestWriteAlarmHysteresis(const int server_address, int alarm_num, const float a_hys);

    void processModbusRequestQueue();

    void connectDevice();
    void disconnectDevice();

    bool checkState();

signals:
    void PVInputValue(const int server_address, const float value);
    void TargetSetpoint(const int server_address, const float setpoint);
    void ManualOutputValue(const int server_address, const float value);
    void WorkingOutput(const int server_address, const float value);
    void WorkingSetpoint(const int server_address, const float setpoint);
    void ProportionalBand(const int server_address, const float prop_band);
    void ControlAction(const int server_address, const ControlActions control_action);
    void IntegralTime(const int server_address, const float int_time);
    void DerivativeTime(const int server_address, const float dev_time);
    void InputRangeLowLimit(const int server_address, const float low_range);
    void InputRangeHighLimit(const int server_address, const float high_range);
    void AlarmThreshold(const int server_address, const int alarm_num, const float a1);
    void ActiveSetpoint(const int server_address, const Setpoints sp_sel);
    void Channel2Deadband(const int server_address, const float d_band);
    void CutbackLow(const int server_address, const float cb_lo);
    void CutbackHigh(const int server_address, const float cb_hi);
    void RelativeCoolCh2Gain(const int server_address, const float r2g);
    void TimerStatus(const int server_address, const TimerStatuses t_stat);
    void Setpoint(const int server_address, const int sp_num, const float sp);
    void RemoteSetpoint(const int server_address, const float rm_sp);
    void LocalTrim(const int server_address, const float rm_sp);
    void ManualReset(const int server_address, const float mr);
    void OutputHighLimit(const int server_address, const float op_hi);
    void OutputLowLimit(const int server_address, const float op_low);
    void SafeOutputValueforSensorBreak(const int server_address, const float safe);
    void SetpointRateLimitValue(const int server_address, const float sp_rat);
    void CalculatedError(const int server_address, const float p_err);
    void AlarmHysteresis(const int server_address, const int alarm_num, const float a_hys);
    void AlarmStatus(const int server_address, const int alarm_num, const bool status);     // status == true -> active
    void AutoManualStatus(const int server_address, const bool status);                     // status == true -> active
    void SensorBreakStatus(const int server_address, const bool status);                    // status == true -> active
    void LoopBreakStatus(const int server_address, const bool status);                      // status == true -> active
    void CTLowLoadCurrentAlarmStatus(const int server_address, const bool status);          // status == true -> active
    void CTHighLeakageCurrentAlarmStatus(const int server_address, const bool status);      // status == true -> active
    void ProgramEndStatus(const int server_address, const bool status);                     // stauts == true -> active
    void PVOverrangeStatus(const int server_address, const bool status);                    // status == true -> active
    void CTOvercurrentAlarmStatus(const int server_address, const bool status);             // status == true -> active
    void NewAlarmStatus(const int server_address, const bool status);                       // status == true -> active
    void TimeRampRunningStatus(const int server_address, const bool status);                // status == true -> active
    void RemoteSPFailStatus(const int server_address, const bool status);                   // status == true -> active
    void AutotuneStatus(const int server_address, const bool status);                       // status == true -> active

    void deviceConnected(QModbusDevice::State);

private slots:
    void ManageReply();
};

#endif // EUROTHERMSERIALCLASS_H
