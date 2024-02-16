#pragma once

#include <QtWidgets/QMainWindow>

#include "../headers/Project.h"
#include "../headers/ProjectWidget.h"

#include "../ui/ui_TimeTrackingApp.h"


QT_BEGIN_NAMESPACE
namespace Ui { class TimeTrackingAppClass; };
QT_END_NAMESPACE

class TimeTrackingApp : public QMainWindow
{
    Q_OBJECT

public:
    TimeTrackingApp(QWidget *parent = nullptr);
    ~TimeTrackingApp();

private:
    void initSavesDir();
    void loadAllProjects();
    void createProjectWidgets();
    void removeProjectWidgets();
    void refreshProjectsDisplay();

private:
    Ui::TimeTrackingAppClass *ui;
    QVBoxLayout* scrollAreaVBoxLayout;

    std::vector<Project*> projects;
    std::vector<ProjectWidget*> projectsVm;


};
