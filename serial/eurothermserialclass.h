#ifndef EUROTHERMSERIALCLASS_H
#define EUROTHERMSERIALCLASS_H

#include <QVariant>
#include <QSerialPort>
#include <QModbusReply>
#include <QModbusClient>
#include <QModbusDataUnit>
#include <QModbusRtuSerialMaster>

#include "baseserialclass.h"

class EurothermSerialClass : public BaseSerialClass
{
    Q_OBJECT
private:
    QModbusRtuSerialMaster *modbus_client;
    QModbusReply *reply;

public:
    enum ControlAction {
        ReverseActing,
        DirectActing
    };

    enum BaudRate {
        Baud9600        = 0,
        Baud19200       = 1,
        Baud4800        = 2,
        Baud2400        = 3,
        Baud1200        = 4
    };

    enum Alarm {
        Alarm1,
        Alarm2,
        Alarm3,
        Alarm4
    };

    enum Parity {
        NoParity        = 0,
        EvenParity      = 1,
        OddParity       = 2
    };

    enum Setpoint {
        Setpoint1,
        Setpoint2
    };

    enum TimerStatus {
        Reset,
        Run,
        Hold,
        End
    };

public:
    explicit EurothermSerialClass(QObject *parent = 0);
    ~EurothermSerialClass();

    bool deviceConnected() const;
    bool deviceDisconnected() const;

public slots:
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
    void requestReadAlarmThreshold(const int server_address, const Alarm alarm);
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
    void requestReadAlarmHysteresis(const int server_address, const Alarm alarm);
    void resquestReadInstrumentStatus(const int server_address);

    void requestWriteTargetSetpoint(const int server_address, const float setpoint);        // Do not use this function continuously!!
    void requestWriteProportionalBand(const int server_address, const float pb);
    void requestWriteIntegralTime(const int server_address, const float ti);
    void requestWriteDerivativeTime(const int server_address, const float td);
    void requestWriteAlarmThreshold(const int server_address, const Alarm alarm, const float a);
    void requestWriteActiveSetpoint(const int server_address, const Setpoint setpoint);
    void requestWriteChannel2Deadband(const int server_address, const float d_band);
    void requestWriteCutbackLow(const int server_address, const float cb_lo);
    void requestWriteCutbackHigh(const int server_address, const float cb_hi);
    void requestWriteRelativeCoolCh2Gain(const int server_address, const float r2g);
    void requestWriteTimerStatus(const int server_address, const TimerStatus status);
    void requestWriteSetpoint(const int server_address, const int sp_num, const float sp);  // Do not use this function continuously!!
    void requestWriteRemoteSetpoint(const int server_address, const float rm_sp);
    void requestWriteLocalTrim(const int server_address, const float loc_t);
    void requestWriteManualReset(const int server_address, const float mr);
    void requestWriteOutputHighLimit(const int server_address, const float op_hi);
    void requestWriteOutputLowLimit(const int server_address, const float op_lo);
    void requestWriteSafeOutputValueforSensorBreak(const int server_address, const float safe);
    void requestWriteSetpointRateLimitValue(const int server_address, const float sp_rat);
    void requestWriteAlarmHysteresis(const int server_address, const Alarm alarm, const float a_hys);

    void processRequestQueue();
    void manageReply();

    void connectDevice();
    void disconnectDevice();

    bool checkState();
    bool processPending() const;

signals:
    void PVInputValue(const int server_address, const float value);
    void TargetSetpoint(const int server_address, const float setpoint);
    void ManualOutputValue(const int server_address, const float value);
    void WorkingOutput(const int server_address, const float value);
    void WorkingSetpoint(const int server_address, const float setpoint);
    void ProportionalBand(const int server_address, const float prop_band);
    void CurrentControlAction(const int server_address, const ControlAction control_action);
    void IntegralTime(const int server_address, const float int_time);
    void DerivativeTime(const int server_address, const float dev_time);
    void InputRangeLowLimit(const int server_address, const float low_range);
    void InputRangeHighLimit(const int server_address, const float high_range);
    void AlarmThreshold(const int server_address, const int alarm_num, const float a1);
    void ActiveSetpoint(const int server_address, const Setpoint setpoint);
    void Channel2Deadband(const int server_address, const float d_band);
    void CutbackLow(const int server_address, const float cb_lo);
    void CutbackHigh(const int server_address, const float cb_hi);
    void RelativeCoolCh2Gain(const int server_address, const float r2g);
    void CurrentTimerStatus(const int server_address, const TimerStatus t_stat);
    void CurrentSetpointValue(const int server_address, const Setpoint sp_sel, const float sp);
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
};

#endif // EUROTHERMSERIALCLASS_H
