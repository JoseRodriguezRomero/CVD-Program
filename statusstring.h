#ifndef STATUSSTRING_H
#define STATUSSTRING_H

#include <QWidget>
#include <QHBoxLayout>
#include <QFrame>
#include <QLabel>

class StatusString : public QFrame
{
    Q_OBJECT
private:
    QString name;
    QLabel *label;

public:
    explicit StatusString(const QString &name, QWidget *parent = 0);

    void setConnected();
    void setDisconnected();

    QString StatusLabel() const;

public slots:
    void setStatusLabel(const QString &label, bool status);
};

#endif // STATUSSTRING_H
