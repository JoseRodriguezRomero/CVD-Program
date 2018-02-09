#ifndef PFEIFFERSERIALCLASS_H
#define PFEIFFERSERIALCLASS_H

#include <QTimer>
#include <QVector>
#include <QObject>
#include <QSerialPort>

struct PfeifferSerialStruct;

class PfeifferSerialclass : public QObject
{
    Q_OBJECT
private:
    PfeifferSerialStruct *private_struct;

    QSerialPort *serial_port;
    QTimer reconnect_timer;
    QTimer event_timer;

    QString port_name;
    QSerialPort::BaudRate baud_rate;

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

    QString PortName() const;
    QSerialPort::FlowControl FlowControl() const;
    QSerialPort::BaudRate BaudRate() const;
    QSerialPort::StopBits StopBits() const;
    QSerialPort::DataBits DataBits() const;
    QSerialPort::Parity Parity() const;

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
    void processSerialRequestQueue();

    void setPortName(const QString &port_name);
    void setBaudRate(const QSerialPort::BaudRate baud_rate);

    void requestReadSensorStatuses();
    void requestReadSensorControl(Sensor sensor);
    void requestReadStatusAndPressure(Sensor sensor);

    void requestWriteSensorStatus(int sensor_num, SensorStatus status);
    void requestWriteSensorControl(Sensor sensor, ControllingSource switch_on,
                                   ControllingSource switch_off,
                                   float switch_on_value,
                                   float switch_off_value);

    bool checkState();

    void connectDevice();
    void disconnectDevice();

private slots:
    void ManageReply();

private:
    bool manageSensorStatusReply();
    bool manageSensorControlReply();
    bool manageStatusAndPressureReply();
};

#endif // PFEIFFERSERIALCLASS_H
