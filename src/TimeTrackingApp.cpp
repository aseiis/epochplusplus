#include "../headers/TimeTrackingApp.h"

TimeTrackingApp::TimeTrackingApp(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::TimeTrackingAppClass())
{
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