#pragma once

#include <QtWidgets/QMainWindow>
#include <QInputDialog>
#include <qmessagebox.h>

#include "ProjectData.h"
#include "ProjectWidget.h"

#include "../ui/ui_TimeTrackingApp.h"


QT_BEGIN_NAMESPACE
namespace Ui { class TimeTrackingAppClass; };
QT_END_NAMESPACE

class TimeTrackingApp : public QMainWindow
{
    Q_OBJECT

// METHODS //

public:
    TimeTrackingApp(QWidget *parent = nullptr);
    ~TimeTrackingApp();

private:
    void initSavesDir();
    void loadAndDisplayProjects();
    void askNewProject();
    void createProject(const QString& newProjectName = QString("__unnammed_project"));
    bool isProjectNameUnique(QString& testProjectName);
    void newProjectWidget(ProjectData* project);
    /*
    void refreshProjectsDisplay();
    void removeProjectWidgets();
    */
    void configureStyleSheet();

// SLOTS //

public slots:
    void deleteProject(int projectID);

// SIGNALS //

// ATTRIBUTES //

private:
    Ui::TimeTrackingAppClass *ui;
    QVBoxLayout* scrollAreaVBoxLayout;

    std::vector<ProjectData*> projects;
    std::vector<ProjectWidget*> projectsVm;


};
