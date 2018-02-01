#ifndef RECIPESPAGE_H
#define RECIPESPAGE_H

#include <QDoubleSpinBox>
#include <QTableWidget>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QGridLayout>
#include <QHeaderView>
#include <QPushButton>
#include <QHeaderView>
#include <QTableView>
#include <QComboBox>
#include <QLineEdit>
#include <QAction>
#include <QWidget>
#include <QTimer>

#include "serial/eurothermserialclass.h"

class RecipesPage : public QWidget
{
    Q_OBJECT
private:
    QTableWidget recipes_table;

    QDoubleValidator validator;

    QPushButton start_stop_recipe;
    QPushButton pause_resume_recipe;
    QPushButton open_recipe;
    QPushButton save_recipe;
    QPushButton add_recipe_step;
    QPushButton rem_recipe_step;

    bool cooking;
    bool paused;

    int recipe_step;
    int step_target_time;
    int step_taget_completion;
    double step_percent_completion;
    QTimer recipe_timer;

    EurothermSerialClass *eurotherm_serial;

public:
    explicit RecipesPage(QWidget *parent = 0);
    ~RecipesPage();

    void setEurothermSerialClass(EurothermSerialClass *eurotherm_serial);

signals:
    void RecipeStarted(bool recipe_started);
    void RecipePaused(bool recipe_paused);

public slots:
    void addRecipeStep();
    void removeSteps();

    void StartStopRecipe();
    void PauseResumeRecipe();

    void RecipeTimerTicked();
};

#endif // RECIPESPAGE_H
