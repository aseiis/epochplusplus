#pragma once

#include <QtWidgets/QMainWindow>

#include "../headers/Project.h"
#include "../headers/ProjectWidget.h"
#include "../headers/ProjectController.h"

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
    Ui::TimeTrackingAppClass *ui;
};
