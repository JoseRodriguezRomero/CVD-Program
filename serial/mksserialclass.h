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

    enum Key {
        Off,
        On,
        Esc,
        Enter,
        Right,
        Left,
        Up,
        Down,
        NoKey,
        Number
    };

    enum DisplayDialog {
        ConfigurableDisplay0,
        ConfigurableDisplay1,
        ConfigurableDisplay2,
        UserDisplay,
        StatusDisplay,
        AutoTriggerAndSwitches,
        SetupMode,
        SetupDisplay1And2,
        SetupDisplay3And4,
        SetpointSetup,
        RangeSetup,
        Gain1,
        Gain2,
        RTDOffset,
        Channel1IOScale,
        Channl2IOScale
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

    void requestReadDisplayText();
    void requestReadLastKey();
    void requestReadDisplayDialog();

    void requestWriteDisplayText(const QString &text);
    void requestWriteDisplayDialog(const DisplayDialog dialog);

signals:
    void displayText(const QString text);
    void lastKey(const Key key);

private:
    void manageErrorReply();
};

#endif // MKSSERIALCLASS_H
