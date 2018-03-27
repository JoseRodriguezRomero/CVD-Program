#include "mainwindow.h"

#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    eurotherm_status_string("Eurotherm",this),
    mks_status_string("MKS", this),
    pfeiffer_status_string("Pfeiffer",this)
{
    QWidget *central_widget = new QWidget(this);
    central_widget->setLayout(new QVBoxLayout(central_widget));
    this->setCentralWidget(central_widget);

    QTabWidget *tab_widget = new QTabWidget(this);
    central_widget->layout()->addWidget(tab_widget);

    tab_widget->addTab(&manual_control_page,"Manual Control");
    tab_widget->addTab(&recipes_page,"Recipes");
    tab_widget->addTab(&logs_page,"Recipe Log");

    QWidget *bottom_bar = new QWidget(this);
    bottom_bar->setLayout(new QHBoxLayout(bottom_bar));
    bottom_bar->layout()->setMargin(0);
    central_widget->layout()->addWidget(bottom_bar);

    bottom_bar->layout()->addWidget(&eurotherm_status_string);
    bottom_bar->layout()->addWidget(&mks_status_string);
    bottom_bar->layout()->addWidget(&pfeiffer_status_string);

    bottom_bar->layout()->addItem(
                new QSpacerItem(0,0,QSizePolicy::MinimumExpanding,
                                                  QSizePolicy::Minimum));

    QPushButton *serial_settings = new QPushButton(this);
    serial_settings->setText("Serial Settings");
    bottom_bar->layout()->addWidget(serial_settings);

    QPushButton *exit = new QPushButton(this);
    exit->setText("Exit");
    bottom_bar->layout()->addWidget(exit);

    serial_settings_window.hide();

    eurotherm_serial = new EurothermSerialClass(nullptr);
    pfeiffer_serial = new PfeifferSerialclass(nullptr);
    mks_serial = new MKSSerialClass(nullptr);

    connect(exit,SIGNAL(clicked(bool)),this,SLOT(close()));
    connect(serial_settings,SIGNAL(clicked(bool)),this,SLOT(openSerialSettingsWindow()));

    connect(&global_timer,SIGNAL(timeout()),this,SLOT(eventLoop()));

    connect(&recipes_page,SIGNAL(RecipeStarted(bool)),&manual_control_page,SLOT(setBlockedCommands(bool)));
    connect(&recipes_page,SIGNAL(RecipeStarted(bool)),serial_settings,SLOT(setDisabled(bool)));
    connect(&recipes_page,SIGNAL(RecipeStarted(bool)),&logs_page,SLOT(startLoging(bool)));

    connect(&recipes_page,SIGNAL(RecipePaused(bool)),&manual_control_page,SLOT(setUnBlockedCommands(bool)));
    connect(&recipes_page,SIGNAL(RecipePaused(bool)),serial_settings,SLOT(setEnabled(bool)));

    connect(&recipes_page,SIGNAL(RecipeStarted(bool)),this,SLOT(onRecipeStarted(bool)));
    connect(&recipes_page,SIGNAL(RecipePaused(bool)),this,SLOT(onRecipePaused(bool)));

    connect(eurotherm_serial,SIGNAL(deviceConnected(QSerialPort::SerialPortError,bool)),
            &eurotherm_status_string,SLOT(setStatusLabel(QSerialPort::SerialPortError,bool)));
    connect(pfeiffer_serial,SIGNAL(deviceConnected(QSerialPort::SerialPortError,bool)),
            &pfeiffer_status_string,SLOT(setStatusLabel(QSerialPort::SerialPortError,bool)));
    connect(mks_serial,SIGNAL(deviceConnected(QSerialPort::SerialPortError,bool)),
            &mks_status_string,SLOT(setStatusLabel(QSerialPort::SerialPortError,bool)));

    connect(&serial_settings_window,SIGNAL(changePortName(SerialSettingsWindow::Device,QString)),
            this,SLOT(setPortName(SerialSettingsWindow::Device,QString)));
    connect(&serial_settings_window,SIGNAL(changeBaudRate(SerialSettingsWindow::Device,QSerialPort::BaudRate)),
            this,SLOT(setBaudRate(SerialSettingsWindow::Device,QSerialPort::BaudRate)));
    connect(&serial_settings_window,SIGNAL(changeParity(SerialSettingsWindow::Device,QSerialPort::Parity)),
            this,SLOT(setParity(SerialSettingsWindow::Device,QSerialPort::Parity)));
    connect(&serial_settings_window,SIGNAL(changeStopBits(SerialSettingsWindow::Device,QSerialPort::StopBits)),
            this,SLOT(setStopBits(SerialSettingsWindow::Device,QSerialPort::StopBits)));
    connect(&serial_settings_window,SIGNAL(changeDataBits(SerialSettingsWindow::Device,QSerialPort::DataBits)),
            this,SLOT(setDataBits(SerialSettingsWindow::Device,QSerialPort::DataBits)));
    connect(&serial_settings_window,SIGNAL(resetDevice(SerialSettingsWindow::Device)),
            this,SLOT(resetConnection(SerialSettingsWindow::Device)));

    connect(this,SIGNAL(startEurothermEventLoop()),eurotherm_serial,SLOT(startEventLoop()));
    connect(this,SIGNAL(deleteEurothermSerialClass()),eurotherm_serial,SLOT(deleteLater()));
    connect(this,SIGNAL(connectEurothermSerialPort()),eurotherm_serial,SLOT(connectDevice()));
    connect(this,SIGNAL(disconnectEurothermSerialPort()),eurotherm_serial,SLOT(disconnectDevice()));
    connect(this,SIGNAL(setEurothermPortName(QString)),eurotherm_serial,SLOT(setSerialPortName(QString)));
    connect(this,SIGNAL(setEurothermPortDataBits(QSerialPort::DataBits)),eurotherm_serial,SLOT(setDataBits(QSerialPort::DataBits)));
    connect(this,SIGNAL(setEurothermPortStopBits(QSerialPort::StopBits)),eurotherm_serial,SLOT(setStopBits(QSerialPort::StopBits)));
    connect(this,SIGNAL(setEurothermPortBaudRate(QSerialPort::BaudRate)),eurotherm_serial,SLOT(setBaudRate(QSerialPort::BaudRate)));
    connect(this,SIGNAL(setEurothermPortParity(QSerialPort::Parity)),eurotherm_serial,SLOT(setParity(QSerialPort::Parity)));

    connect(this,SIGNAL(startPfeifferEventLoop()),pfeiffer_serial,SLOT(startEventLoop()));
    connect(this,SIGNAL(deletePfeifferSerialClass()),pfeiffer_serial,SLOT(deleteLater()));
    connect(this,SIGNAL(connectPfeifferSerialPort()),pfeiffer_serial,SLOT(connectDevice()));
    connect(this,SIGNAL(disconnectPfeifferSerialPort()),pfeiffer_serial,SLOT(disconnectDevice()));
    connect(this,SIGNAL(setPfeifferPortName(QString)),pfeiffer_serial,SLOT(setSerialPortName(QString)));
    connect(this,SIGNAL(setPfeifferPortDataBits(QSerialPort::DataBits)),pfeiffer_serial,SLOT(setDataBits(QSerialPort::DataBits)));
    connect(this,SIGNAL(setPfeifferPortStopBits(QSerialPort::StopBits)),pfeiffer_serial,SLOT(setStopBits(QSerialPort::StopBits)));
    connect(this,SIGNAL(setPfeifferPortBaudRate(QSerialPort::BaudRate)),pfeiffer_serial,SLOT(setBaudRate(QSerialPort::BaudRate)));
    connect(this,SIGNAL(setPfeifferPortParity(QSerialPort::Parity)),pfeiffer_serial,SLOT(setParity(QSerialPort::Parity)));

    connect(this,SIGNAL(startMKSEventLoop()),mks_serial,SLOT(startEventLoop()));
    connect(this,SIGNAL(deleteMKSSerialClass()),mks_serial,SLOT(deleteLater()));
    connect(this,SIGNAL(connectMKSSerialPort()),mks_serial,SLOT(connectDevice()));
    connect(this,SIGNAL(disconnectMKSSerialPort()),mks_serial,SLOT(disconnectDevice()));
    connect(this,SIGNAL(setMKSPortName(QString)),mks_serial,SLOT(setSerialPortName(QString)));
    connect(this,SIGNAL(setMKSPortDataBits(QSerialPort::DataBits)),mks_serial,SLOT(setDataBits(QSerialPort::DataBits)));
    connect(this,SIGNAL(setMKSPortStopBits(QSerialPort::StopBits)),mks_serial,SLOT(setStopBits(QSerialPort::StopBits)));
    connect(this,SIGNAL(setMKSPortBaudRate(QSerialPort::BaudRate)),mks_serial,SLOT(setBaudRate(QSerialPort::BaudRate)));
    connect(this,SIGNAL(setMKSPortParity(QSerialPort::Parity)),mks_serial,SLOT(setParity(QSerialPort::Parity)));

    connect(this,SIGNAL(readPVInputValue(int)),
            eurotherm_serial,SLOT(requestReadPVInputValue(int)));
    connect(this,SIGNAL(readTargetSetpoint(int)),
            eurotherm_serial,SLOT(requestReadTargetSetpoint(int)));
    connect(this,SIGNAL(readSetpointRateLimitValue(int)),
            eurotherm_serial,SLOT(requestReadSetpointRateLimitValue(int)));

    connect(this,SIGNAL(readPressureAndStatus(PfeifferSerialclass::Sensor)),
            pfeiffer_serial,SLOT(requestReadStatusAndPressure(PfeifferSerialclass::Sensor)));

    connect(this,SIGNAL(readMFCSetpoint(MKSSerialClass::Channel)),
            mks_serial,SLOT(requestReadSetpoint(MKSSerialClass::Channel)));

    manual_control_page.setEurothermSerialClasss(eurotherm_serial);
    manual_control_page.setPfeifferSerialClass(pfeiffer_serial);

    recipes_page.setEurothermSerialClass(eurotherm_serial);
    logs_page.setEurothermSerialClass(eurotherm_serial);

    eurotherm_serial->moveToThread(&serial_thread);
    pfeiffer_serial->moveToThread(&serial_thread);
    mks_serial->moveToThread(&serial_thread);

    serial_thread.start();
    serial_thread.setPriority(QThread::HighestPriority);

    global_timer.setInterval(2000);
    global_timer.setSingleShot(false);
}

MainWindow::~MainWindow()
{
    emit disconnectEurothermSerialPort();
    emit disconnectPfeifferSerialPort();
    emit disconnectMKSSerialPort();

    emit deleteEurothermSerialClass();
    emit deletePfeifferSerialClass();
    emit deleteMKSSerialClass();

    QTime delete_timer;
    delete_timer.start();
    while (delete_timer.elapsed() < 500)
    {
    }

    serial_thread.quit();
    while (serial_thread.isRunning())
    {
    }
}

void MainWindow::initializeSerialDevices()
{
    emit startEurothermEventLoop();
    emit startPfeifferEventLoop();
    emit startMKSEventLoop();
    global_timer.start();
}

void MainWindow::setPortName(const SerialSettingsWindow::Device device,
                             const QString &port_name)
{
    switch (device) {
    case SerialSettingsWindow::Eurotherm:
        emit setEurothermPortName(port_name);
        break;
    case SerialSettingsWindow::Pfeiffer:
        emit setPfeifferPortName(port_name);
        break;
    case SerialSettingsWindow::MKS:
        emit setMKSPortName(port_name);
        break;
    default:
        return;
    }
}

void MainWindow::setBaudRate(const SerialSettingsWindow::Device device,
                             const QSerialPort::BaudRate baud_rate)
{
    switch (device) {
    case SerialSettingsWindow::Eurotherm:
        emit setEurothermPortBaudRate(baud_rate);
        break;
    case SerialSettingsWindow::Pfeiffer:
        emit setPfeifferPortBaudRate(baud_rate);
        break;
    case SerialSettingsWindow::MKS:
        emit setMKSPortBaudRate(baud_rate);
        break;
    default:
        return;
    }
}

void MainWindow::setStopBits(const SerialSettingsWindow::Device device,
                             const QSerialPort::StopBits stop_bits)
{
    switch (device) {
    case SerialSettingsWindow::Eurotherm:
        emit setEurothermPortStopBits(stop_bits);
        break;
    case SerialSettingsWindow::Pfeiffer:
        emit setPfeifferPortStopBits(stop_bits);
        break;
    case SerialSettingsWindow::MKS:
        emit setMKSPortStopBits(stop_bits);
        break;
    default:
        return;
    }
}

void MainWindow::setDataBits(const SerialSettingsWindow::Device device,
                             const QSerialPort::DataBits data_bits)
{
    switch (device) {
    case SerialSettingsWindow::Eurotherm:
        emit setEurothermPortDataBits(data_bits);
        break;
    case SerialSettingsWindow::Pfeiffer:
        emit setPfeifferPortDataBits(data_bits);
        break;
    case SerialSettingsWindow::MKS:
        emit setMKSPortDataBits(data_bits);
        break;
    default:
        return;
    }
}

void MainWindow::setParity(const SerialSettingsWindow::Device device,
                           const QSerialPort::Parity parity)
{
    switch (device) {
    case SerialSettingsWindow::Eurotherm:
        emit setEurothermPortParity(parity);
        break;
    case SerialSettingsWindow::Pfeiffer:
        emit setPfeifferPortParity(parity);
        break;
    case SerialSettingsWindow::MKS:
        emit setMKSPortParity(parity);
        break;
    default:
        return;
    }
}

void MainWindow::resetConnection(const SerialSettingsWindow::Device device)
{
    switch (device) {
    case SerialSettingsWindow::Eurotherm:
        emit disconnectEurothermSerialPort();
        emit connectEurothermSerialPort();
        break;
    case SerialSettingsWindow::Pfeiffer:
        emit disconnectPfeifferSerialPort();
        emit connectPfeifferSerialPort();
        break;
    case SerialSettingsWindow::MKS:
        emit disconnectMKSSerialPort();
        emit connectMKSSerialPort();
        break;
    default:
        return;
    }
}

void MainWindow::openSerialSettingsWindow()
{
    QVector<SerialSettingsWindow::Device> device_id;
    QVector<BaseSerialClass*> devices;

    device_id.append(SerialSettingsWindow::Eurotherm);
    device_id.append(SerialSettingsWindow::Pfeiffer);

    devices.append(eurotherm_serial);
    devices.append(pfeiffer_serial);

    serial_settings_window.refresh();

    for (int i = 0; i < device_id.length(); i++)
    {
        serial_settings_window.setPortName(device_id.at(i),
                                           devices.at(i)->serialPortName());
        serial_settings_window.setBaudRate(device_id.at(i),
                                           devices.at(i)->baudRate());
        serial_settings_window.setParity(device_id.at(i),
                                         devices.at(i)->parity());
        serial_settings_window.setStopBits(device_id.at(i),
                                           devices.at(i)->stopBits());
        serial_settings_window.setDataBits(device_id.at(i),
                                           devices.at(i)->dataBits());
    }

    serial_settings_window.setModal(true);
    serial_settings_window.show();
}

void MainWindow::eventLoop()
{
    for (int i = 0; i < 3; i++)
    {
        emit readPVInputValue(i+1);
        emit readTargetSetpoint(i+1);
        emit readSetpointRateLimitValue(i+1);
    }

    emit readMFCStatus(MKSSerialClass::Channel1);
    emit readMFCActualValue(MKSSerialClass::Channel1);
    emit readMFCActualValue(MKSSerialClass::Channel1);

    emit readPressureAndStatus(PfeifferSerialclass::Sensor6);
}

void MainWindow::onRecipePaused(bool recipe_paused)
{
    if (recipe_paused)
    {
        setWindowTitle("CVD-Program (Recipe paused)");
    }
    else
    {
        setWindowTitle("CVD-Program (Recipe running)");
    }
}

void MainWindow::onRecipeStarted(bool recipe_started)
{
    if (recipe_started)
    {
        setWindowTitle("CVD-Program (Recipe running)");
    }
    else
    {
        setWindowTitle("CVD-Program");
    }
}
