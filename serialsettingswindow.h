#ifndef SERIALSETTINGSWINDOW_H
#define SERIALSETTINGSWINDOW_H

#include <QList>
#include <QLabel>
#include <QWidget>
#include <QComboBox>
#include <QLineEdit>
#include <QTabWidget>
#include <QMainWindow>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QPushButton>

#include <QSerialPort>
#include <QSerialPortInfo>

class SerialSettingsWindow : public QMainWindow
{
    Q_OBJECT
private:
    QComboBox *eurotherm_port_name;
    QComboBox *eurotherm_baud_rate;
    QComboBox *eurotherm_parity;
    QComboBox *eurotherm_stop_bits;
    QComboBox *eurotherm_data_bits;
    QLineEdit *eurotherm_manufacturer;

    QComboBox *mks_port_name;
    QComboBox *mks_baud_rate;
    QComboBox *mks_parity;
    QComboBox *mks_stop_bits;
    QComboBox *mks_data_bits;
    QLineEdit *mks_identifier;

    QComboBox *pfeiffer_port_name;
    QComboBox *pfeiffer_baud_rate;
    QComboBox *pfeiffer_parity;
    QComboBox *pfeiffer_stop_bits;
    QComboBox *pfeiffer_data_bits;
    QLineEdit *pfeiffer_identifier;

public:
    SerialSettingsWindow(QWidget *parent = 0);

    void setEurothermPortName(const QString &port_name);
    void setEurothermPortBaudRate(QSerialPort::BaudRate baud_rate);
    void setEurothermPortParity(QSerialPort::Parity parity);
    void setEurothermPortStopBits(QSerialPort::StopBits stop_bits);
    void setEurothermPortDataBits(QSerialPort::DataBits data_bits);

    void setMKSPortName(const QString &port_name);
    void setMKSPortBaudRate(QSerialPort::BaudRate baud_rate);
    void setMKSPortParity(QSerialPort::Parity parity);
    void setMKSPortStopBits(QSerialPort::StopBits stop_bits);
    void setMKSPortDataBits(QSerialPort::DataBits data_bits);

    void setPfeifferPortName(const QString &port_name);
    void setPfeifferPortBaudRate(QSerialPort::BaudRate baud_rate);
    void setPfeifferPortParity(QSerialPort::Parity parity);
    void setPfeifferPortStopBits(QSerialPort::StopBits stop_bits);
    void setPfeifferPortDataBits(QSerialPort::DataBits data_bits);

public slots:
    void apply();
    void refresh();

    void updateEurothermProductIdentifier();

signals:
    void changeEurothermPortName(const QString &port_name);
    void changeEurothermPortBaudRate(QSerialPort::BaudRate baud_rate);
    void changeEurothermPortParity(QSerialPort::Parity parity);
    void changeEurothermPortStopBits(QSerialPort::StopBits stop_bits);
    void changeEurothermPortDataBits(QSerialPort::DataBits data_bits);

    void changeMKSPortName(const QString &port_name);
    void changeMKSPortBaudRate(QSerialPort::BaudRate baud_rate);
    void changeMKSPortParity(QSerialPort::Parity parity);
    void changeMKSPortStopBits(QSerialPort::StopBits stop_bits);
    void changeMKSPortDataBits(QSerialPort::DataBits data_bits);

    void changePfeifferPortName(const QString &port_name);
    void changePfeifferPortBaudRate(QSerialPort::BaudRate baud_rate);
    void changePfeifferPortParity(QSerialPort::Parity parity);
    void changePfeifferPortStopBits(QSerialPort::StopBits stop_bits);
    void changePfeifferPortDataBits(QSerialPort::DataBits data_bits);

private:
    void emitEurothermSerialInfo();
    void emitMKSSerialInfo();
    void emitPfeifferSerialInfo();
};

#endif // SERIALSETTINGSWINDOW_H
