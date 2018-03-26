#include "statusstring.h"

char connected_style[] = "QFrame {"
                         "  background-color: green;"
                         "}";

char disconnected_style[] = "QFrame {"
                            "  background-color: red;"
                            "}";

char no_reply_style[] = "QFrame {"
                        "  background-color: orange;"
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

void StatusString::setNoReply()
{
    this->setStyleSheet(no_reply_style);
}

QString StatusString::StatusLabel() const
{
    return label->text();
}

void StatusString::setStatusLabel(const QSerialPort::SerialPortError error,
                                  const bool no_reply)
{
    QString label(name);
    bool no_error = false;

    switch (error) {
    case QSerialPort::NoError:
        label.append(": CONNECTED");
        no_error = true;
        setConnected();
        break;
    case QSerialPort::DeviceNotFoundError:
        label.append(": CONNECTED");
        break;
    case QSerialPort::PermissionError:
        label.append(": PERMISSION ERROR");
        break;
    case QSerialPort::OpenError:
        label.append(": OPEN ERROR");
        break;
    case QSerialPort::NotOpenError:
        label.append(": NOT OPEN");
        break;
    case QSerialPort::WriteError:
        label.append(": WRITE ERROR");
        break;
    case QSerialPort::ReadError:
        label.append(": READ ERROR");
        break;
    case QSerialPort::ResourceError:
        label.append(": RESOURCE ERROR");
        break;
    case QSerialPort::UnsupportedOperationError:
        label.append(": UNSUPORTED OPERATION");
        break;
    case QSerialPort::TimeoutError:
        label.append(": TIMEOUT ERROR");
        break;
    case QSerialPort::UnknownError:
        label.append(": UNKNOWN ERROR");
        break;
    default:
        break;
    }

    if (no_error)
    {
        if (no_reply)
        {
            setNoReply();
            this->label->setText(name + ": NO REPLY");
        }
        else
        {
            setConnected();
            this->label->setText(label);
        }
    }
    else
    {
        setDisconnected();
        this->label->setText(label);
    }
}
