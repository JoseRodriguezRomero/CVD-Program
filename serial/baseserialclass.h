#ifndef BASESERIALCLASS_H
#define BASESERIALCLASS_H

#include <QObject>

class BaseSerialClass : public QObject
{
    Q_OBJECT
public:
    explicit BaseSerialClass(QObject *parent = nullptr);

signals:

public slots:
};

#endif // BASESERIALCLASS_H