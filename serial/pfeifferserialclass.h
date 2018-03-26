#ifndef PFEIFFERSERIALCLASS_H
#define PFEIFFERSERIALCLASS_H

#include "baseserialclass.h"

class PfeifferSerialclass : public BaseSerialClass
{
    Q_OBJECT
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

    enum Relay {
        Relay1,
        Relay2,
        Relay3,
        Relay4,
        Relay5,
        Relay6
    };

public:
    PfeifferSerialclass(QObject *parent = nullptr);
    ~PfeifferSerialclass();

signals:
    void sensorStatus(const PfeifferSerialclass::Sensor sensor,
                      const PfeifferSerialclass::SensorStatus status);
    void sensorControl(const PfeifferSerialclass::Sensor sensor,
                       const PfeifferSerialclass::ControllingSource switch_on,
                       const PfeifferSerialclass::ControllingSource switch_off,
                       float switch_on_value, float switch_off_value);
    void sensorPressureAndStautus(
            const PfeifferSerialclass::Sensor sensor,
            const PfeifferSerialclass::PressureMeasurementStatus staus,
            float pressure);
    void decimalDigits(const PfeifferSerialclass::DecimalDigits digits);
    void measurementPointName(
            const PfeifferSerialclass::MeasurementPoint measure_point,
            const QString &name);
    void unitsOfMeasurement(
            const PfeifferSerialclass::PfeifferSerialclass::Units units);
    void baragraphMode(const PfeifferSerialclass::BaragraphMode bar_mode);
    void displayContrast(const int contrast);
    void screenSave(int interval);
    void thresholdValueSetting(const PfeifferSerialclass::Relay relay,
                               const PfeifferSerialclass::Sensor sensor,
                               const float lower_threshold,
                               const float upper_threshold);

public slots:
    void processRequestQueue();

    void requestReadSensorStatuses();
    void requestReadSensorControl(const PfeifferSerialclass::Sensor sensor);
    void requestReadStatusAndPressure(const PfeifferSerialclass::Sensor sensor);
    void requestReadDecimalDigits();
    void requestReadMeasurementPointNames();
    void requestReadUnitsOfMeasurement();
    void requestReadBaragraph();
    void requestReadDisplayContrast();
    void requestReadScreenSave();
    void requestReadThresholdValueSetting(const Relay relay);

    void requestWriteSensorStatus(
            const PfeifferSerialclass::Sensor sensor,
            const PfeifferSerialclass::SensorStatus status);
    void requestWriteSensorControl(
            const PfeifferSerialclass::Sensor sensor,
            const PfeifferSerialclass::ControllingSource switch_on,
            const PfeifferSerialclass::ControllingSource switch_off,
            const float switch_on_value, const float switch_off_value);
    void requestWriteDecimalDigits(
            const PfeifferSerialclass::DecimalDigits digits);
    void requestWriteMeasurementPointNames(const QString names[6]);
    void requestWriteUnitsOfMeasurement(const Units units);
    void requestWriteBaragraph(PfeifferSerialclass::BaragraphMode bar_mode);
    void requestWriteDisplayContrast(const int contrast); // (0->min) (20->max)
    void requestWriteScreenSave(const int interval); // in hours
    void requestWriteThresholdValueSetting(
            const PfeifferSerialclass::Relay relay,
            const PfeifferSerialclass::Sensor sensor,
            const float lower_threshold, const float upper_threshold);

    void manageReply();
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
    bool manageScreenSaveReply();
    bool manageThresholdValueSettingReply();
};

#endif // PFEIFFERSERIALCLASS_H
