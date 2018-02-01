#include <QDebug>
#include "recipespage.h"

void InsertStepRow(QTableWidget &parent, QDoubleValidator &validator)
{
    int rows = parent.rowCount();
    parent.setRowCount(rows+1);

    QString style_sheet = "QComboBox:!hover {"
                          " background-color: transparent;"
                          "}"
                          "QLineEdit {"
                          " background-color: transparent;"
                          " border: 0px solid white;"
                          "}";
    parent.setStyleSheet(style_sheet);

    QTableWidgetItem *vertical_header = new QTableWidgetItem();
    vertical_header->setText(QString::number(rows+1));
    parent.setVerticalHeaderItem(rows,vertical_header);

    QComboBox *cont = new QComboBox(&parent);
    cont->setFrame(false);
    cont->setFocusPolicy(Qt::StrongFocus);
    cont->addItem("Yes");
    cont->addItem("No");
    parent.setCellWidget(rows,0,cont);

    for (int i = 1; i < parent.columnCount(); i++)
    {
        QLineEdit *edit = new QLineEdit(&parent);
        edit->setFrame(false);
        edit->setValidator(&validator);
        parent.setCellWidget(rows,i,edit);
    }
}

RecipesPage::RecipesPage(QWidget *parent) :
    QWidget(parent)
{
    this->setLayout(new QVBoxLayout(this));

    this->layout()->addWidget(&recipes_table);

    recipes_table.setColumnCount(9);
    recipes_table.setRowCount(0);

    QTableWidgetItem *head_item1 =
            new QTableWidgetItem("Continue?\n[Yes / No]");

    QTableWidgetItem *head_item2 =
            new QTableWidgetItem("Duration\n[min]");

    QTableWidgetItem *head_item3 =
            new QTableWidgetItem("t1_sp\n[ºC]");

    QTableWidgetItem *head_item4 =
            new QTableWidgetItem("t2_sp\n[ºC]");

    QTableWidgetItem *head_item5 =
            new QTableWidgetItem("t3_sp\n[ºC]");

    QTableWidgetItem *head_item6 =
            new QTableWidgetItem("t1_rp\n[ºC / min]");

    QTableWidgetItem *head_item7 =
            new QTableWidgetItem("t2_rp\n[ºC / min]");

    QTableWidgetItem *head_item8 =
            new QTableWidgetItem("t3_rp\n[ºC / min]");

    QTableWidgetItem *head_item9 =
            new QTableWidgetItem("mfc_sp\n[sccm]");

    recipes_table.setHorizontalHeaderItem(0,head_item1);
    recipes_table.setHorizontalHeaderItem(1,head_item2);
    recipes_table.setHorizontalHeaderItem(2,head_item3);
    recipes_table.setHorizontalHeaderItem(3,head_item4);
    recipes_table.setHorizontalHeaderItem(4,head_item5);
    recipes_table.setHorizontalHeaderItem(5,head_item6);
    recipes_table.setHorizontalHeaderItem(6,head_item7);
    recipes_table.setHorizontalHeaderItem(7,head_item8);
    recipes_table.setHorizontalHeaderItem(8,head_item9);

    QWidget *bottom_bar = new QWidget(this);
    bottom_bar->setLayout(new QHBoxLayout(bottom_bar));
    bottom_bar->layout()->setContentsMargins(0,0,0,0);
    this->layout()->addWidget(bottom_bar);

    start_stop_recipe.setText("Start Recipe");
    bottom_bar->layout()->addWidget(&start_stop_recipe);

    pause_resume_recipe.setText("Pause");
    pause_resume_recipe.setDisabled(true);
    bottom_bar->layout()->addWidget(&pause_resume_recipe);

    bottom_bar->layout()->addItem(
                new QSpacerItem(0,0,QSizePolicy::MinimumExpanding));

    open_recipe.setText("Open Recipe");
    bottom_bar->layout()->addWidget(&open_recipe);

    save_recipe.setText("Save Recipe");
    bottom_bar->layout()->addWidget(&save_recipe);

    add_recipe_step.setText("Add Step");
    bottom_bar->layout()->addWidget(&add_recipe_step);

    rem_recipe_step.setText("Remove Step");
    bottom_bar->layout()->addWidget(&rem_recipe_step);

    cooking = false;

    connect(&add_recipe_step,SIGNAL(clicked(bool)),this,SLOT(addRecipeStep()));
    connect(&rem_recipe_step,SIGNAL(clicked(bool)),this,SLOT(removeSteps()));

    connect(&pause_resume_recipe,SIGNAL(clicked(bool)),this,
            SLOT(PauseResumeRecipe()));
    connect(&start_stop_recipe,SIGNAL(clicked(bool)),this,
            SLOT(StartStopRecipe()));

    connect(&recipe_timer,SIGNAL(timeout()),this,SLOT(RecipeTimerTicked()));


    validator.setBottom(0);
    validator.setDecimals(2);
    InsertStepRow(recipes_table, validator);

    recipe_timer.setInterval(300);
    recipe_timer.setSingleShot(false);

    recipe_step = 0;

    eurotherm_serial = nullptr;
}

RecipesPage::~RecipesPage()
{
}

void RecipesPage::setEurothermSerialClass(EurothermSerialClass *eurotherm_serial)
{
    this->eurotherm_serial = eurotherm_serial;
}

void RecipesPage::addRecipeStep()
{
    InsertStepRow(recipes_table, validator);

    int row_count = recipes_table.rowCount();

    if (recipes_table.rowCount() > 1)
    {
        QComboBox *new_step_cbox = static_cast<QComboBox*>(
                    recipes_table.cellWidget(row_count-1,0));

        QComboBox *old_step_cbox = static_cast<QComboBox*>(
                    recipes_table.cellWidget(row_count-2,0));


        new_step_cbox->setCurrentIndex(old_step_cbox->currentIndex());

        for (int i = 1; i < recipes_table.columnCount(); i++)
        {
            QLineEdit *new_line_edit = static_cast<QLineEdit*>(
                        recipes_table.cellWidget(row_count-1,i));
            QLineEdit *old_line_edit = static_cast<QLineEdit*>(
                        recipes_table.cellWidget(row_count-2,i));

            new_line_edit->setText(old_line_edit->text());
        }
    }
}

void RecipesPage::removeSteps()
{
    if (recipes_table.rowCount() == 1)
    {
        recipes_table.setRowCount(0);
        InsertStepRow(recipes_table, validator);
        return;
    }

    if (recipes_table.selectedRanges().length() == 0)
    {
        recipes_table.setRowCount(recipes_table.rowCount()-1);
    }
    else
    {
        QVector<int> selected_ranges;
        QList<QTableWidgetSelectionRange> selection_range =
                recipes_table.selectedRanges();

        for (int i = 0; i < selection_range.length(); i++)
        {
            int top = selection_range.at(i).topRow();
            int bot = selection_range.at(i).bottomRow();

            for (int j = top; j <= bot; j++)
            {
                if (!selected_ranges.contains(j))
                {
                    selected_ranges.append(j);
                }
            }
        }

        qSort(selected_ranges);
        int len = selected_ranges.length();

        for (int i = 0; i < selected_ranges.length(); i++)
        {
            recipes_table.removeRow(selected_ranges.at(len-i-1));
        }
    }

    for (int i = 0; i < recipes_table.rowCount(); i++)
    {
        QTableWidgetItem *header = const_cast<QTableWidgetItem*>(
                    recipes_table.verticalHeaderItem(i));
        header->setText(QString::number(i+1));
    }

    if (recipes_table.rowCount() == 0)
    {
        InsertStepRow(recipes_table, validator);
    }
}

void RecipesPage::StartStopRecipe()
{
    if (eurotherm_serial == nullptr)
    {
        return;
    }

    cooking = !cooking;
    paused = false;
    step_percent_completion = 0;
    step_taget_completion = 0;
    recipe_step = 0;

    double aux_target_time = static_cast<QLineEdit*>(
                recipes_table.cellWidget(0,1))->text().toDouble();

    aux_target_time *= 60*1000;
    step_target_time = aux_target_time;

    pause_resume_recipe.setText("Pause");

    if (!cooking)
    {
        start_stop_recipe.setText("Start Recipe");
        recipe_timer.stop();
    }
    else
    {
        start_stop_recipe.setText("Stop Recipe");
        recipe_timer.start();

        for (int i = 0; i < recipes_table.rowCount(); i++)
        {
            QTableWidgetItem *header = const_cast<QTableWidgetItem*>(
                        recipes_table.verticalHeaderItem(i));
            header->setText(QString::number(i+1));
        }

        double sp, rp;
        QLineEdit *aux_edit;

        for (int i = 0; i < 3; i++)
        {
            aux_edit = static_cast<QLineEdit*>(recipes_table.cellWidget(
                                                   recipe_step,2+i));
            sp = aux_edit->text().toDouble();

            aux_edit = static_cast<QLineEdit*>(recipes_table.cellWidget(
                                                   recipe_step,5+i));
            rp = aux_edit->text().toDouble();
            eurotherm_serial->requestWriteSetpoint(i,1,sp);
        }
    }

    int row_count = recipes_table.rowCount();
    int col_count = recipes_table.columnCount();

    for (int i = 0; i < row_count; i++)
    {
        for (int j = 0; j < col_count; j++)
        {
            recipes_table.cellWidget(i,j)->setDisabled(cooking);
        }
    }

    pause_resume_recipe.setDisabled(!cooking);
    add_recipe_step.setDisabled(cooking);
    rem_recipe_step.setDisabled(cooking);
    open_recipe.setDisabled(cooking);

    emit RecipeStarted(cooking);
}

void RecipesPage::PauseResumeRecipe()
{
    paused = !paused;

    if (paused)
    {
        pause_resume_recipe.setText("Resume");
        recipe_timer.stop();
    }
    else
    {
        pause_resume_recipe.setText("Pause");
        recipe_timer.start();
    }

    int row_count = recipes_table.rowCount();
    int col_count = recipes_table.columnCount();

    int aux_start = recipe_step+1;

    if (step_taget_completion == 0)
    {
        aux_start--;
    }

    for (int i = aux_start; i < row_count; i++)
    {
        for (int j = 0; j < col_count; j++)
        {
            recipes_table.cellWidget(i,j)->setEnabled(paused);
        }
    }

    add_recipe_step.setEnabled(paused);
    rem_recipe_step.setEnabled(paused);
    open_recipe.setEnabled(paused);

    emit RecipePaused(paused);
}

void RecipesPage::RecipeTimerTicked()
{
    step_taget_completion += recipe_timer.interval();
    if (step_taget_completion < step_target_time)
    {
        step_percent_completion = static_cast<double>(step_taget_completion) /
                static_cast<double>(step_target_time);
        step_percent_completion = round(step_percent_completion*100);

        QTableWidgetItem *header = const_cast<QTableWidgetItem*>(
                    recipes_table.verticalHeaderItem(recipe_step));
        QString header_string = QString::number(recipe_step+1);
        header_string += " (" + QString::number(static_cast<int>(
                                                    step_percent_completion));
        header_string += "%)";
        header->setText(header_string);
    }
    else
    {
        step_percent_completion = 0;
        step_taget_completion = 0;
        recipe_step++;

        QTableWidgetItem *previous_header =
                const_cast<QTableWidgetItem*>(
                    recipes_table.verticalHeaderItem(recipe_step-1));

        QString header_string(QString::number(recipe_step));
        header_string += " (100%)";

        previous_header->setText(header_string);

        QComboBox *cbox_cont = static_cast<QComboBox*>(
                    recipes_table.cellWidget(recipe_step-1,0));

        if(cbox_cont->currentText() == "No")
        {
            PauseResumeRecipe();
        }

        if (recipe_step >= recipes_table.rowCount())
        {
            cooking = false;
            recipe_timer.stop();

            pause_resume_recipe.setDisabled(!cooking);
            add_recipe_step.setDisabled(cooking);
            rem_recipe_step.setDisabled(cooking);
            open_recipe.setDisabled(cooking);

            start_stop_recipe.setText("Start Recipe");

            emit RecipeStarted(cooking);
        }
        else
        {
            double aux_target_time = static_cast<QLineEdit*>(
                        recipes_table.cellWidget(
                            recipe_step,1))->text().toDouble();

            aux_target_time *= 60*1000;
            step_target_time = aux_target_time;
        }

        double sp, rp;
        QLineEdit *aux_edit;

        if (recipe_step >= recipes_table.rowCount())
        {
            return;
        }

        for (int i = 0; i < 3; i++)
        {
            aux_edit = static_cast<QLineEdit*>(recipes_table.cellWidget(
                                                   recipe_step,2+i));
            sp = aux_edit->text().toDouble();

            aux_edit = static_cast<QLineEdit*>(recipes_table.cellWidget(
                                                   recipe_step,5+i));
            rp = aux_edit->text().toDouble();
            eurotherm_serial->requestWriteSetpoint(i,1,sp);
        }
    }
}
