#include "../headers/TimeTrackingApp.h"

TimeTrackingApp::TimeTrackingApp(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::TimeTrackingAppClass())
{
    InitSavesDir();
    ui->setupUi(this);

    //Test
    Project* project = new Project(0, "Test project");
    ProjectWidget* projectVM = new ProjectWidget(this, project);

    ui->ProjectsLayout->addWidget(projectVM);
}

TimeTrackingApp::~TimeTrackingApp()
{
    delete ui;
}

void TimeTrackingApp::InitSavesDir()
{
    QString savesDirPath = QDir::currentPath() + "/" + "saves/";
    QFile file(savesDirPath);
    if (!file.open(QIODevice::WriteOnly)) {
        QDir dir;
        if (!dir.mkpath(savesDirPath))
        {
            qDebug() << "ERROR: Couldn't create directory at " << savesDirPath;
            return;
        }
    }
}