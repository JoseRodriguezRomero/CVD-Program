#define LABEL_ALIGMENT      Qt::AlignHCenter| Qt::AlignVCenter

#include <QDebug>
#include "manualcontrolpage.h"

ManualControlPage::ManualControlPage(QWidget *parent) : QWidget(parent)
{
    this->setLayout(new QVBoxLayout(this));
    QVBoxLayout *central_layout = static_cast<QVBoxLayout*>(this->layout());

    QGroupBox *eurotherm_gbox = new QGroupBox(this);
    eurotherm_gbox->setLayout(new QGridLayout(eurotherm_gbox));

    eurotherm_gbox->setTitle(
                "Reactor Temperature Control (Eurotherm Controller)");
    central_layout->addWidget(eurotherm_gbox);

    QWidget *hbox_1 = new QWidget(this);
    hbox_1->setLayout(new QHBoxLayout(hbox_1));
    hbox_1->layout()->setContentsMargins(0,0,0,0);
    central_layout->addWidget(hbox_1);

    QGroupBox *mks_gbox = new QGroupBox(this);
    mks_gbox->setTitle("Ar Mass Flow Controller (MKS MFC)");
    hbox_1->layout()->addWidget(mks_gbox);

    QGroupBox *pfeiffer_gbox = new QGroupBox(this);
    pfeiffer_gbox->setTitle("Pressure Gauge (Pfeiffer Controller)");
    hbox_1->layout()->addWidget(pfeiffer_gbox);

    hbox_1->layout()->addItem(new QSpacerItem(0,0,QSizePolicy::Expanding,
                                              QSizePolicy::Minimum));

    central_layout->addItem(new QSpacerItem(QSizePolicy::Expanding,
                                            QSizePolicy::Expanding));

    QGridLayout *eurotherm_layout = static_cast<QGridLayout*>(
                eurotherm_gbox->layout());

    QGroupBox *eurotherm_left_gbox = new QGroupBox(this);
    eurotherm_left_gbox->setTitle("Left (Gas Inlet)");
    eurotherm_layout->addWidget(eurotherm_left_gbox,0,0,1,1);

    eurotherm_left_gbox->setLayout(new QGridLayout(eurotherm_left_gbox));
    QGridLayout *eurotherm_left_gbox_layout =
            static_cast<QGridLayout*>(eurotherm_left_gbox->layout());

    QLabel *eurotherm_label1_left = new QLabel(this);
    eurotherm_label1_left->setText("Actual Value\n[ºC]");
    eurotherm_label1_left->setAlignment(LABEL_ALIGMENT);
    eurotherm_left_gbox_layout->addWidget(
                eurotherm_label1_left,0,0,1,1);

    measured_temp_left.setDisabled(true);
    eurotherm_left_gbox_layout->addWidget(
                &measured_temp_left,0,1,1,1);

    QLabel *eurotherm_label2_left = new QLabel(this);
    eurotherm_label2_left->setText("Temperature\nSetpoint [ºC]");
    eurotherm_label2_left->setAlignment(LABEL_ALIGMENT);
    eurotherm_left_gbox_layout->addWidget(
                eurotherm_label2_left,1,0,1,1);

    eurotherm_left_gbox_layout->addWidget(
                &temp_setpoint_left,1,1,1,1);

    QLabel *eurotherm_label3_left = new QLabel(this);
    eurotherm_label3_left->setText("Temperature\nRamp [ºC / min]");
    eurotherm_label3_left->setAlignment(LABEL_ALIGMENT);
    eurotherm_left_gbox_layout->addWidget(
                eurotherm_label3_left,2,0,1,1);

    eurotherm_left_gbox_layout->addWidget(
                &temp_ramp_left,2,1,1,1);

    QGroupBox *eurotherm_center_gbox = new QGroupBox(this);
    eurotherm_center_gbox->setTitle("Center");
    eurotherm_layout->addWidget(eurotherm_center_gbox,0,1,1,1);

    eurotherm_center_gbox->setLayout(new QGridLayout(eurotherm_center_gbox));
    QGridLayout *eurotherm_center_gbox_layout =
            static_cast<QGridLayout*>(eurotherm_center_gbox->layout());

    QLabel *eurotherm_label1_center = new QLabel(this);
    eurotherm_label1_center->setText("Actual Value\n[ºC]");
    eurotherm_label1_center->setAlignment(LABEL_ALIGMENT);
    eurotherm_center_gbox_layout->addWidget(
                eurotherm_label1_center,0,0,1,1);

    measured_temp_center.setDisabled(true);
    eurotherm_center_gbox_layout->addWidget(
                &measured_temp_center,0,1,1,1);

    QLabel *eurotherm_label2_center = new QLabel(this);
    eurotherm_label2_center->setText("Temperature\nSetpoint [ºC]");
    eurotherm_label2_center->setAlignment(LABEL_ALIGMENT);
    eurotherm_center_gbox_layout->addWidget(
                eurotherm_label2_center,1,0,1,1);

    eurotherm_center_gbox_layout->addWidget(
                &temp_setpoint_center,1,1,1,1);

    QLabel *eurotherm_label3_center = new QLabel(this);
    eurotherm_label3_center->setText("Temperature\nRamp [ºC / min]");
    eurotherm_label3_center->setAlignment(LABEL_ALIGMENT);
    eurotherm_center_gbox_layout->addWidget(
                eurotherm_label3_center,2,0,1,1);

    eurotherm_center_gbox_layout->addWidget(
                &temp_ramp_center,2,1,1,1);

    QGroupBox *eurotherm_right_gbox = new QGroupBox(this);
    eurotherm_right_gbox->setTitle("Right (Gas Outlet)");
    eurotherm_layout->addWidget(eurotherm_right_gbox,0,2,1,1);

    eurotherm_right_gbox->setLayout(new QGridLayout(eurotherm_right_gbox));
    QGridLayout *eurotherm_right_gbox_layout =
            static_cast<QGridLayout*>(eurotherm_right_gbox->layout());

    QLabel *eurotherm_label1_right = new QLabel(this);
    eurotherm_label1_right->setText("Actual Value\n[ºC]");
    eurotherm_label1_right->setAlignment(LABEL_ALIGMENT);
    eurotherm_right_gbox_layout->addWidget(
                eurotherm_label1_right,0,0,1,1);

    measured_temp_right.setDisabled(true);
    eurotherm_right_gbox_layout->addWidget(
                &measured_temp_right,0,1,1,1);

    QLabel *eurotherm_label2_right = new QLabel(this);
    eurotherm_label2_right->setText("Temperature\nSetpoint [ºC]");
    eurotherm_label2_right->setAlignment(LABEL_ALIGMENT);
    eurotherm_right_gbox_layout->addWidget(
                eurotherm_label2_right,1,0,1,1);

    eurotherm_right_gbox_layout->addWidget(
                &temp_setpoint_right,1,1,1,1);

    QLabel *eurotherm_label3_right = new QLabel(this);
    eurotherm_label3_right->setText("Temperature\nRamp [ºC / min]");
    eurotherm_label3_right->setAlignment(LABEL_ALIGMENT);
    eurotherm_right_gbox_layout->addWidget(
                eurotherm_label3_right,2,0,1,1);

    eurotherm_right_gbox_layout->addWidget(
                &temp_ramp_right,2,1,1,1);

    mks_gbox->setLayout(new QGridLayout(mks_gbox));
    QGridLayout *mks_gbox_layout =
            static_cast<QGridLayout*>(mks_gbox->layout());

    QLabel *mks_label1 = new QLabel(this);
    mks_label1->setText("Actual Value\n[sccm]");
    mks_label1->setAlignment(LABEL_ALIGMENT);
    mks_gbox_layout->addWidget(mks_label1,0,0,1,1);

    mfc_flow.setDisabled(true);
    mks_gbox_layout->addWidget(&mfc_flow,0,1,1,1);

    QLabel *mks_label2 = new QLabel(this);
    mks_label2->setText("MFC Status");
    mks_label2->setAlignment(LABEL_ALIGMENT);
    mks_gbox_layout->addWidget(mks_label2,1,0,1,1);

    mfc_status.setDisabled(true);
    mks_gbox_layout->addWidget(&mfc_status,1,1,1,1);

    QLabel *mks_label3 = new QLabel(this);
    mks_label3->setText("Mass Flow\nSetpoint [sccm]");
    mks_label3->setAlignment(LABEL_ALIGMENT);
    mks_gbox_layout->addWidget(mks_label3,2,0,1,1);

    mks_gbox_layout->addWidget(&mfc_setpoint,2,1,1,1);

    pfeiffer_gbox->setLayout(new QGridLayout(pfeiffer_gbox));
    QGridLayout *pfeiffer_gbox_layout =
            static_cast<QGridLayout*>(pfeiffer_gbox->layout());

    QLabel *pfeiffer_label1 = new QLabel(this);
    pfeiffer_label1->setText("Gauge Pressure\n[mbar]");
    pfeiffer_label1->setAlignment(LABEL_ALIGMENT);
    pfeiffer_gbox_layout->addWidget(pfeiffer_label1,0,0,1,1);

    gauge_pressure.setDisabled(true);
    pfeiffer_gbox_layout->addWidget(&gauge_pressure,0,1,1,1);

    QLabel *pfeiffer_label2 = new QLabel(this);
    pfeiffer_label2->setText("Gauge Sensor");
    pfeiffer_label2->setAlignment(LABEL_ALIGMENT);
    pfeiffer_gbox_layout->addWidget(pfeiffer_label2,1,0,1,1);

    gauge_sensor.setDisabled(true);
    pfeiffer_gbox_layout->addWidget(&gauge_sensor,1,1,1,1);

    QLabel *pfeiffer_label3 = new QLabel(this);
    pfeiffer_label3->setText("Sensor Status");
    pfeiffer_label3->setAlignment(LABEL_ALIGMENT);
    pfeiffer_gbox_layout->addWidget(pfeiffer_label3,2,0,1,1);

    sensor_status.setDisabled(true);
    pfeiffer_gbox_layout->addWidget(&sensor_status,2,1,1,1);

    central_layout->addItem(new QSpacerItem(0,0,QSizePolicy::Expanding,
                                            QSizePolicy::Expanding));

    temp_setpoint_left.setMaximum(2000);
    temp_setpoint_left.setMinimum(0);
    temp_setpoint_center.setMaximum(2000);
    temp_setpoint_center.setMinimum(0);
    temp_setpoint_right.setMaximum(2000);
    temp_setpoint_right.setMinimum(0);

    eurotherm_serial = nullptr;
    pfeiffer_serial = nullptr;

    connect(&temp_setpoint_left,SIGNAL(editingFinished()),
            this,SLOT(requestSetTemperatureSetpoint()));
    connect(&temp_setpoint_center,SIGNAL(editingFinished()),
            this,SLOT(requestSetTemperatureSetpoint()));
    connect(&temp_setpoint_right,SIGNAL(editingFinished()),
            this,SLOT(requestSetTemperatureSetpoint()));

    connect(&temp_ramp_left,SIGNAL(editingFinished()),
            this,SLOT(requestSetTemperatureRamp()));
    connect(&temp_ramp_center,SIGNAL(editingFinished()),
            this,SLOT(requestSetTemperatureRamp()));
    connect(&temp_ramp_right,SIGNAL(editingFinished()),
            this,SLOT(requestSetTemperatureRamp()));

    connect(&mfc_setpoint,SIGNAL(editingFinished()),
            this,SLOT(requestSetMassFlowSetpoint()));
}

void ManualControlPage::setEurothermSerialClasss(
        EurothermSerialClass *eurotherm_serial)
{
    if (this->eurotherm_serial != nullptr)
    {
        disconnect(eurotherm_serial,SIGNAL(PVInputValue(int,float)),
                   this,SLOT(setMeasuredTemperature(int,float)));
        disconnect(eurotherm_serial,SIGNAL(TargetSetpoint(int,float)),
                   this,SLOT(setTemperatureSetpoint(int,float)));
        disconnect(eurotherm_serial,SIGNAL(setpointRateLimitValue(int,float)),
                   this,SLOT(setTemperatureRamp(int,float)));

        disconnect(this,SIGNAL(writeEurothermTemperatureSetpoint(int,float)),
                   eurotherm_serial,SLOT(requestWriteTargetSetpoint(int,float)));
        disconnect(this,SIGNAL(writeEurothermTemperatureRamp(int,float)),
                   eurotherm_serial,SLOT(requestWriteSetpointRateLimitValue(int,float)));
    }

    this->eurotherm_serial = eurotherm_serial;

    connect(eurotherm_serial,SIGNAL(PVInputValue(int,float)),
            this,SLOT(setMeasuredTemperature(int,float)));
    connect(eurotherm_serial,SIGNAL(targetSetpoint(int,float)),
            this,SLOT(setTemperatureSetpoint(int,float)));
    connect(eurotherm_serial,SIGNAL(setpointRateLimitValue(int,float)),
            this,SLOT(setTemperatureRamp(int,float)));

    connect(this,SIGNAL(writeEurothermTemperatureSetpoint(int,float)),
            eurotherm_serial,SLOT(requestWriteTargetSetpoint(int,float)));
    connect(this,SIGNAL(writeEurothermTemperatureRamp(int,float)),
            eurotherm_serial,SLOT(requestWriteSetpointRateLimitValue(int,float)));
}

void ManualControlPage::setPfeifferSerialClass(
        PfeifferSerialclass *pfeiffer_serial)
{
    if (this->pfeiffer_serial != nullptr)
    {
        disconnect(pfeiffer_serial,SIGNAL(sensorPressureAndStautus(PfeifferSerialclass::Sensor,PfeifferSerialclass::PressureMeasurementStatus,float)),
                   this,SLOT(setPfeifferPressure(PfeifferSerialclass::SensorStatus,float)));
    }

    connect(pfeiffer_serial,SIGNAL(sensorPressureAndStautus(PfeifferSerialclass::Sensor,PfeifferSerialclass::PressureMeasurementStatus,float)),
            this,SLOT(setPfeifferPressure(PfeifferSerialclass::Sensor,PfeifferSerialclass::PressureMeasurementStatus,float)));
}

void ManualControlPage::setMKSSerialClass(MKSSerialClass *mks_serial)
{
    if (this->mks_serial != nullptr)
    {
        disconnect(mks_serial,SIGNAL(setpoint(MKSSerialClass::Channel,float)),
                   this,SLOT(setMassFlowSetpoint(MKSSerialClass::Channel,float)));
        disconnect(mks_serial,SIGNAL(actualValue(MKSSerialClass::Channel,float)),
                   this,SLOT(setMeasuredMassFlow(MKSSerialClass::Channel,float)));

        disconnect(this,SIGNAL(writeMFCSetpoint(MKSSerialClass::Channel,float)),
                   mks_serial,SLOT(requestWriteSetpoint(MKSSerialClass::Channel,float)));
    }

    connect(mks_serial,SIGNAL(setpoint(MKSSerialClass::Channel,float)),
            this,SLOT(setMassFlowSetpoint(MKSSerialClass::Channel,float)));

    connect(mks_serial,SIGNAL(actualValue(MKSSerialClass::Channel,float)),
            this,SLOT(setMeasuredMassFlow(MKSSerialClass::Channel,float)));

    connect(this,SIGNAL(writeMFCSetpoint(MKSSerialClass::Channel,float)),
            mks_serial,SLOT(requestWriteSetpoint(MKSSerialClass::Channel,float)));
}

void ManualControlPage::setMeasuredTemperature(const int server_address,
                                               const float temperature)
{
    QLineEdit *measured_temp;

    switch (server_address) {
    case 1:
        measured_temp = &measured_temp_left;
        break;
    case 2:
        measured_temp = &measured_temp_center;
        break;
    case 3:
        measured_temp = &measured_temp_right;
        break;
    default:
        return;
    }

    measured_temp->setText(QString::number(temperature));
}

void ManualControlPage::setTemperatureSetpoint(const int server_address,
                                               const float setpoint)
{
    QDoubleSpinBox *temp_setpoint;

    switch (server_address) {
    case 1:
        temp_setpoint = &temp_setpoint_left;
        break;
    case 2:
        temp_setpoint = &temp_setpoint_center;
        break;
    case 3:
        temp_setpoint = &temp_setpoint_right;
        break;
    default:
        return;
    }

    if (temp_setpoint->hasFocus())
    {
        return;
    }

    temp_setpoint->setValue(setpoint);
}

void ManualControlPage::setTemperatureRamp(const int server_address,
                                           const float ramp)
{
    QDoubleSpinBox *temp_ramp;

    switch (server_address) {
    case 1:
        temp_ramp = &temp_ramp_left;
        break;
    case 2:
        temp_ramp = &temp_ramp_center;
        break;
    case 3:
        temp_ramp = &temp_ramp_right;
        break;
    default:
        return;
    }

    if (temp_ramp->hasFocus())
    {
        return;
    }

    temp_ramp->setValue(ramp);
}

void ManualControlPage::setPfeifferPressure(
        PfeifferSerialclass::Sensor sensor,
        PfeifferSerialclass::PressureMeasurementStatus status,
        const float pressure)
{
    gauge_pressure.setText(QString::number(pressure,'E'));

    switch (sensor) {
    case PfeifferSerialclass::Sensor1:
        gauge_sensor.setText("Sensor 1");
        break;
    case PfeifferSerialclass::Sensor2:
        gauge_sensor.setText("Sensor 2");
        break;
    case PfeifferSerialclass::Sensor3:
        gauge_sensor.setText("Sensor 3");
        break;
    case PfeifferSerialclass::Sensor4:
        gauge_sensor.setText("Sensor 4");
        break;
    case PfeifferSerialclass::Sensor5:
        gauge_sensor.setText("Sensor 5");
        break;
    case PfeifferSerialclass::Sensor6:
        gauge_sensor.setText("Sensor 6");
        break;
    default:
        break;
    }

    switch (status) {
    case PfeifferSerialclass::MeasurementDataOkay:
        sensor_status.setText("Sensor OK");
        break;
    case PfeifferSerialclass::Underrange:
        sensor_status.setText("Underrange Error");
        break;
    case PfeifferSerialclass::Overrange:
        sensor_status.setText("Overrange Error");
        break;
    case PfeifferSerialclass::SensorError:
        sensor_status.setText("Unknown Error");
        break;
    case PfeifferSerialclass::SensorOff:
        sensor_status.setText("Sensor Off");
        break;
    case PfeifferSerialclass::NoSensor:
        sensor_status.setText("No Sensor");
        break;
    case PfeifferSerialclass::IdentificationError:
        sensor_status.setText("Identification Error");
        break;
    default:
        break;
    }
}

void ManualControlPage::setMeasuredMassFlow(
        const MKSSerialClass::Channel channel, const float actual_value)
{
    mfc_flow.setText(QString::number(actual_value));
}

void ManualControlPage::setMassFlowSetpoint(
        const MKSSerialClass::Channel channel, const float setpoint)
{
    if (mfc_setpoint.hasFocus())
    {
        return;
    }

    mfc_setpoint.setValue(setpoint);
}

void ManualControlPage::setBlockedCommands(bool block)
{
    temp_setpoint_left.setDisabled(block);
    temp_ramp_left.setDisabled(block);

    temp_setpoint_center.setDisabled(block);
    temp_ramp_center.setDisabled(block);

    temp_setpoint_right.setDisabled(block);
    temp_ramp_right.setDisabled(block);

    mfc_setpoint.setDisabled(block);
}

void ManualControlPage::setUnBlockedCommands(bool unblock)
{
    setBlockedCommands(!unblock);
}

void ManualControlPage::requestSetTemperatureSetpoint()
{ 
    QDoubleSpinBox *sender_widget = static_cast<QDoubleSpinBox*>(sender());

    if (!sender_widget->hasFocus())
    {
        return;
    }

    sender_widget->clearFocus();

    intptr_t send_address = reinterpret_cast<intptr_t>(sender_widget);
    intptr_t l_address = reinterpret_cast<intptr_t>(&temp_setpoint_left);
    intptr_t c_address = reinterpret_cast<intptr_t>(&temp_setpoint_center);
    intptr_t r_address = reinterpret_cast<intptr_t>(&temp_setpoint_right);

    if (send_address == l_address)
    {
        emit writeEurothermTemperatureSetpoint(1,temp_setpoint_left.value());
    }
    else if (send_address == c_address)
    {
        emit writeEurothermTemperatureSetpoint(2,temp_setpoint_center.value());
    }
    else if (send_address == r_address)
    {
        emit writeEurothermTemperatureSetpoint(3,temp_setpoint_right.value());
    }
}

void ManualControlPage::requestSetTemperatureRamp()
{
    QDoubleSpinBox *sender_widget = static_cast<QDoubleSpinBox*>(sender());

    if (!sender_widget->hasFocus())
    {
        return;
    }

    sender_widget->clearFocus();

    intptr_t send_address = reinterpret_cast<intptr_t>(sender_widget);
    intptr_t l_address = reinterpret_cast<intptr_t>(&temp_ramp_left);
    intptr_t c_address = reinterpret_cast<intptr_t>(&temp_ramp_center);
    intptr_t r_address = reinterpret_cast<intptr_t>(&temp_ramp_right);

    if (send_address == l_address)
    {
        emit writeEurothermTemperatureRamp(1,temp_ramp_left.value());
    }
    else if (send_address == c_address)
    {
        emit writeEurothermTemperatureRamp(2,temp_ramp_center.value());
    }
    else if (send_address == r_address)
    {
        emit writeEurothermTemperatureRamp(3,temp_ramp_right.value());
    }
}

void ManualControlPage::requestSetMassFlowSetpoint()
{
    QDoubleSpinBox *sender_widget = static_cast<QDoubleSpinBox*>(sender());

    if (!sender_widget->hasFocus())
    {
        return;
    }

    sender_widget->clearFocus();
    emit writeMFCSetpoint(MKSSerialClass::Channel1,sender_widget->value());
}
