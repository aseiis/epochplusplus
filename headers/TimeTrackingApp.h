#pragma once

#include <iostream>

#include <QtWidgets/QMainWindow>
#include <QInputDialog>

#include "../headers/ProjectData.h"
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
    void loadAndDisplayProjects();
    void askNewProject();
    void createProject(QString& newProjectName = QString("Unnammed Project"));
    void newProjectWidget(ProjectData* project);
    void removeProjectWidgets();
    void refreshProjectsDisplay();

private:
    Ui::TimeTrackingAppClass *ui;
    QVBoxLayout* scrollAreaVBoxLayout;

    std::vector<ProjectData*> projects;
    std::vector<ProjectWidget*> projectsVm;


};
