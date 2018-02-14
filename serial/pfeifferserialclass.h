#ifndef PFEIFFERSERIALCLASS_H
#define PFEIFFERSERIALCLASS_H

#include <QTimer>
#include <QVector>
#include <QObject>
#include <QSerialPort>

#include "baseserialclass.h"

class PfeifferSerialclass : public BaseSerialClass
{
    Q_OBJECT
private:
    QSerialPort *serial_port;
    QByteArray buffer;

public:
    enum SensorStatus {
        On,
        Off
    };

    enum Sensor {
        Sensor1,
        Sensor2,
        Sensor3,
        Sensor4,
        Sensor5,
        Sensor6
    };

    enum ControllingSource {
        Sensor1Control,
        Sensor2Control,
        Sensor3Control,
        Sensor4Control,
        Sensor5Control,
        Sensor6Control,
        ExternalControl,
        ManualControl,
        HotStart
    };

    enum PressureMeasurementStatus {
        MeasurementDataOkay,
        Underrange,
        Overrange,
        SensorError,
        SensorOff,
        NoSensor,
        IdentificationError
    };

public:
    PfeifferSerialclass(QObject *parent = 0);
    ~PfeifferSerialclass();

    bool deviceConnected() const;
    bool deviceDisconnected() const;

signals:
    void ErrorString(const QString &error_string, bool status);
    void deviceConnected(QSerialPort::SerialPortError);

    void sensorStatus(Sensor sensor, SensorStatus status);
    void sensorControl(Sensor sensor, ControllingSource switch_on,
                       ControllingSource switch_off, float switch_on_value,
                       float switch_off_value);
    void sensorPressureAndStautus(Sensor sensor,
                                  PressureMeasurementStatus staus,
                                  float pressure);

public slots:
    void processRequestQueue();

    void requestReadSensorStatuses();
    void requestReadSensorControl(Sensor sensor);
    void requestReadStatusAndPressure(Sensor sensor);

    void requestWriteSensorStatus(int sensor_num, SensorStatus status);
    void requestWriteSensorControl(Sensor sensor, ControllingSource switch_on,
                                   ControllingSource switch_off,
                                   float switch_on_value,
                                   float switch_off_value);

    bool checkState();
    void manageReply();

    void connectDevice();
    void disconnectDevice();



    bool processPending() const;

private:
    bool manageSensorStatusReply();
    bool manageSensorControlReply();
    bool manageStatusAndPressureReply();
};

#endif // PFEIFFERSERIALCLASS_H
