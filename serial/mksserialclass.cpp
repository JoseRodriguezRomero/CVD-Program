#include "mksserialclass.h"

struct MKSRequestStruct
{
    int device_address;
    int channel;
    QString mneumonic;
    QVector<QString> args;
    bool pending;
    bool enquiry;
    bool enquirying;
};

MKSSerialClass::MKSSerialClass(QObject *parent) : QObject(parent)
{

}

MKSSerialClass::~MKSSerialClass()
{

}

bool MKSSerialClass::processPending() const
{

}

void MKSSerialClass::manageReply()
{

}

void MKSSerialClass::processRequestQueue()
{

}

void MKSSerialClass::connectDevice()
{

}

void MKSSerialClass::disconnectDevice()
{

}

bool MKSSerialClass::checkState()
{

}
