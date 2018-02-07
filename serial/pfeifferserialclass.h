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
        Sensor1 = 0,
        Sensor2 = 1,
        Sensor3 = 2,
        Sensor4 = 3,
        Sensor5 = 4,
        Sensor6 = 5
    };

    enum ControllingSource {
        Sensor1 =           0,
        Sensor2 =           1,
        Sensor3 =           2,
        Sensor4 =           3,
        Sensor5 =           4,
        Sensor6 =           5,
        ExternalControl =   6,
        Manual =            7,
        HotStart =          8
    };

public:
    PfeifferSerialclass(QObject *parent);
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

public slots:
    void processSerialRequestQueue();

    void setPortName(const QString &port_name);
    void setBaudRate(const QSerialPort::BaudRate baud_rate);

    void requestReadSensorStatuses();
    void requestReadSensorControl(Sensor sensor);

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
};

#endif // PFEIFFERSERIALCLASS_H
