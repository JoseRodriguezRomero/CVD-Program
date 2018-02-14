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
public:
    enum Device {
        Eurotherm,
        Pfeiffer,
        MKS
    };

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

    void setPortName(const Device device, const QString &port_name);
    void setBaudRate(const Device device, const QSerialPort::BaudRate baud_rate);
    void setStopBits(const Device device, const QSerialPort::StopBits stop_bits);
    void setDataBits(const Device device, const QSerialPort::DataBits data_bits);
    void setParity(const Device device, const QSerialPort::Parity parity);

public slots:
    void apply();
    void refresh();

    void updateProductIdentifiers();

signals:
    void changePortName(const SerialSettingsWindow::Device device, const QString &port_name);
    void changeBaudRate(const SerialSettingsWindow::Device device, const QSerialPort::BaudRate baud_rate);
    void changeStopBits(const SerialSettingsWindow::Device device, const QSerialPort::StopBits stop_bits);
    void changeDataBits(const SerialSettingsWindow::Device device, const QSerialPort::DataBits data_bits);
    void changeParity(const SerialSettingsWindow::Device device, const QSerialPort::Parity parity);
};

#endif // SERIALSETTINGSWINDOW_H
