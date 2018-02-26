#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTabWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QStatusBar>
#include <QThread>
#include <QTimer>

#include "manualcontrolpage.h"
#include "recipespage.h"
#include "logspage.h"
#include "statusstring.h"

#include "serial/eurothermserialclass.h"
#include "serial/pfeifferserialclass.h"

#include "serialsettingswindow.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

private:
    ManualControlPage manual_control_page;
    RecipesPage recipes_page;
    LogsPage logs_page;

    StatusString eurotherm_status_string;
    StatusString mks_status_string;
    StatusString pfeiffer_status_string;

    QThread serial_thread;
    EurothermSerialClass *eurotherm_serial;
    PfeifferSerialclass *pfeiffer_serial;

    SerialSettingsWindow serial_settings_window;

    QTimer global_timer;

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void initializeSerialDevices();

public slots:
    void setPortName(const SerialSettingsWindow::Device device,
                     const QString &port_name);
    void setBaudRate(const SerialSettingsWindow::Device device,
                     const QSerialPort::BaudRate baud_rate);
    void setStopBits(const SerialSettingsWindow::Device device,
                     const QSerialPort::StopBits stop_bits);
    void setDataBits(const SerialSettingsWindow::Device device,
                     const QSerialPort::DataBits data_bits);
    void setParity(const SerialSettingsWindow::Device device,
                   const QSerialPort::Parity parity);
    void resetConnection(const SerialSettingsWindow::Device device);

private slots:
    void openSerialSettingsWindow();

    void eventLoop();

    void onRecipePaused(bool recipe_paused);
    void onRecipeStarted(bool recipe_stopped);

signals:
    void readPressureAndStatus(const PfeifferSerialclass::Sensor sensor);
    void readReadPVInputValue(const int server_address);
    void readReadTargetSetpoint(const int server_address);

    void setEurothermPortName(const QString &name);
    void setEurothermPortParity(const QSerialPort::Parity parity);
    void setEurothermPortBaudRate(const QSerialPort::BaudRate baud_rate);
    void setEurothermPortStopBits(const QSerialPort::StopBits stop_bits);
    void setEurothermPortDataBits(const QSerialPort::DataBits data_bits);
    void startEurothermEventLoop();

    void setPfeifferPortName(const QString &name);
    void setPfeifferPortParity(const QSerialPort::Parity parity);
    void setPfeifferPortBaudRate(const QSerialPort::BaudRate baud_rate);
    void setPfeifferPortStopBits(const QSerialPort::StopBits stop_bits);
    void setPfeifferPortDataBits(const QSerialPort::DataBits data_bits);
    void startPfeifferEventLoop();

    void disconnectEurothermSerialPort();
    void connectEurothermSerialPort();

    void disconnectPfeifferSerialPort();
    void connectPfeifferSerialPort();
};

#endif // MAINWINDOW_H
