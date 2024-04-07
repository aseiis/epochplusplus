#include "ProjectWidget.h"

ProjectWidget::ProjectWidget(QWidget* parent, ProjectData* projectPtr)
    : QWidget(parent), ui(new Ui::ProjectWidgetClass()), project(projectPtr)
{
    currentSessionElapsedSecs = 0;
    currentSessionTimer = new QTimer();
    connect(currentSessionTimer, &QTimer::timeout, this, [this] { ProjectWidget::updateCurrentSession(1); });

    ui->setupUi(this);

    setAttribute(Qt::WA_StyledBackground, true);
    isExpanded = false;
    ui->details->hide();
    ui->line->setStyleSheet(project->getProjectColorQSS());

    connect(ui->playButton, &QToolButton::clicked, this, &ProjectWidget::togglePlay);
    connect(ui->expandButton, &QToolButton::clicked, this, &ProjectWidget::toggleDetails);
    connect(ui->renameButton, &QToolButton::clicked, this, [this] { ProjectWidget::rename(ui->renameLineEdit->text()); });
    connect(ui->deleteButton, &QToolButton::clicked, this, &ProjectWidget::askDelete);
    connect(ui->changeColorButton,&QToolButton::clicked, this, &ProjectWidget::askAndSetColor);

    updateUI();
}

ProjectWidget::~ProjectWidget()
{
    delete ui;
    delete currentSessionTimer;
}

void ProjectWidget::updateUI()
{
    ui->projectNameLabel->setText(project->getProjectName());
    ui->projectTotalDurationLabel->setText(project->getPrettyTotalDuration());
    ui->nbSessionsLabel->setText(QString("%1 sessions").arg(project->getSessionsCount()));
    ui->avgSessionDurationLabel->setText(QString("Average session duration: %1").arg(project->getAvgSessionDuration()));
    ui->nbActiveDaysLabel->setText(QString("%1 active days").arg(project->getActiveDaysCount()));
    ui->avgTimePerDayLabel->setText(QString("Average time per active day: %1").arg(project->getAvgTimePerActiveDay()));
}

void ProjectWidget::startCurrentSessionTimer()
{
    currentSessionElapsedSecs = 0;
    currentSessionTimer->start(1000);
}

void ProjectWidget::endCurrentSessionTimer()
{
    currentSessionElapsedSecs = 0;
    currentSessionTimer->stop();
    ui->currentSessionDurationLabel->setText(getPrettyCurrentSessionDuration());
}

QString ProjectWidget::getPrettyCurrentSessionDuration()
{
    quint64 sessionDuration = currentSessionElapsedSecs;

    int minutes = static_cast<int>((sessionDuration / 60) % 60);
    int seconds = static_cast<int>(sessionDuration % 60);

    // Formatting the output as mm:ss
    return QString("%1:%2s")
        .arg(minutes, 2, 10, QChar('0'))
        .arg(seconds, 2, 10, QChar('0'));
}

void ProjectWidget::updateCurrentSession(int elapsedTime)
{
    currentSessionElapsedSecs += elapsedTime;
    ui->currentSessionDurationLabel->setText(getPrettyCurrentSessionDuration());
}

void ProjectWidget::togglePlay()
{
    if (!project->isRunning()) {
        project->start();
        startCurrentSessionTimer();
        ui->playButton->setIcon(QIcon(":/icons/icons/pause-solid.svg"));
    }
    else {
        project->stop();
        endCurrentSessionTimer();
        ui->playButton->setIcon(QIcon(":/icons/icons/play-solid.svg"));
    }

    updateUI();
}

void ProjectWidget::toggleDetails()
{
    if (isExpanded) {
        ui->details->hide();
        isExpanded = false;
        ui->expandButton->setText("More");
        ui->expandButton->setIcon(QIcon(":/icons/icons/nav-arrow-down"));
    }
    else {
        isExpanded = true;
        ui->details->show();
        ui->expandButton->setText("Close");
        ui->expandButton->setIcon(QIcon(":/icons/icons/nav-arrow-up"));
    }
}

void ProjectWidget::rename(const QString& newProjectName)
{
    if (newProjectName.isEmpty()) {
        MsgBoxGen::throwNewMessageBox(Epochpp::g_mainWindow, "Project name already used. Please provide a new, unique name.", QMessageBox::Ok)->exec();
        qDebug() << "WARNING: Tried to rename project (widget level) but new project name is empty. Aborting" << Qt::endl;
    }

    project->rename(newProjectName);
    ui->renameLineEdit->setText("");
    updateUI();
}

void ProjectWidget::askDelete()
{
    int res = MsgBoxGen::throwNewMessageBox(Epochpp::g_mainWindow,
                            QString("Are you sure you want to delete \"%1\" project?").arg(project->getProjectName()),
                            QMessageBox::Ok | QMessageBox::Cancel,
                            QMessageBox::Cancel,
                            "Project Delete")->exec();
    if(res == QMessageBox::Ok){
        immediateDelete();
    }
}

void ProjectWidget::immediateDelete()
{
    emit requestProjectDeletion(project->ID);
    //bool deletionSuccessful = ...;
    //if(deletionSuccessful)
    // ... REMOVE WIDGET
    //else
    // ... DONT
}

void ProjectWidget::askAndSetColor()
{
    QColor newColor = QColorDialog::getColor();
    if(!newColor.isValid()){
        MsgBoxGen::throwNewMessageBox(Epochpp::g_mainWindow, "Invalid color", QMessageBox::Ok)->exec();
        return;
    }
    project->setProjectColorQSS(newColor);
    ui->line->setStyleSheet(project->getProjectColorQSS());
}
