#include "../headers/TimeTrackingApp.h"

TimeTrackingApp::TimeTrackingApp(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::TimeTrackingAppClass())
{
    ui->setupUi(this);

    //Test
    Project projectModel = Project("Test Project");
    Project* projectModelPtr = new Project("Test project");
    ProjectWidget* projectViewPtr = new ProjectWidget(this);
    ProjectController projectController = ProjectController(projectModelPtr, projectViewPtr);

    ui->ProjectsLayout->addWidget(projectViewPtr);
}

TimeTrackingApp::~TimeTrackingApp()
{
    delete ui;
}