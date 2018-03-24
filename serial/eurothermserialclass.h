#ifndef EUROTHERMSERIALCLASS_H
#define EUROTHERMSERIALCLASS_H

#include "baseserialclass.h"

class EurothermSerialClass : public BaseSerialClass
{
    Q_OBJECT
public:
    enum ControlAction {
        ReverseActing,
        DirectActing
    };

    enum Alarm {
        Alarm1,
        Alarm2,
        Alarm3,
        Alarm4
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

    bool processPending() const;

signals:
    void PVInputValue(const int server_address, const float value);
    void targetSetpoint(const int server_address, const float setpoint);
    void manualOutputValue(const int server_address, const float value);
    void workingOutput(const int server_address, const float value);
    void workingSetpoint(const int server_address, const float setpoint);
    void proportionalBand(const int server_address, const float prop_band);
    void controlAction(const int server_address, const ControlAction control_action);
    void integralTime(const int server_address, const float int_time);
    void derivativeTime(const int server_address, const float dev_time);
    void inputRangeLowLimit(const int server_address, const float low_range);
    void inputRangeHighLimit(const int server_address, const float high_range);
    void alarmThreshold(const int server_address, const int alarm_num, const float a1);
    void activeSetpoint(const int server_address, const Setpoint setpoint);
    void channel2Deadband(const int server_address, const float d_band);
    void cutbackLow(const int server_address, const float cb_lo);
    void cutbackHigh(const int server_address, const float cb_hi);
    void relativeCoolCh2Gain(const int server_address, const float r2g);
    void currentTimerStatus(const int server_address, const TimerStatus t_stat);
    void currentSetpointValue(const int server_address, const Setpoint sp_sel, const float sp);
    void remoteSetpoint(const int server_address, const float rm_sp);
    void localTrim(const int server_address, const float rm_sp);
    void manualReset(const int server_address, const float mr);
    void outputHighLimit(const int server_address, const float op_hi);
    void outputLowLimit(const int server_address, const float op_low);
    void safeOutputValueforSensorBreak(const int server_address, const float safe);
    void setpointRateLimitValue(const int server_address, const float sp_rat);
    void calculatedError(const int server_address, const float p_err);
    void alarmHysteresis(const int server_address, const int alarm_num, const float a_hys);
    void alarmStatus(const int server_address, const int alarm_num, const bool status);     // status == true -> active
    void autoManualStatus(const int server_address, const bool status);                     // status == true -> active
    void sensorBreakStatus(const int server_address, const bool status);                    // status == true -> active
    void loopBreakStatus(const int server_address, const bool status);                      // status == true -> active
    void CTLowLoadCurrentAlarmStatus(const int server_address, const bool status);          // status == true -> active
    void CTHighLeakageCurrentAlarmStatus(const int server_address, const bool status);      // status == true -> active
    void programEndStatus(const int server_address, const bool status);                     // stauts == true -> active
    void PVOverrangeStatus(const int server_address, const bool status);                    // status == true -> active
    void CTOvercurrentAlarmStatus(const int server_address, const bool status);             // status == true -> active
    void newAlarmStatus(const int server_address, const bool status);                       // status == true -> active
    void timeRampRunningStatus(const int server_address, const bool status);                // status == true -> active
    void remoteSPFailStatus(const int server_address, const bool status);                   // status == true -> active
    void autotuneStatus(const int server_address, const bool status);                       // status == true -> active
};

#endif // EUROTHERMSERIALCLASS_H
