#ifndef STATUSSTRING_H
#define STATUSSTRING_H

#include <QFrame>
#include <QLabel>
#include <QWidget>
#include <QSerialPort>
#include <QHBoxLayout>

class StatusString : public QFrame
{
    Q_OBJECT
private:
    QString name;
    QLabel *label;

public:
    explicit StatusString(const QString &name, QWidget *parent = 0);

    void setConnected();
    void setDisconnected();

    QString StatusLabel() const;

public slots:
    void setStatusLabel(const QSerialPort::SerialPortError error);
};

#endif // STATUSSTRING_H
