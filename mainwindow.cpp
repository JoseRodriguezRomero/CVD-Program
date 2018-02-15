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
    connect(serial_settings,SIGNAL(clicked(bool)),this,SLOT(openSerialSettingsWindow()));

    connect(&global_timer,SIGNAL(timeout()),this,SLOT(UpdateWorkingSetPoints()));

    connect(&recipes_page,SIGNAL(RecipeStarted(bool)),&manual_control_page,SLOT(setBlockedCommands(bool)));
    connect(&recipes_page,SIGNAL(RecipeStarted(bool)),serial_settings,SLOT(setDisabled(bool)));
    connect(&recipes_page,SIGNAL(RecipeStarted(bool)),&logs_page,SLOT(startLoging(bool)));

    connect(&recipes_page,SIGNAL(RecipePaused(bool)),&manual_control_page,SLOT(setUnBlockedCommands(bool)));
    connect(&recipes_page,SIGNAL(RecipePaused(bool)),serial_settings,SLOT(setEnabled(bool)));

    connect(&recipes_page,SIGNAL(RecipeStarted(bool)),this,SLOT(onRecipeStarted(bool)));
    connect(&recipes_page,SIGNAL(RecipePaused(bool)),this,SLOT(onRecipePaused(bool)));

    connect(&eurotherm_serial,SIGNAL(ErrorString(QString,bool)),&eurotherm_status_string,SLOT(setStatusLabel(QString,bool)));

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

    eurotherm_serial.startEventLoop();
    manual_control_page.setEurothermSerialClasss(&eurotherm_serial);

    manual_control_page.setEurothermSerialClasss(&eurotherm_serial);
    recipes_page.setEurothermSerialClass(&eurotherm_serial);
    logs_page.setEurothermSerialClass(&eurotherm_serial);

    pfeiffer_serial.connectDevice();

    global_timer.setInterval(1000);
    global_timer.setSingleShot(false);
    global_timer.start();
}

MainWindow::~MainWindow()
{
    eurotherm_serial.disconnectDevice();
}

void MainWindow::setPortName(const SerialSettingsWindow::Device device,
                             const QString &port_name)
{
    BaseSerialClass *serial_class;

    switch (device) {
    case SerialSettingsWindow::Eurotherm:
        serial_class = &eurotherm_serial;
        break;
    case SerialSettingsWindow::Pfeiffer:
        serial_class = &pfeiffer_serial;
        break;
    case SerialSettingsWindow::MKS:
        return;
        break;
    default:
        return;
    }

    serial_class->setSerialPortName(port_name);
}

void MainWindow::setBaudRate(const SerialSettingsWindow::Device device,
                             const QSerialPort::BaudRate baud_rate)
{
    BaseSerialClass *serial_class;

    switch (device) {
    case SerialSettingsWindow::Eurotherm:
        serial_class = &eurotherm_serial;
        break;
    case SerialSettingsWindow::Pfeiffer:
        serial_class = &pfeiffer_serial;
        break;
    case SerialSettingsWindow::MKS:
        return;
        break;
    default:
        return;
    }

    serial_class->setBaudRate(baud_rate);
}

void MainWindow::setStopBits(const SerialSettingsWindow::Device device,
                             const QSerialPort::StopBits stop_bits)
{
    BaseSerialClass *serial_class;

    switch (device) {
    case SerialSettingsWindow::Eurotherm:
        serial_class = &eurotherm_serial;
        break;
    case SerialSettingsWindow::Pfeiffer:
        serial_class = &pfeiffer_serial;
        break;
    case SerialSettingsWindow::MKS:
        return;
        break;
    default:
        return;
    }

    serial_class->setStopBits(stop_bits);
}

void MainWindow::setDataBits(const SerialSettingsWindow::Device device,
                             const QSerialPort::DataBits data_bits)
{
    BaseSerialClass *serial_class;

    switch (device) {
    case SerialSettingsWindow::Eurotherm:
        serial_class = &eurotherm_serial;
        break;
    case SerialSettingsWindow::Pfeiffer:
        serial_class = &pfeiffer_serial;
        break;
    case SerialSettingsWindow::MKS:
        return;
        break;
    default:
        return;
    }

    serial_class->setDataBits(data_bits);
}

void MainWindow::setParity(const SerialSettingsWindow::Device device,
                           const QSerialPort::Parity parity)
{
    BaseSerialClass *serial_class;

    switch (device) {
    case SerialSettingsWindow::Eurotherm:
        serial_class = &eurotherm_serial;
        break;
    case SerialSettingsWindow::Pfeiffer:
        serial_class = &pfeiffer_serial;
        break;
    case SerialSettingsWindow::MKS:
        return;
        break;
    default:
        return;
    }

    serial_class->setParity(parity);
}

void MainWindow::openSerialSettingsWindow()
{
    QVector<SerialSettingsWindow::Device> device_id;
    QVector<BaseSerialClass*> devices;

    device_id.append(SerialSettingsWindow::Eurotherm);
    device_id.append(SerialSettingsWindow::Pfeiffer);

    devices.append(&eurotherm_serial);
    devices.append(&pfeiffer_serial);

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

    serial_settings_window.show();
}

void MainWindow::UpdateWorkingSetPoints()
{
    for (int i = 0; i < 3; i++)
    {
        eurotherm_serial.requestReadPVInputValue(i+1);
        eurotherm_serial.requestReadTargetSetpoint(i+1);
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
