#include <QDebug>

#include "mainwindow.h"

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

    connect(exit,SIGNAL(clicked(bool)),this,SLOT(close()));
    connect(serial_settings,SIGNAL(clicked(bool)),
            this,SLOT(openSerialSettingsWindow()));

    connect(&serial_settings_window,SIGNAL(changeEurothermPortName(QString)),
            &eurotherm_serial,SLOT(setSerialPortName(QString)));
    connect(&serial_settings_window,SIGNAL(changeEurothermPortBaudRate(
                                              QSerialPort::BaudRate)),
            &eurotherm_serial,SLOT(setBaudRate(QSerialPort::BaudRate)));
    connect(&serial_settings_window,SIGNAL(changeEurothermPortParity(
                                              QSerialPort::Parity)),
            &eurotherm_serial,SLOT(setParity(QSerialPort::Parity)));
    connect(&serial_settings_window,SIGNAL(changeEurothermPortStopBits(
                                              QSerialPort::StopBits)),
            &eurotherm_serial,SLOT(setStopBits(QSerialPort::StopBits)));
    connect(&serial_settings_window,SIGNAL(changeEurothermPortDataBits(
                                              QSerialPort::DataBits)),
            &eurotherm_serial,SLOT(setDataBits(QSerialPort::DataBits)));

    connect(&global_timer,SIGNAL(timeout()),
            this,SLOT(UpdateWorkingSetPoints()));

    connect(&recipes_page,SIGNAL(RecipeStarted(bool)),&manual_control_page,
            SLOT(setBlockedCommands(bool)));
    connect(&recipes_page,SIGNAL(RecipeStarted(bool)),serial_settings,
            SLOT(setDisabled(bool)));
    connect(&recipes_page,SIGNAL(RecipeStarted(bool)),&logs_page,
            SLOT(startLoging(bool)));

    connect(&recipes_page,SIGNAL(RecipePaused(bool)),&manual_control_page,
            SLOT(setUnBlockedCommands(bool)));
    connect(&recipes_page,SIGNAL(RecipePaused(bool)),serial_settings,
            SLOT(setEnabled(bool)));

    connect(&recipes_page,SIGNAL(RecipeStarted(bool)),this,
            SLOT(onRecipeStarted(bool)));
    connect(&recipes_page,SIGNAL(RecipePaused(bool)),this,
            SLOT(onRecipePaused(bool)));

    connect(&eurotherm_serial,SIGNAL(deviceConnected(QModbusDevice::State)),
            this,SLOT(initializeEurotherm(QModbusDevice::State)));

    connect(&eurotherm_serial,SIGNAL(ErrorString(QString,bool)),
            &eurotherm_status_string,SLOT(setStatusLabel(QString,bool)));

    eurotherm_serial.connectDevice();
    manual_control_page.setEurothermSerialClasss(&eurotherm_serial);

    global_timer.setInterval(500);
    global_timer.setSingleShot(false);
    global_timer.start();
}

MainWindow::~MainWindow()
{
    eurotherm_serial.disconnectDevice();
}

void MainWindow::openSerialSettingsWindow()
{
    serial_settings_window.refresh();
    serial_settings_window.setEurothermPortName(
                eurotherm_serial.SerialPortName());
    serial_settings_window.setEurothermPortBaudRate(
                eurotherm_serial.BaudRate());
    serial_settings_window.setEurothermPortParity(
                eurotherm_serial.Parity());
    serial_settings_window.setEurothermPortStopBits(
                eurotherm_serial.StopBits());
    serial_settings_window.setEurothermPortDataBits(
                eurotherm_serial.DataBits());

    serial_settings_window.show();
}

void MainWindow::UpdateWorkingSetPoints()
{
    for (int i = 0; i < 3; i++)
    {
        eurotherm_serial.requestReadPVInputValue(i+1);
    }
}

void MainWindow::UpdateWorkingValues()
{
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

void MainWindow::initializeEurotherm(QModbusDevice::State state)
{
    manual_control_page.setEurothermSerialClasss(&eurotherm_serial);
    recipes_page.setEurothermSerialClass(&eurotherm_serial);
    logs_page.setEurothermSerialClass(&eurotherm_serial);

    switch (state) {
    case QModbusDevice::ConnectedState:
        for (int i = 0; i < 3; i++)
        {
            eurotherm_serial.requestWriteActiveSetpoint(
                        i,EurothermSerialClass::SP1);
            eurotherm_serial.requestReadTargetSetpoint(i+1);
            eurotherm_serial.requestReadPVInputValue(i+1);
        }
        break;
    }
}
