#pragma once

#include <QtWidgets/QMainWindow>
#include <QInputDialog>
#include <QFileDialog>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
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
    void initOpenedFiles();
    void initSavesDir();
    void loadFrom(QDir savesDir);
    void loadAt(const QString filepath, bool trackingCheckEnabled=true);
    void loadAllOpenedProjects();
    void loadProjectsFromDefaultDirectory();
    void createProject(const QString& newProjectName = QString("__unnammed_project"));
    bool isProjectNameUnique(QString& testProjectName);
    bool isProjectAlreadyTracked(const QString& filepath);
    void newProjectWidget(ProjectData* project);
    /*
    void refreshProjectsDisplay();
    void removeProjectWidgets();
    */
    void configureStyleSheet();

// SLOTS //

public slots:
    void askNewProject();
    void loadSingleProject();
    void loadAllProjectsFrom();
    void deleteProject(int projectID);
    void untrackProject(const QString& filepath);
    void addToOpenedFiles(const QString& projectFilepath);

// SIGNALS //

// ATTRIBUTES //

private:
    Ui::TimeTrackingAppClass *ui;
    QVBoxLayout* scrollAreaVBoxLayout;

    std::vector<ProjectData*> projects;
    std::vector<ProjectWidget*> projectsVm;


};
