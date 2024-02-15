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

    Project* project2 = new Project(0, "Very long length line height length project 2");
    ProjectWidget* projectVM2 = new ProjectWidget(this, project2);

    Project* project3 = new Project(0, "~~~~~~~~~~~~~~~~~~");
    ProjectWidget* projectVM3 = new ProjectWidget(this, project3);

    Project* project4 = new Project(0, "Test project 323");
    ProjectWidget* projectVM4 = new ProjectWidget(this, project4);

    Project* project5 = new Project(0, "Test project 552");
    ProjectWidget* projectVM5 = new ProjectWidget(this, project5);

    
    Project* project6 = new Project(0, "Test project 6");
    ProjectWidget* projectVM6 = new ProjectWidget(this, project6);

    Project* project7 = new Project(0, "Test project 7");
    ProjectWidget* projectVM7 = new ProjectWidget(this, project7);
    

    QVBoxLayout* scrollAreaVBoxLayout = new QVBoxLayout(ui->scrollAreaWidgetContents);
    ui->scrollAreaWidgetContents->setLayout(scrollAreaVBoxLayout);

    scrollAreaVBoxLayout->addWidget(projectVM);
    scrollAreaVBoxLayout->addWidget(projectVM2);
    scrollAreaVBoxLayout->addWidget(projectVM3);
    scrollAreaVBoxLayout->addWidget(projectVM4);
    scrollAreaVBoxLayout->addWidget(projectVM5);
    scrollAreaVBoxLayout->addWidget(projectVM6);
    scrollAreaVBoxLayout->addWidget(projectVM7);
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