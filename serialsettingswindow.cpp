#include "serialsettingswindow.h"

SerialSettingsWindow::SerialSettingsWindow(QWidget *parent) :
    QMainWindow(parent)
{
    this->setWindowModality(Qt::WindowModal);
    this->setWindowTitle("Serial Settings");

    QWidget *central_widget = new QWidget(this);
    central_widget->setLayout(new QVBoxLayout(central_widget));
    this->setCentralWidget(central_widget);

    QTabWidget *tab_widget = new QTabWidget(this);
    central_widget->layout()->addWidget(tab_widget);

    QWidget *eurotherm_page = new QWidget(this);
    QWidget *mks_page = new QWidget(this);
    QWidget *pfeiffer_page = new QWidget(this);

    tab_widget->addTab(eurotherm_page,"Eurotherm");
    tab_widget->addTab(mks_page,"MKS");
    tab_widget->addTab(pfeiffer_page,"Pfeiffer");

    QWidget *bottom_bar = new QWidget(this);
    bottom_bar->setLayout(new QHBoxLayout(bottom_bar));
    bottom_bar->layout()->setContentsMargins(0,0,0,0);
    central_widget->layout()->addWidget(bottom_bar);

    QPushButton *refresh_button = new QPushButton(this);
    refresh_button->setText("Refresh");
    bottom_bar->layout()->addWidget(refresh_button);

    bottom_bar->layout()->addItem(
                new QSpacerItem(0,0,QSizePolicy::MinimumExpanding));

    QPushButton *ok_button = new QPushButton(this);
    ok_button->setText("Ok");
    bottom_bar->layout()->addWidget(ok_button);

    QPushButton *cancel_button = new QPushButton(this);
    cancel_button->setText("Cancel");
    bottom_bar->layout()->addWidget(cancel_button);

    eurotherm_page->setLayout(new QGridLayout(eurotherm_page));
    QGridLayout *eurotherm_layout =
            static_cast<QGridLayout*>(eurotherm_page->layout());

    QLabel *eurotherm_label1 = new QLabel(this);
    eurotherm_label1->setText("Port Identifier");
    eurotherm_label1->setAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
    eurotherm_layout->addWidget(eurotherm_label1,0,0,1,1);

    eurotherm_port_name = new QComboBox(this);
    eurotherm_port_name->setEditable(true);
    eurotherm_layout->addWidget(eurotherm_port_name,0,1,1,1);

    QLabel *eurotherm_label2 = new QLabel(this);
    eurotherm_label2->setText("Baud Rate");
    eurotherm_label2->setAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
    eurotherm_layout->addWidget(eurotherm_label2,1,0,1,1);

    eurotherm_baud_rate = new QComboBox(this);
    eurotherm_layout->addWidget(eurotherm_baud_rate,1,1,1,1);

    QLabel *eurotherm_label3 = new QLabel(this);
    eurotherm_label3->setText("Parity");
    eurotherm_label3->setAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
    eurotherm_layout->addWidget(eurotherm_label3,2,0,1,1);

    eurotherm_parity = new QComboBox(this);
    eurotherm_layout->addWidget(eurotherm_parity,2,1,1,1);

    eurotherm_layout->addItem(new QSpacerItem(30,0),0,2,1,1);

    QLabel *eurotherm_label4 = new QLabel(this);
    eurotherm_label4->setText("Stop Bits");
    eurotherm_label4->setAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
    eurotherm_layout->addWidget(eurotherm_label4,0,3,1,1);

    eurotherm_stop_bits = new QComboBox(this);
    eurotherm_layout->addWidget(eurotherm_stop_bits,0,4,1,1);

    QLabel *eurotherm_label5 = new QLabel(this);
    eurotherm_label5->setText("Data Bits");
    eurotherm_label5->setAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
    eurotherm_layout->addWidget(eurotherm_label5,1,3,1,1);

    eurotherm_data_bits = new QComboBox(this);
    eurotherm_layout->addWidget(eurotherm_data_bits,1,4,1,1);

    QLabel *eurotherm_label6 = new QLabel(this);
    eurotherm_label6->setText("Port Manufacturer");
    eurotherm_label6->setAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
    eurotherm_layout->addWidget(eurotherm_label6,3,0,1,1);

    eurotherm_manufacturer = new QLineEdit(this);
    eurotherm_manufacturer->setDisabled(true);
    eurotherm_layout->addWidget(eurotherm_manufacturer,3,1,1,4);

    mks_page->setLayout(new QGridLayout(mks_page));
    QGridLayout *mks_layout =
            static_cast<QGridLayout*>(mks_page->layout());

    QLabel *mks_label1 = new QLabel(this);
    mks_label1->setText("Port Identifier");
    mks_label1->setAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
    mks_layout->addWidget(mks_label1,0,0,1,1);

    mks_port_name = new QComboBox(this);
    mks_port_name->setEditable(true);
    mks_layout->addWidget(mks_port_name,0,1,1,1);

    QLabel *mks_label2 = new QLabel(this);
    mks_label2->setText("Baud Rate");
    mks_label2->setAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
    mks_layout->addWidget(mks_label2,1,0,1,1);

    mks_baud_rate = new QComboBox(this);
    mks_layout->addWidget(mks_baud_rate,1,1,1,1);

    QLabel *mks_label3 = new QLabel(this);
    mks_label3->setText("Parity");
    mks_label3->setAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
    mks_layout->addWidget(mks_label3,2,0,1,1);

    mks_parity = new QComboBox(this);
    mks_layout->addWidget(mks_parity,2,1,1,1);

    mks_layout->addItem(new QSpacerItem(30,0),0,2,1,1);

    QLabel *mks_label4 = new QLabel(this);
    mks_label4->setText("Stop Bits");
    mks_label4->setAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
    mks_layout->addWidget(mks_label4,0,3,1,1);

    mks_stop_bits = new QComboBox(this);
    mks_layout->addWidget(mks_stop_bits,0,4,1,1);

    QLabel *mks_label5 = new QLabel(this);
    mks_label5->setText("Data Bits");
    mks_label5->setAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
    mks_layout->addWidget(mks_label5,1,3,1,1);

    mks_data_bits = new QComboBox(this);
    mks_layout->addWidget(mks_data_bits,1,4,1,1);

    pfeiffer_page->setLayout(new QGridLayout(pfeiffer_page));
    QGridLayout *pfeiffer_layout =
            static_cast<QGridLayout*>(pfeiffer_page->layout());

    QLabel *pfeiffer_label1 = new QLabel(this);
    pfeiffer_label1->setText("Port Identifier");
    pfeiffer_label1->setAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
    pfeiffer_layout->addWidget(pfeiffer_label1,0,0,1,1);

    pfeiffer_port_name = new QComboBox(this);
    pfeiffer_port_name->setEditable(true);
    pfeiffer_layout->addWidget(pfeiffer_port_name,0,1,1,1);

    QLabel *pfeiffer_label2 = new QLabel(this);
    pfeiffer_label2->setText("Baud Rate");
    pfeiffer_label2->setAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
    pfeiffer_layout->addWidget(pfeiffer_label2,1,0,1,1);

    pfeiffer_baud_rate = new QComboBox(this);
    pfeiffer_layout->addWidget(pfeiffer_baud_rate,1,1,1,1);

    QLabel *pfeiffer_label3 = new QLabel(this);
    pfeiffer_label3->setText("Parity");
    pfeiffer_label3->setAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
    pfeiffer_layout->addWidget(pfeiffer_label3,2,0,1,1);

    pfeiffer_parity = new QComboBox(this);
    pfeiffer_layout->addWidget(pfeiffer_parity,2,1,1,1);

    pfeiffer_layout->addItem(new QSpacerItem(30,0),0,2,1,1);

    QLabel *pfeiffer_label4 = new QLabel(this);
    pfeiffer_label4->setText("Stop Bits");
    pfeiffer_label4->setAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
    pfeiffer_layout->addWidget(pfeiffer_label4,0,3,1,1);

    pfeiffer_stop_bits = new QComboBox(this);
    pfeiffer_layout->addWidget(pfeiffer_stop_bits,0,4,1,1);

    QLabel *pfeiffer_label5 = new QLabel(this);
    pfeiffer_label5->setText("Data Bits");
    pfeiffer_label5->setAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
    pfeiffer_layout->addWidget(pfeiffer_label5,1,3,1,1);

    pfeiffer_data_bits = new QComboBox(this);
    pfeiffer_layout->addWidget(pfeiffer_data_bits,1,4,1,1);

    eurotherm_baud_rate->insertItem(0,"1200");
    eurotherm_baud_rate->insertItem(1,"2400");
    eurotherm_baud_rate->insertItem(2,"4800");
    eurotherm_baud_rate->insertItem(3,"9600");
    eurotherm_baud_rate->insertItem(4,"19200");

    eurotherm_parity->insertItem(0,"None");
    eurotherm_parity->insertItem(1,"Even");
    eurotherm_parity->insertItem(2,"Odd");

    eurotherm_stop_bits->insertItem(0,"1");
    eurotherm_stop_bits->insertItem(1,"2");

    eurotherm_data_bits->insertItem(0,"5");
    eurotherm_data_bits->insertItem(1,"6");
    eurotherm_data_bits->insertItem(2,"7");
    eurotherm_data_bits->insertItem(3,"8");

    mks_baud_rate->insertItem(0,"1200");
    mks_baud_rate->insertItem(1,"2400");
    mks_baud_rate->insertItem(2,"4800");
    mks_baud_rate->insertItem(3,"9600");
    mks_baud_rate->insertItem(4,"19200");

    mks_parity->insertItem(0,"None");
    mks_parity->insertItem(1,"Even");
    mks_parity->insertItem(2,"Odd");

    mks_stop_bits->insertItem(0,"1");
    mks_stop_bits->insertItem(1,"2");

    mks_data_bits->insertItem(0,"5");
    mks_data_bits->insertItem(1,"6");
    mks_data_bits->insertItem(2,"7");
    mks_data_bits->insertItem(3,"8");

    pfeiffer_baud_rate->insertItem(0,"1200");
    pfeiffer_baud_rate->insertItem(1,"2400");
    pfeiffer_baud_rate->insertItem(2,"4800");
    pfeiffer_baud_rate->insertItem(3,"9600");
    pfeiffer_baud_rate->insertItem(4,"19200");

    pfeiffer_parity->insertItem(0,"None");
    pfeiffer_parity->insertItem(1,"Even");
    pfeiffer_parity->insertItem(2,"Odd");

    pfeiffer_stop_bits->insertItem(0,"1");
    pfeiffer_stop_bits->insertItem(1,"2");

    pfeiffer_data_bits->insertItem(0,"5");
    pfeiffer_data_bits->insertItem(1,"6");
    pfeiffer_data_bits->insertItem(2,"7");
    pfeiffer_data_bits->insertItem(3,"8");

    connect(ok_button,SIGNAL(clicked(bool)),this,SLOT(apply()));
    connect(cancel_button,SIGNAL(clicked(bool)),this,SLOT(close()));
    connect(refresh_button,SIGNAL(clicked(bool)),this,SLOT(refresh()));

    connect(eurotherm_port_name,SIGNAL(currentIndexChanged(int)),
            this,SLOT(updateEurothermProductIdentifier()));
}

void SerialSettingsWindow::setEurothermPortName(const QString &port_name)
{
    eurotherm_port_name->setCurrentText(port_name);
}

void SerialSettingsWindow::setEurothermPortBaudRate(
        QSerialPort::BaudRate baud_rate)
{
    switch (baud_rate) {
    case QSerialPort::Baud1200:
        eurotherm_baud_rate->setCurrentIndex(0);
        break;
    case QSerialPort::Baud2400:
        eurotherm_baud_rate->setCurrentIndex(1);
        break;
    case QSerialPort::Baud4800:
        eurotherm_baud_rate->setCurrentIndex(2);
        break;
    case QSerialPort::Baud9600:
        eurotherm_baud_rate->setCurrentIndex(3);
        break;
    case QSerialPort::Baud19200:
        eurotherm_baud_rate->setCurrentIndex(4);
        break;
    }
}

void SerialSettingsWindow::setEurothermPortParity(QSerialPort::Parity parity)
{
    switch (parity) {
    case QSerialPort::NoParity:
        eurotherm_parity->setCurrentIndex(0);
        break;
    case QSerialPort::EvenParity:
        eurotherm_parity->setCurrentIndex(1);
        break;
    case QSerialPort::OddParity:
        eurotherm_parity->setCurrentIndex(2);
        break;
    }
}

void SerialSettingsWindow::setEurothermPortStopBits(
        QSerialPort::StopBits stop_bits)
{
    switch (stop_bits) {
    case QSerialPort::OneStop:
        eurotherm_stop_bits->setCurrentIndex(0);
        break;
    case QSerialPort::TwoStop:
        eurotherm_stop_bits->setCurrentIndex(1);
        break;
    }
}

void SerialSettingsWindow::setEurothermPortDataBits(
        QSerialPort::DataBits data_bits)
{
    switch (data_bits) {
    case QSerialPort::Data5:
        eurotherm_data_bits->setCurrentIndex(0);
        break;
    case QSerialPort::Data6:
        eurotherm_data_bits->setCurrentIndex(1);
        break;
    case QSerialPort::Data7:
        eurotherm_data_bits->setCurrentIndex(2);
        break;
    case QSerialPort::Data8:
        eurotherm_data_bits->setCurrentIndex(3);
        break;
    }
}

void SerialSettingsWindow::apply()
{
    emitEurothermSerialInfo();
    emitMKSSerialInfo();
    emitPfeifferSerialInfo();
    this->close();
}

void SerialSettingsWindow::refresh()
{
    QList<QSerialPortInfo> port_list = QSerialPortInfo::availablePorts();
    QString eurotherm_current = eurotherm_port_name->currentText();
    QString mks_current = mks_port_name->currentText();
    QString pfeiffer_current = pfeiffer_port_name->currentText();

    eurotherm_port_name->clear();
    mks_port_name->clear();
    pfeiffer_port_name->clear();

    for (int i = 0; i < port_list.length(); i++)
    {
        QString port_name = port_list.at(i).portName();
        eurotherm_port_name->insertItem(i,port_name);
        eurotherm_port_name->setCurrentText(eurotherm_current);
        mks_port_name->insertItem(i,port_name);
        mks_port_name->setCurrentText(mks_current);
        pfeiffer_port_name->insertItem(i,port_name);
        pfeiffer_port_name->setCurrentText(pfeiffer_current);
    }
}

void SerialSettingsWindow::updateEurothermProductIdentifier()
{
    QSerialPortInfo selected_port(eurotherm_port_name->currentText());
    eurotherm_manufacturer->setText(selected_port.manufacturer());
}

void SerialSettingsWindow::emitEurothermSerialInfo()
{
    emit changeEurothermPortName(eurotherm_port_name->currentText());

    QSerialPort::BaudRate baud_rate;
    switch (eurotherm_baud_rate->currentIndex()) {
    case 0:
        baud_rate = QSerialPort::Baud1200;
        break;
    case 1:
        baud_rate = QSerialPort::Baud2400;
        break;
    case 2:
        baud_rate = QSerialPort::Baud4800;
        break;
    case 3:
        baud_rate = QSerialPort::Baud9600;
        break;
    default:
        baud_rate = QSerialPort::Baud19200;
        break;
    }
    emit changeEurothermPortBaudRate(baud_rate);

    QSerialPort::Parity parity;
    switch (eurotherm_parity->currentIndex()) {
    case 0:
        parity = QSerialPort::NoParity;
        break;
    case 1:
        parity = QSerialPort::EvenParity;
        break;
    default:
        parity = QSerialPort::OddParity;
        break;
    }
    emit changeEurothermPortParity(parity);

    QSerialPort::StopBits stop_bits;
    switch (eurotherm_stop_bits->currentIndex()) {
    case 0:
        stop_bits = QSerialPort::OneStop;
        break;
    default:
        stop_bits = QSerialPort::TwoStop;
        break;
    }
    emit changeEurothermPortStopBits(stop_bits);

    QSerialPort::DataBits data_bits;
    switch (eurotherm_data_bits->currentIndex()) {
    case 0:
        data_bits = QSerialPort::Data5;
        break;
    case 1:
        data_bits = QSerialPort::Data6;
        break;
    case 2:
        data_bits = QSerialPort::Data7;
        break;
    default:
        data_bits = QSerialPort::Data8;
        break;
    }
    emit changeEurothermPortDataBits(data_bits);
}

void SerialSettingsWindow::emitMKSSerialInfo()
{

}

void SerialSettingsWindow::emitPfeifferSerialInfo()
{

}
