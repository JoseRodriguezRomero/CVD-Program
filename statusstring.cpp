#include "statusstring.h"

char connected_style[] = "QFrame {"
                             "  background-color: green;"
                             "}";

char disconnected_style[] = "QFrame {"
                             "  background-color: red;"
                             "}";

StatusString::StatusString(const QString &name, QWidget *parent) :
    QFrame(parent),
    name(name)
{
    this->setLayout(new QHBoxLayout(this));

    this->layout()->setContentsMargins(5,5,5,5);
    this->setStyleSheet(disconnected_style);

    label = new QLabel(this);
    label->setText(name + ": CONNECTION ERROR");
    this->layout()->addWidget(label);
}

void StatusString::setConnected()
{
    this->setStyleSheet(connected_style);
}

void StatusString::setDisconnected()
{
    this->setStyleSheet(disconnected_style);
}

QString StatusString::StatusLabel() const
{
    return label->text();
}

void StatusString::setStatusLabel(const QString &label, bool status)
{
    if (status)
    {
        setConnected();
    }
    else
    {
        setDisconnected();
    }

    this->label->setText(label);
}
