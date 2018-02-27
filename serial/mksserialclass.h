#ifndef MKSSERIALCLASS_H
#define MKSSERIALCLASS_H

#include <QSerialPort>

#include "baseserialclass.h"

class MKSSerialClass : public BaseSerialClass
{
    Q_OBJECT
public:
    enum ErrorMessage {
        CommunicationError,
        ADCOverflowOrUnderflow,
        RangeError,
        OffsetError
    };

private:
    QSerialPort *serial_port;
    QByteArray buffer;

public:
    explicit MKSSerialClass(QObject *parent = nullptr);
    ~MKSSerialClass();

    bool deviceConnected() const;
    bool deviceDisconnected() const;

signals:
    void deviceConnected(QSerialPort::SerialPortError);
    void errorMessage(const ErrorMessage error_message);

public slots:
    bool processPending() const;

    void manageReply();
    void processRequestQueue();

    void connectDevice();
    void disconnectDevice();

    bool checkState();

private:
    void manageErrorReply();
};

#endif // MKSSERIALCLASS_H
