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

    enum DecimalDigits {
        TwoDigits,
        ThreeDigits
    };

    enum MeasurementPoint {
        MeasurePoint1,
        MeasurePoint2,
        MeasurePoint3,
        MeasurePoint4,
        MeasurePoint5,
        MeasurePoint6
    };

    enum Units {
        mBar,
        Torr,
        Pascal
    };

    enum BaragraphMode {
        BaragraphOff,
        MeasurementRange,
        OneDecade
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
    void decimalDigits(const DecimalDigits digits);
    void measurementPointName(const MeasurementPoint measure_point,
                              const QString &name);
    void unitsOfMeasurement(const Units units);
    void baragraphMode(BaragraphMode bar_mode);
    void displayContrast(const int contrast);

public slots:
    void processRequestQueue();

    void requestReadSensorStatuses();
    void requestReadSensorControl(const Sensor sensor);
    void requestReadStatusAndPressure(const Sensor sensor);
    void requestReadDecimalDigits();
    void requestReadMeasurementPointNames();
    void requestReadUnitsOfMeasurement();
    void requestReadBaragraph();
    void requestReadDisplayContrast();

    void requestWriteSensorStatus(const Sensor sensor, const SensorStatus status);
    void requestWriteSensorControl(const Sensor sensor,
                                   const ControllingSource switch_on,
                                   const ControllingSource switch_off,
                                   const float switch_on_value,
                                   const float switch_off_value);
    void requestWriteDecimalDigits(const DecimalDigits digits);
    void requestWriteMeasurementPointNames(const QString names[6]);
    void requestWriteUnitsOfMeasurement(const Units units);
    void requestWriteBaragraph(BaragraphMode bar_mode);
    void requestWriteDisplayContrast(const int contrast); // (0->min) (20->max)

    bool checkState();
    void manageReply();

    void connectDevice();
    void disconnectDevice();

    bool processPending() const;

private:
    bool manageSensorStatusReply();
    bool manageSensorControlReply();
    bool manageStatusAndPressureReply();
    bool manageDecimalDigitsReply();
    bool manageMeasurementPointNamesReply();
    bool manageUnitsOfMeasurementReply();
    bool manageBaragraphModeReply();
    bool manageReplyDisplayContrast();
};

#endif // PFEIFFERSERIALCLASS_H
