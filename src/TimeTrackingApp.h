#pragma once

#include <iostream>

#include <QtWidgets/QMainWindow>
#include <QInputDialog>
#include <qmessagebox.h>

#include "ProjectData.h"
#include "ProjectWidget.h"

#include "../ui/ui_TimeTrackingApp.h"


QT_BEGIN_NAMESPACE
namespace Ui { class TimeTrackingAppClass; };
QT_END_NAMESPACE

const QString APP_NAME = QString("Epoch++");

class TimeTrackingApp : public QMainWindow
{
    Q_OBJECT

// METHODS //

public:
    TimeTrackingApp(QWidget *parent = nullptr);
    ~TimeTrackingApp();
    static QMessageBox* throwNewMessageBox(QMainWindow* mainWindow, QString text, QFlags<QMessageBox::StandardButton> buttons, QMessageBox::StandardButton defaultButton = QMessageBox::NoButton, QString windowTitle = "");

private:
    void initSavesDir();
    void loadAndDisplayProjects();
    void askNewProject();
    void createProject(const QString& newProjectName = QString("__unnammed_project"));
    bool isProjectNameUnique(QString& testProjectName);
    void newProjectWidget(ProjectData* project);
    void removeProjectWidgets();
    void refreshProjectsDisplay();
    void configureStyleSheet();

// ATTRIBUTES //

private:
    Ui::TimeTrackingAppClass *ui;
    QVBoxLayout* scrollAreaVBoxLayout;

    std::vector<ProjectData*> projects;
    std::vector<ProjectWidget*> projectsVm;


};
