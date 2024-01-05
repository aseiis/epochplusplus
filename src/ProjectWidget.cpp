#include "../headers/ProjectWidget.h"

ProjectWidget::ProjectWidget(QWidget* parent)
    : QWidget(parent)
    , ui(new Ui::ProjectWidgetClass())
{
    ui->setupUi(this);
}

void ProjectWidget::UpdateUI(Project* projectDataPtr)
{
    ui->projectNameLabel->setText(projectDataPtr->getProjectName());
    ui->projectTotalDurationLabel->setText(projectDataPtr->getPrettyTotalDuration());
}

ProjectWidget::~ProjectWidget()
{
    delete ui;
}