#include <QDebug>

#include "logspage.h"

LogsPage::LogsPage(QWidget *parent) : QWidget(parent)
{
    this->setLayout(new QVBoxLayout(this));

    QTabWidget *tab_widget = new QTabWidget(this);
    this->layout()->addWidget(tab_widget);

    tab_widget->addTab(&reactor_chart_view,"Reactor Temperature");

    QWidget *mfc_page = new QWidget(this);
    mfc_page->setLayout(new QVBoxLayout);
    mfc_page->layout()->setContentsMargins(0,0,0,0);
    mfc_page->layout()->setSpacing(0);
    tab_widget->addTab(mfc_page,"Mass Flow Controller");

    mfc_page->layout()->addWidget(&mfc_chart_view);

    QWidget *mfc_bottom_bar = new QWidget(this);
    mfc_bottom_bar->setLayout(new QHBoxLayout(mfc_bottom_bar));
    mfc_page->layout()->addWidget(mfc_bottom_bar);

    QCheckBox *check_mfc_mflow = new QCheckBox(this);
    check_mfc_mflow->setText("Mass Flow");
    mfc_bottom_bar->layout()->addWidget(check_mfc_mflow);

    QCheckBox *check_mfc_pressure = new QCheckBox(this);
    check_mfc_pressure->setChecked(true);
    check_mfc_pressure->setText("Pressure");
    mfc_bottom_bar->layout()->addWidget(check_mfc_pressure);

    QButtonGroup *bot_group_box_mfc = new QButtonGroup(this);
    bot_group_box_mfc->addButton(check_mfc_mflow);
    bot_group_box_mfc->addButton(check_mfc_pressure);
    bot_group_box_mfc->setExclusive(true);

    mfc_bottom_bar->layout()->addItem(
                new QSpacerItem(0,0,QSizePolicy::Expanding,
                                QSizePolicy::Minimum));

    tab_widget->addTab(&gauge_chart_view,"Pressure Gaugue");

    QWidget *bottom_bar = new QWidget(this);
    bottom_bar->setLayout(new QHBoxLayout(bottom_bar));
    bottom_bar->layout()->setContentsMargins(0,0,0,0);
    this->layout()->addWidget(bottom_bar);

    QPushButton *clear_log = new QPushButton(this);
    clear_log->setText("Clear Log");
    bottom_bar->layout()->addWidget(clear_log);

    bottom_bar->layout()->addItem(
                new QSpacerItem(0,0,QSizePolicy::Expanding,
                                QSizePolicy::Minimum));

    QPushButton *open_log = new QPushButton(this);
    open_log->setText("Open Log");
    bottom_bar->layout()->addWidget(open_log);

    QPushButton *save_log = new QPushButton(this);
    save_log->setText("Save Log");
    bottom_bar->layout()->addWidget(save_log);

    reactor_chart_view.setRenderHint(QPainter::Antialiasing);
    mfc_chart_view.setRenderHint(QPainter::Antialiasing);
    gauge_chart_view.setRenderHint(QPainter::Antialiasing);

    reactor_chart_view.setChart(&reactor_temp_chart);

    QtCharts::QValueAxis *reactor_temp_x_axis = new QtCharts::QValueAxis(this);
    QtCharts::QValueAxis *reactor_temp_y_axis = new QtCharts::QValueAxis(this);

    reactor_temp_x_axis->setMin(0);
    reactor_temp_x_axis->setMax(10);
    reactor_temp_x_axis->setTitleText("Time [m]");

    reactor_temp_y_axis->setMin(0);
    reactor_temp_y_axis->setMax(10);
    reactor_temp_y_axis->setTitleText("Temperature [ºC]");

    reactor_temp_chart.setAxisX(reactor_temp_x_axis);
    reactor_temp_chart.setAxisY(reactor_temp_y_axis);

    mfc_chart_view.setChart(&mfc_pressure_chart);
    mfc_chart_view.chart()->legend()->setVisible(false);
    mfc_pressure_chart.addSeries(&mfc_pressure);

    QtCharts::QValueAxis *mfc_pressure_x_axis = new QtCharts::QValueAxis(this);
    QtCharts::QLogValueAxis *mfc_pressure_y_axis =
            new QtCharts::QLogValueAxis(this);

    mfc_pressure_x_axis->setMin(0);
    mfc_pressure_x_axis->setMax(10);
    mfc_pressure_x_axis->setTitleText("Time [m]");

    mfc_pressure_y_axis->setBase(10);
    mfc_pressure_y_axis->setMin(1.0E-2);
    mfc_pressure_y_axis->setMax(1.0E3);
    mfc_pressure_y_axis->setLabelFormat("%2.1e");
    mfc_pressure_y_axis->setTitleText("Pressure [mbar]");

    mfc_pressure_chart.setAxisX(mfc_pressure_x_axis);
    mfc_pressure_chart.setAxisY(mfc_pressure_y_axis);

    mfc_mflow_chart.addSeries(&mfc_mflow);

    QtCharts::QValueAxis *mfc_mflow_x_axis = new QtCharts::QValueAxis(this);
    QtCharts::QValueAxis *mfc_mflow_y_axis = new QtCharts::QValueAxis(this);

    mfc_mflow_x_axis->setMin(0);
    mfc_mflow_x_axis->setMax(10);
    mfc_mflow_x_axis->setTitleText("Time [m]");

    mfc_mflow_y_axis->setMin(0);
    mfc_mflow_y_axis->setMax(10);
    mfc_mflow_y_axis->setTitleText("Ar Mass Flow [sccm]");

    mfc_mflow_chart.setAxisX(mfc_mflow_x_axis);
    mfc_mflow_chart.setAxisY(mfc_mflow_y_axis);

    gauge_chart_view.setChart(&gauge_pressure_chart);
    gauge_chart_view.chart()->legend()->setVisible(false);
    gauge_pressure_chart.addSeries(&gauge_pressure);

    QtCharts::QValueAxis *gauge_pressure_x_axis =
            new QtCharts::QValueAxis(this);
    QtCharts::QLogValueAxis *gauge_pressure_y_axis =
            new QtCharts::QLogValueAxis(this);

    gauge_pressure_x_axis->setMin(0);
    gauge_pressure_x_axis->setMax(10);
    gauge_pressure_x_axis->setTitleText("Time [m]");

    gauge_pressure_y_axis->setBase(10);
    gauge_pressure_y_axis->setMin(1.0E-2);
    gauge_pressure_y_axis->setMax(1.0E3);
    gauge_pressure_y_axis->setLabelFormat("%2.1e");
    gauge_pressure_y_axis->setTitleText("Pressure [mbar]");

    gauge_pressure_chart.setAxisX(gauge_pressure_x_axis);
    gauge_pressure_chart.setAxisY(gauge_pressure_y_axis);

    log_time.start();

    connect(clear_log,SIGNAL(clicked(bool)),this,SLOT(clearLogs()));
    connect(check_mfc_mflow,SIGNAL(clicked(bool)),
            this,SLOT(setMFCtoMassFlowChart()));
    connect(check_mfc_pressure,SIGNAL(clicked(bool)),
            this,SLOT(setMFCtoPressureChart()));
}


LogsPage::~LogsPage()
{
}

void LogsPage::setEurothermSerialClass(EurothermSerialClass *eurotherm_serial)
{
    if (eurotherm_serial != nullptr)
    {
        connect(eurotherm_serial,SIGNAL(PVInputValue(int,float)),
                this,SLOT(logReactorTemp(int,float)));
    }

    this->eurotherm_serial = eurotherm_serial;

    connect(eurotherm_serial,SIGNAL(PVInputValue(int,float)),
            this,SLOT(logReactorTemp(int,float)));
}

void LogsPage::startLoging(bool log_events)
{
    this->log_events = log_events;

    if (log_events)
    {
        t1_sp.clear();
        t2_sp.clear();
        t3_sp.clear();
    }

    log_time.restart();
}

void LogsPage::clearLogs()
{
    t1_sp.clear();
    t2_sp.clear();
    t3_sp.clear();

    mfc_mflow.clear();
    mfc_pressure.clear();

    gauge_pressure.clear();
}

void LogsPage::logReactorTemp(int server_address, float temp)
{
    if (!log_events)
    {
        return;
    }

    qreal time = log_time.elapsed();
    time /= (1000.0*60.0);

    switch (server_address) {
    case 1:
        t1_sp.append(QPointF(time,temp));
        break;
    case 2:
        t2_sp.append(QPointF(time,temp));
        break;
    case 3:
        t3_sp.append(QPointF(time,temp));

        reactor_temp_chart.removeAllSeries();

        QtCharts::QLineSeries *t1_sp_ptr = new QtCharts::QLineSeries(this);
        t1_sp_ptr->setUseOpenGL(true);
        QtCharts::QLineSeries *t2_sp_ptr = new QtCharts::QLineSeries(this);
        t2_sp_ptr->setUseOpenGL(true);
        QtCharts::QLineSeries *t3_sp_ptr = new QtCharts::QLineSeries(this);
        t3_sp_ptr->setUseOpenGL(true); 

        t1_sp_ptr->setName("t1_sp (left side)");
        t2_sp_ptr->setName("t2_sp (center)");
        t3_sp_ptr->setName("t3_sp (right side)");

        t1_sp_ptr->append(t1_sp);
        t2_sp_ptr->append(t2_sp);
        t3_sp_ptr->append(t3_sp);

        reactor_temp_chart.addSeries(t1_sp_ptr);
        reactor_temp_chart.addSeries(t2_sp_ptr);
        reactor_temp_chart.addSeries(t3_sp_ptr);

        reactor_temp_chart.createDefaultAxes();

        reactor_temp_chart.axisX()->setTitleText("Time [m]");
        reactor_temp_chart.axisY()->setTitleText("Temperature [ºC]");

        break;
    }
}

void LogsPage::logMFCPressure(float pressure)
{
    if (!log_events)
    {
        return;
    }

    float time = log_time.elapsed();
    time /= (1000.0*60.0);

    mfc_pressure.append(time,pressure);
}

void LogsPage::logMFCMassFlow(float mass_flow)
{
    if (!log_events)
    {
        return;
    }

    float time = log_time.elapsed();
    time /= (1000.0*60.0);

    mfc_mflow.append(time,mass_flow);
}

void LogsPage::logGaugePressure(float pressure)
{
    if (!log_events)
    {
        return;
    }

    float time = log_time.elapsed();
    time /= (1000.0*60.0);

    gauge_pressure.append(time,pressure);
}

void LogsPage::setMFCtoPressureChart()
{
    mfc_chart_view.setChart(&mfc_pressure_chart);
    mfc_chart_view.chart()->legend()->setVisible(false);
}

void LogsPage::setMFCtoMassFlowChart()
{
    mfc_chart_view.setChart(&mfc_mflow_chart);
    mfc_chart_view.chart()->legend()->setVisible(false);
}
