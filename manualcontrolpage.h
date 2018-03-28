#ifndef MANUALCONTROLPAGE_H
#define MANUALCONTROLPAGE_H

#include <QWidget>
#include <QFrame>
#include <QLabel>
#include <QGroupBox>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QGridLayout>
#include <QLineEdit>
#include <QDoubleSpinBox>

#include "serial/eurothermserialclass.h"
#include "serial/pfeifferserialclass.h"
#include "serial/mksserialclass.h"

class ManualControlPage : public QWidget
{
    Q_OBJECT
private:
    QLineEdit measured_temp_left;
    QDoubleSpinBox temp_setpoint_left;
    QDoubleSpinBox temp_ramp_left;

    QLineEdit measured_temp_center;
    QDoubleSpinBox temp_setpoint_center;
    QDoubleSpinBox temp_ramp_center;

    QLineEdit measured_temp_right;
    QDoubleSpinBox temp_setpoint_right;
    QDoubleSpinBox temp_ramp_right;

    QLineEdit mfc_flow;
    QLineEdit mfc_status;
    QDoubleSpinBox mfc_setpoint;

    QLineEdit gauge_pressure;
    QLineEdit gauge_sensor;
    QLineEdit sensor_status;

    EurothermSerialClass *eurotherm_serial;
    PfeifferSerialclass *pfeiffer_serial;
    MKSSerialClass *mks_serial;

public:
    explicit ManualControlPage(QWidget *parent = 0);
    void setEurothermSerialClasss(EurothermSerialClass *eurotherm_serial);
    void setPfeifferSerialClass(PfeifferSerialclass *pfeiffer_serial);
    void setMKSSerialClass(MKSSerialClass *mks_serial);

signals:
    void writeEurothermTemperatureSetpoint(const int server_address,
                                           const float setpoint);
    void writeEurothermTemperatureRamp(const int server_address,
                                       const float ramp);

    void writeMFCSetpoint(const MKSSerialClass::Channel channel,
                          const float setpoint);

public slots:
    void setMeasuredTemperature(const int server_address,
                                const float temperature);
    void setTemperatureSetpoint(const int server_address,
                                const float setpoint);
    void setTemperatureRamp(const int server_address,
                            const float ramp);

    void setPfeifferPressure(
            PfeifferSerialclass::Sensor sensor,
            PfeifferSerialclass::PressureMeasurementStatus status,
            const float pressure);

    void setMeasuredMassFlow(const MKSSerialClass::Channel channel,
                             const float actual_value);
    void setMassFlowSetpoint(const MKSSerialClass::Channel channel,
                             const float setpoint);

    void setBlockedCommands(bool block);
    void setUnBlockedCommands(bool unblock);

    void requestSetTemperatureSetpoint();
    void requestSetTemperatureRamp();

    void requestSetMassFlowSetpoint();
};

#endif // MANUALCONTROLPAGE_H
