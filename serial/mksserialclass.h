#ifndef MKSSERIALCLASS_H
#define MKSSERIALCLASS_H

#include <QSerialPort>

#include "baseserialclass.h"

class MKSSerialClass : public BaseSerialClass
{
    Q_OBJECT
public:
    enum Channel {
        Channel1    = 1,
        Channel2    = 2,
        NoChannel   = 0
    };

    enum ErrorMessage {
        CommunicationError          = 0,
        ADCOverflowOrUnderflow      = 1,
        RangeError                  = 2,
        OffsetError                 = 1
    };

    enum Key {
        OffKey      = 7,
        OnKey       = 8,
        EscKey      = 9,
        EnterKey    = 10,
        RightKey    = 11,
        LeftKey     = 12,
        UpKey       = 13,
        DownKey     = 14,
        NoKey       = 255
    };

    enum DisplayDialog {
        ConfigurableDisplay0        = 0,
        ConfigurableDisplay1        = 1,
        ConfigurableDisplay2        = 2,
        UserDisplay                 = 3,
        StatusDisplay               = 4,
        AutoTriggerAndSwitches      = 5,
        SetupMode                   = 6,
        SetupDisplay1And2           = 7,
        SetupDisplay3And4           = 8,
        SetpointSetup               = 9,
        RangeSetup                  = 10,
        Gain1                       = 11,
        Gain2                       = 12,
        RTDOffset                   = 13,
        Channel1IOScale             = 14,
        Channel2IOScale             = 15,
        SignalProcessingMode1       = 16,
        SignalProcessingMode2       = 17,
        Linearization1              = 18,
        Linearization2              = 19,
        LimitMode1                  = 20,
        LimitDefinitions1           = 21,
        LimitMode2                  = 22,
        LimitDefinitions2           = 23,
        RelayFormulas               = 24,
        TemporaryFormulas1And2      = 25,
        TemporaryFormulas3And4      = 26,
        InterfaceSetup1             = 27,
        InterfaceSetup2             = 28,
        ResetToDefault              = 29
    };

    enum RemoteMode {
        RemoteModeOn,
        RemoteModeOff
    };

    enum Status {
        CommuncationErrorStatus     = 0,
        UnderrangeAIN0Status        = 1,
        OverrangeAIN0Status         = 2,
        UnderrangeAIN1Status        = 3,
        OverrangeAIN1Status         = 4,
        Relay0Status                = 5,
        Relay1Status                = 6,
        ParamChangeStatus           = 7,
        NoError                     = 8
    };

public:
    explicit MKSSerialClass(QObject *parent = nullptr);
    ~MKSSerialClass();    

public slots:
    bool processPending() const;

    void manageReply();
    void processRequestQueue();

    void requestReadDisplayText();
    void requestReadLastKey();
    void requestReadDisplayDialog();
    void requestReadIDString();
    void requestReadRemoteMode();
    void requestReadAccessChannel(const MKSSerialClass::Channel channel);
    void requestReadActualValue(const MKSSerialClass::Channel channel);
    void requestReadSetpoint(const MKSSerialClass::Channel channel);
    void requestReadExternalInput(const MKSSerialClass::Channel channel);
    void requestReadStatus();
    void requestReadValve(const MKSSerialClass::Channel channel);

    void requestWriteDisplayText(const QString &text);
    void requestWriteDisplayDialog(const MKSSerialClass::DisplayDialog dialog);
    void requestWriteRemoteMode(const MKSSerialClass::RemoteMode mode);
    void requestWriteAccessChannel(const MKSSerialClass::Channel channel,
                                   float setpoint);
    void requestWriteAccessChannel(const MKSSerialClass::Channel channel,
                                   float setpoint, const bool valve_open);
    void requestWriteActualValue(const MKSSerialClass::Channel channel,
                                 float setpoint);
    void requestWriteSetpoint(const MKSSerialClass::Channel channel,
                              float setpoint);
    void requestWriteValve(const MKSSerialClass::Channel channel,
                           const bool valve_open);

signals:
    void errorMessage(const ErrorMessage error_message);

    void displayText(const QString text);
    void lastKey(const MKSSerialClass::Key key, const int polls);
    void displayDialog(const MKSSerialClass::DisplayDialog dialog);
    void IDString(const QString version, const QString release,
                  const QString serial_number);
    void remoteMode(const MKSSerialClass::RemoteMode mode);
    void actualValue(const MKSSerialClass::Channel channel, const float value);
    void setpoint(const MKSSerialClass::Channel channel, const float setpoint);
    void valve(const MKSSerialClass::Channel channel, const bool valve_open);
    void externalInput(const MKSSerialClass::Channel channel, const float value);
    void status(const Status status);

private:
    Channel channelQueryed() const;

    bool manageErrorReply();

    bool manageRequestKeyReply(const QVector<QString> &args);
    bool manageDisplayDialogReply(const QVector<QString> &args);
    bool manageIDStringReply(const QVector<QString> &args);
    bool manageRemoteModeReply(const QVector<QString> &args);
    bool manageAccessChannelReply(const QVector<QString> &args);
    bool manageActualValueReply(const QVector<QString> &args);
    bool manageSetpointReply(const QVector<QString> &args);
    bool manageExternalInputReply(const QVector<QString> &args);
    bool manageStatusReply(const QVector<QString> &args);
    bool manageValvesReply(const QVector<QString> &args);
};

#endif // MKSSERIALCLASS_H
