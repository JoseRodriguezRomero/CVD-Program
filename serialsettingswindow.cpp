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

    QVector<QComboBox*> baud_rates;
    baud_rates.append(eurotherm_baud_rate);
    baud_rates.append(mks_baud_rate);
    baud_rates.append(pfeiffer_baud_rate);

    QVector<QComboBox*> parities;
    parities.append(eurotherm_parity);
    parities.append(mks_parity);
    parities.append(pfeiffer_parity);

    QVector<QComboBox*> stop_bits;
    stop_bits.append(eurotherm_stop_bits);
    stop_bits.append(mks_stop_bits);
    stop_bits.append(pfeiffer_stop_bits);

    QVector<QComboBox*> data_bits;
    data_bits.append(eurotherm_data_bits);
    data_bits.append(mks_data_bits);
    data_bits.append(pfeiffer_data_bits);

    for (int i = 0; i < 3; i++)
    {
        baud_rates.at(i)->insertItem(0,"1200");
        baud_rates.at(i)->insertItem(1,"2400");
        baud_rates.at(i)->insertItem(2,"4800");
        baud_rates.at(i)->insertItem(3,"9600");
        baud_rates.at(i)->insertItem(4,"19200");

        parities.at(i)->insertItem(0,"None");
        parities.at(i)->insertItem(1,"Even");
        parities.at(i)->insertItem(2,"Odd");

        stop_bits.at(i)->insertItem(0,"1");
        stop_bits.at(i)->insertItem(1,"2");

        data_bits.at(i)->insertItem(0,"5");
        data_bits.at(i)->insertItem(1,"6");
        data_bits.at(i)->insertItem(2,"7");
        data_bits.at(i)->insertItem(3,"8");
    }

    connect(ok_button,SIGNAL(clicked(bool)),this,SLOT(apply()));
    connect(cancel_button,SIGNAL(clicked(bool)),this,SLOT(close()));
    connect(refresh_button,SIGNAL(clicked(bool)),this,SLOT(refresh()));

    connect(eurotherm_port_name,SIGNAL(currentIndexChanged(int)),
            this,SLOT(updateProductIdentifiers()));
}

void SerialSettingsWindow::setPortName(
        const SerialSettingsWindow::Device device, const QString &port_name)
{
    QComboBox *port_name_combobox;

    switch (device) {
    case Eurotherm:
        port_name_combobox = eurotherm_port_name;
        break;
    case Pfeiffer:
        port_name_combobox = pfeiffer_port_name;
        break;
    case MKS:
        port_name_combobox = mks_port_name;
        break;
    default:
        return;
    }

    port_name_combobox->setCurrentText(port_name);
}

void SerialSettingsWindow::setBaudRate(
        const SerialSettingsWindow::Device device,
        const QSerialPort::BaudRate baud_rate)
{
    QComboBox *baud_rate_combobox;

    switch (device) {
    case Eurotherm:
        baud_rate_combobox = eurotherm_baud_rate;
        break;
    case Pfeiffer:
        baud_rate_combobox = pfeiffer_baud_rate;
        break;
    case MKS:
        baud_rate_combobox = mks_baud_rate;
        break;
    default:
        return;
    }

    switch (baud_rate) {
    case QSerialPort::Baud1200:
        baud_rate_combobox->setCurrentIndex(0);
        break;
    case QSerialPort::Baud2400:
        baud_rate_combobox->setCurrentIndex(1);
        break;
    case QSerialPort::Baud4800:
        baud_rate_combobox->setCurrentIndex(2);
        break;
    case QSerialPort::Baud9600:
        baud_rate_combobox->setCurrentIndex(3);
        break;
    case QSerialPort::Baud19200:
        baud_rate_combobox->setCurrentIndex(4);
        break;
    }
}

void SerialSettingsWindow::setStopBits(
        const SerialSettingsWindow::Device device,
        const QSerialPort::StopBits stop_bits)
{
    QComboBox *stop_bits_combobox;

    switch (device) {
    case Eurotherm:
        stop_bits_combobox = eurotherm_stop_bits;
        break;
    case Pfeiffer:
        stop_bits_combobox = pfeiffer_stop_bits;
        break;
    case MKS:
        stop_bits_combobox = mks_stop_bits;
        break;
    default:
        return;
    }

    switch (stop_bits) {
    case QSerialPort::OneStop:
        stop_bits_combobox->setCurrentIndex(0);
        break;
    case QSerialPort::TwoStop:
        stop_bits_combobox->setCurrentIndex(1);
        break;
    }
}

void SerialSettingsWindow::setDataBits(
        const SerialSettingsWindow::Device device,
        const QSerialPort::DataBits data_bits)
{
    QComboBox *data_bits_combobox;

    switch (device) {
    case Eurotherm:
        data_bits_combobox = eurotherm_data_bits;
        break;
    case Pfeiffer:
        data_bits_combobox = pfeiffer_data_bits;
        break;
    case MKS:
        data_bits_combobox = mks_data_bits;
        break;
    default:
        return;
    }

    switch (data_bits) {
    case QSerialPort::Data5:
        data_bits_combobox->setCurrentIndex(0);
        break;
    case QSerialPort::Data6:
        data_bits_combobox->setCurrentIndex(1);
        break;
    case QSerialPort::Data7:
        data_bits_combobox->setCurrentIndex(2);
        break;
    case QSerialPort::Data8:
        data_bits_combobox->setCurrentIndex(3);
        break;
    }
}

void SerialSettingsWindow::setParity(const SerialSettingsWindow::Device device,
                                     const QSerialPort::Parity parity)
{
    QComboBox *parity_combobox;

    switch (device) {
    case Eurotherm:
        parity_combobox = eurotherm_parity;
        break;
    case Pfeiffer:
        parity_combobox = pfeiffer_parity;
        break;
    case MKS:
        parity_combobox = mks_parity;
        break;
    default:
        return;
    }

    switch (parity) {
    case QSerialPort::NoParity:
        parity_combobox->setCurrentIndex(0);
        break;
    case QSerialPort::EvenParity:
        parity_combobox->setCurrentIndex(1);
        break;
    case QSerialPort::OddParity:
        parity_combobox->setCurrentIndex(2);
        break;
    }
}

void SerialSettingsWindow::apply()
{
    QVector<Device> devices = {Eurotherm, Pfeiffer, MKS};

    QVector<QComboBox*> port_names_comboboxes;
    port_names_comboboxes.append(eurotherm_port_name);
    port_names_comboboxes.append(pfeiffer_port_name);
    port_names_comboboxes.append(mks_port_name);

    QVector<QComboBox*> baud_rate_comboboxes;
    baud_rate_comboboxes.append(eurotherm_baud_rate);
    baud_rate_comboboxes.append(pfeiffer_baud_rate);
    baud_rate_comboboxes.append(mks_baud_rate);

    QVector<QComboBox*> parity_comboboxes;
    parity_comboboxes.append(eurotherm_parity);
    parity_comboboxes.append(pfeiffer_parity);
    parity_comboboxes.append(mks_parity);

    QVector<QComboBox*> stop_bits_comboboxes;
    stop_bits_comboboxes.append(eurotherm_stop_bits);
    stop_bits_comboboxes.append(pfeiffer_stop_bits);
    stop_bits_comboboxes.append(mks_stop_bits);

    QVector<QComboBox*> data_bits_comboboxes;
    data_bits_comboboxes.append(eurotherm_data_bits);
    data_bits_comboboxes.append(pfeiffer_data_bits);
    data_bits_comboboxes.append(mks_data_bits);

    for (int i = 0; i < 3; i++)
    {
        Device device = devices.at(i);
        QString port_name = port_names_comboboxes.at(i)->currentText();

        QSerialPort::BaudRate baud_rate;
        switch (baud_rate_comboboxes.at(i)->currentIndex()) {
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

        QSerialPort::Parity parity;
        switch (parity_comboboxes.at(i)->currentIndex()) {
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

        QSerialPort::StopBits stop_bits;
        switch (stop_bits_comboboxes.at(i)->currentIndex()) {
        case 0:
            stop_bits = QSerialPort::OneStop;
            break;
        default:
            stop_bits = QSerialPort::TwoStop;
            break;
        }

        QSerialPort::DataBits data_bits;
        switch (data_bits_comboboxes.at(i)->currentIndex()) {
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

        emit changePortName(device,port_name);
        emit changeBaudRate(device,baud_rate);
        emit changeParity(device,parity);
        emit changeStopBits(device,stop_bits);
        emit changeDataBits(device,data_bits);
    }

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

void SerialSettingsWindow::updateProductIdentifiers()
{
    QSerialPortInfo selected_port(eurotherm_port_name->currentText());
    eurotherm_manufacturer->setText(selected_port.manufacturer());
}
