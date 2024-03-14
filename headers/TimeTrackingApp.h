#pragma once

#include <iostream>

#include <QtWidgets/QMainWindow>
#include <QInputDialog>
#include <qmessagebox.h>

#include "../headers/ProjectData.h"
#include "../headers/ProjectWidget.h"

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

private:
    void initSavesDir();
    void loadAndDisplayProjects();
    void askNewProject();
    void createProject(QString& newProjectName = QString("Unnammed Project"));
    bool isProjectNameUnique(QString& testProjectName);
    void newProjectWidget(ProjectData* project);
    void removeProjectWidgets();
    void refreshProjectsDisplay();
    QMessageBox* throwNewMessageBox(QString text, QFlags<QMessageBox::StandardButton> buttons, QMessageBox::StandardButton defaultButton = QMessageBox::NoButton, QString windowTitle = "");
    void configureStyleSheet();

// ATTRIBUTES //

private:
    Ui::TimeTrackingAppClass *ui;
    QVBoxLayout* scrollAreaVBoxLayout;

    std::vector<ProjectData*> projects;
    std::vector<ProjectWidget*> projectsVm;


};
