#ifndef LOGSPAGE_H
#define LOGSPAGE_H

#include <QTime>
#include <QChart>
#include <QWidget>
#include <QCheckBox>
#include <QTabWidget>
#include <QChartView>
#include <QValueAxis>
#include <QLineSeries>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QButtonGroup>
#include <QLogValueAxis>

#include "serial/eurothermserialclass.h"

class LogsPage : public QWidget
{
    Q_OBJECT
private:
    QtCharts::QChartView reactor_chart_view;
    QtCharts::QChartView mfc_chart_view;
    QtCharts::QChartView gauge_chart_view;

    QList<QPointF> t1_sp;
    QList<QPointF> t2_sp;
    QList<QPointF> t3_sp;

    QtCharts::QLineSeries mfc_mflow;
    QtCharts::QLineSeries mfc_pressure;

    QtCharts::QLineSeries gauge_pressure;

    QtCharts::QChart reactor_temp_chart;
    QtCharts::QChart mfc_mflow_chart;
    QtCharts::QChart mfc_pressure_chart;
    QtCharts::QChart gauge_pressure_chart;

    bool log_events;
    QTime log_time;

    EurothermSerialClass *eurotherm_serial;

public:
    explicit LogsPage(QWidget *parent = nullptr);
    ~LogsPage();

    void setEurothermSerialClass(EurothermSerialClass *eurotherm_serial);

signals:

public slots:
    void startLoging(bool log_events);

    void clearLogs();

    void logReactorTemp(int server_address, float temp);
    void logMFCPressure(float pressure);
    void logMFCMassFlow(float mass_flow);
    void logGaugePressure(float pressure);

    void setMFCtoPressureChart();
    void setMFCtoMassFlowChart();

};

#endif // LOGSPAGE_H
