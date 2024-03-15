#include "../headers/ProjectWidget.h"

ProjectWidget::ProjectWidget(QWidget* parent, ProjectData* projectPtr)
    : QWidget(parent), ui(new Ui::ProjectWidgetClass()), project(projectPtr)
{
    currentSessionElapsedSecs = 0;
    currentSessionTimer = new QTimer();
    connect(currentSessionTimer, &QTimer::timeout, this, [this] { ProjectWidget::updateCurrentSession(1); });

    setAttribute(Qt::WA_StyledBackground, true);

    ui->setupUi(this);

    ui->line->setStyleSheet(project->getProjectColorQSS());
    ui->playButton->setIcon(QIcon(":/icons/icons/play-solid.svg"));

    ui->editButton->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    ui->editButton->setIcon(QIcon(":/icons/icons/edit.svg"));

    connect(ui->playButton, &QToolButton::clicked, this, &ProjectWidget::togglePlay);

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
