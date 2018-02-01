#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTabWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QStatusBar>
#include <QTimer>

#include <QThread>

#include "manualcontrolpage.h"
#include "recipespage.h"
#include "logspage.h"
#include "statusstring.h"

#include "serial/eurothermserialclass.h"

#include "serialsettingswindow.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT
private:
    ManualControlPage manual_control_page;
    RecipesPage recipes_page;
    LogsPage logs_page;

    StatusString eurotherm_status_string;
    StatusString mks_status_string;
    StatusString pfeiffer_status_string;

    EurothermSerialClass eurotherm_serial;

    SerialSettingsWindow serial_settings_window;

    QTimer global_timer;

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void openSerialSettingsWindow();

    void UpdateWorkingSetPoints();
    void UpdateWorkingValues();

    void onRecipePaused(bool recipe_paused);
    void onRecipeStarted(bool recipe_stopped);

    void initializeEurotherm(QModbusDevice::State state);
};

#endif // MAINWINDOW_H
