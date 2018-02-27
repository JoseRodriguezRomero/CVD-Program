#ifndef MKSSERIALCLASS_H
#define MKSSERIALCLASS_H

#include <QSerialPort>

#include "baseserialclass.h"

class MKSSerialClass : public BaseSerialClass
{
    Q_OBJECT
private:
    QSerialPort *serial_port;
    QByteArray buffer;

public:
    explicit MKSSerialClass(QObject *parent = nullptr);
    ~MKSSerialClass();

signals:

public slots:
    bool processPending() const;

    void manageReply();
    void processRequestQueue();

    void connectDevice();
    void disconnectDevice();

    bool checkState();
};

#endif // MKSSERIALCLASS_H
